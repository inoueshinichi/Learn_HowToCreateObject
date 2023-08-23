/**
 * @file pool_allocator.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief メモリプール付きのあロケーター
 * @version 0.1
 * @date 2023-08-21
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
// #include <unordered_map> // hash
#include <unordered_set>
#include <map> // dict
#include <functional>
#include <thread> // get_thread_id
#include <algorithm>

#include <Memory/memory.hpp>

class PoolAllocator;
using Key = std::tuple<size_t, Memory *>; // 同じbytesで異なるMemoryインスタンスが存在するのを区別する必要がある.
using Value = Memory *;
using CacheMap = std::map<Key, Value>;
using MemCountMap = std::unordered_set<int>;

Key create_key_by_memory(Memory *mem)
{
    return std::make_tuple(mem->bytes(), mem);
}

#define DEBUG_DEVICE_CACHES

#if defined(DEBUG_DEVICE_CACHES)
/*static*/ void print_cache_map(const /*PoolAllocator::*/ CacheMap &m, bool small)
{
    std::printf("cache_map(%s)\n", small ? "small" : "large");
    int index = 0;
    for (auto &i : m)
    {
        std::printf("[%d] %lu [bytes]\n", index, i.second->bytes());
        index++;
    }
}
#endif

#if defined(DEBUG_DEVICE_CACHES)
#define DEBUG_LOG(...) std::printf(__VA_ARGS__)
#define DEBUG_PRINT_CACHES(M, S) print_cache_map(M, S)
#else
#define DEBUG_LOG(...)
#define DEBUG_PRINT_CACHES(M, S)
#endif

class PoolAllocator
{
public:
    

private:
    static constexpr int _SMALL_ALLOC = 1 << 20;   // 1MB: `small_caching` if * < 1MB else `large_caching`
    static constexpr int _ROUND_SMALL = 512;       // 512B
    static constexpr int _ROUND_LARGE = 128 << 10; // 128KB

    CacheMap _small_cache_map;
    CacheMap _large_cache_map;
    MemCountMap _mem_counter;
    std::function<void(void)> _callfack_func{nullptr};

    size_t round_size(size_t bytes)
    {
        if (bytes < _ROUND_SMALL)
        {
            // small caching
            bytes = _ROUND_SMALL;
        }
        else if (bytes < _SMALL_ALLOC)
        {
            // small caching
            // -1は, 必須. これがないと, _ROUND_SMALL分だけ余分にメモリを確保してしまう.
            bytes = ((bytes + _ROUND_SMALL - 1) / _ROUND_SMALL) * _ROUND_SMALL;
        }
        else
        {
            // large caching
            bytes = ((bytes + _ROUND_LARGE - 1) / _ROUND_LARGE) * _ROUND_LARGE;
        }
        return bytes;
    }

    void try_erase_cache(CacheMap& cache_map, Memory *mem)
    {
        if (!mem /* nullptr */ || !mem->disabled() /* enabled = !_ptr */ )
        {
            return;
        }

        Key key = create_key_by_memory(mem);
        cache_map.erase(key);
    }

    void alloc_retry(Memory *mem)
    {
        /*
            メモリ確保を試みる. もし例外(std::bad_alloc)が発生したら
            利用できるすべてのメモリを解放して, 再確保を試みる.
        */
        try 
        {
            mem->alloc(); // Memoryインスタンスのbytesサイズと同じ数だけを確保
        }
        catch (...)
        {
            std::cerr << "Failed to allocate. Freeing memory cache and retrying." << std::endl;

            // ここにキャッシングしていて利用できるすべてのメモリを解放する処理を追加.

            try
            {
                mem->alloc();
            }
            catch (...)
            {
                std::cerr << "Failed to allocate again." << std::endl;

                throw; // 通常はメモリの再確保に失敗するとプログラムを終了させる
            }
        }
    }

    void free(Memory *mem)
    {
        // クリティカルセクション
        std::lock_guard<std::mutex> locker(_mtx);
        size_t bytes = mem->bytes();

        /* キャッシュに戻す前に隣接するオブジェクトとマージする */
        bool small = bytes <= _SMALL_ALLOC;
        auto& cache_map = small ? _small_cache_map : _large_cache_map;

        // try merge
        Memory *next = mem->next();
        mem->try_merge(next);
        Memory *prev = mem->prev();
        mem->try_merge(prev);

        try_erase_cache(cache_map, next);
        try_erase_cache(cache_map, prev);
    }

    Memory *make_memory(size_t alloc_bytes)
    {
        return new Memory(alloc_bytes);
    }

    Memory *alloc(size_t orig_bytes)
    {
        // クリティカルセクション
        std::lock_guard<std::mutex> locker(_mtx);

        // 少なくとも1byteを確保する. これが効率がいい
        size_t bytes = std::max(bytes, (size_t)1);

        bytes = round_size(bytes);
        bool small = (bytes <= /*1MB*/ _SMALL_ALLOC);

        auto &cache_map = small ? _small_cache_map : _large_cache_map;
        DEBUG_PRINT_CACHES(cache_map, small);

        Memory *mem = nullptr;
        Key key = std::make_tuple(bytes, nullptr);
        auto iter = cache_map.lower_bound(key); // lower_boundでキャッシング中の使い回すMemryを決める
        if (iter != cache_map.end())
        {
            /* Memoryを使い回す */
            mem = iter->second;
            cache_map.erase(iter);
            DEBUG_LOG("[Thread]%d : Found %zu\n", std::this_thread::get_id(), mem->bytes());
        }
        else
        {
            /* 新規にUnlockなMmeoryを作成 */
            
            // 新規にmallocするメモリサイズ
            size_t alloc_bytes = small ? _SMALL_ALLOC : bytes;
            mem = this->make_memory(alloc_bytes);
            DEBUG_LOG("[Thread]%d : Alloc : %zu\n", std::this_thread::get_id(), alloc_bytes);
            alloc_retry(mem); // ここで実メモリが確保される
        }

        /* 分割ルール : 確保したメモリサイズが大きすぎる場合、メモリを分割 */
        // キャッシュされたメモリの貸出(lower_bound)で取得したバイト数は, bytesと異なり, メモリ量が大きい.
        if (mem->bytes() - bytes >= (small ? _ROUND_SMALL : _SMALL_ALLOC + 1))
        {
            DEBUG_LOG("Split (%d): %zu at %zu\n", (int)small, mem->bytes(), bytes);
            Memory *remaining = mem->divide(bytes);
            cache_map[create_key_by_memory(remaining)] = remaining;
        }
        DEBUG_PRINT_CACHES(cache_map, small);
        return mem;
    }

protected:
    std::mutex _mtx;
};

