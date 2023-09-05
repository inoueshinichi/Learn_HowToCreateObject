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
#include <utility> // std::make_tuple
#include <tuple>
#include <vector>
#include <memory> // std::unique_ptr
#include <list>

#include <Memory/memory.hpp>
#include <Delegate/debug_delegate.hpp>

// 1MB : 104,857バイト
/**
 * @brief メモリプールの実装
 *
 * 1. ラウンディングルール (512B*k if small(<1MB) or 128KB*k if large(>=1MB))
 * 2. キャッシュマップ(small or large) ※ 変数の型毎のキャッシングマップは, 今回やっていない.
 * 3. 分割ルール (the rest of memory >= 512B if small, the rest of memory >= 1MB if large)
 * 4. メモリ(インスタンス)の貸出 : 貸出に伴う新規確保 or キャッシュマップからの使い回し, 余分なメモリは分割ルールで新規メモリインスタンス作成 & キャッシュマップに登録
 * 5. メモリ(インスタンス)の回収 : 回収時に隣接ノードとマージできないかチェックする. キャッシュマップの登録変更も更新する.
 * 6. 各機能チェックのために, MemoryDelegateで動作確認.
 */

class PoolAllocator
{
public:
    using Key = std::tuple<size_t, Memory *>; // 同じbytesで異なるMemoryインスタンスが存在するのを区別する必要がある.
    using Value = Memory *;
    using CacheMap = std::map<Key, Value>;

    using HeadMemoryPtr = Memory *;
    using MemCacheChain = std::vector<HeadMemoryPtr>;
    
private:
    // メモリ切り出しの際のラウンディングルール
    static constexpr int _SMALL_ALLOC = 1 << 20;   // 1MB: `small_caching` if * < 1MB else `large_caching`
    static constexpr int _ROUND_SMALL = 512;       // 512B
    static constexpr int _ROUND_LARGE = 128 << 10; // 128KB

    CacheMap _small_cache_map; // 小型メモリ量用キャッシュマップ
    CacheMap _large_cache_map; // 大型メモリ量用キャッシュマップ
    MemCacheChain _small_cache_chain; // 小型メモリの先頭ブロック
    MemCacheChain _large_cache_chain; // 大型メモリの先頭ブロック

    std::mutex _mtx;

    // hook (delegate)
    std::unique_ptr<MemoryDebugProtocol> _delegate;

    // ラウンディングルールによる確保するメモリ量の計算
    size_t round_size(size_t bytes);

    // キャッシュ済メモリのキャッシュマップからの削除
    void try_erase_cache(CacheMap &cache_map, Memory *mem);

    // メモリ貸出(std::bac_alloc例外対応)
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

            // debug delegate
            if (_delegate)
            {
                _delegate->on_allocation_try_failure();
            }

            // キャッシュマップで管理している確保済(未使用)のメモリ領域を解放する
            free_unused_cache_map();

            try
            {
                mem->alloc();
            }
            catch (...)
            {
                std::cerr << "Failed to allocate again." << std::endl;

                // debug delegate
                if (_delegate)
                {
                    _delegate->on_allocation_retry_failure();
                }

                throw; // 通常はメモリの再確保に失敗するとプログラムを終了させる
            }
        }
    }

    // メモリクラスのインスタンスの確保
    Memory *make_memory(size_t alloc_bytes/* round_bytes */)
    {
        return new Memory(alloc_bytes);
    }

public:
    /* funcitons */
    PoolAllocator() 
        : _delegate(new MemoryDelegate("PoolAllocator"))
    {}

    ~PoolAllocator() {}

    MemCacheChain &get_small_cache_chain() { return _small_cache_chain; }
    MemCacheChain &get_large_cache_chain() { return _large_cache_chain; }

    // メモリ(インスタンス)の貸出
    Memory *alloc(size_t orig_bytes);

    // キャッシュ済メモリの解放(キャッシュマップに戻す)
    void free(Memory *mem);

    size_t free_unused_small_cache_map()
    {
        size_t total_free_bytes = 0;

        // small
        auto iter_small = _small_cache_map.begin();
        auto iter_small_end = _small_cache_map.end();
        while (iter_small != iter_small_end)
        {
            size_t free_bytes = iter_small->second->active_bytes(); // 隣接ノードにリンクしておらず, メモリ領域割当が発生していないメモリ領域
            if (free_bytes /* 0 or bytes*/)
            {
                total_free_bytes += free_bytes;
                iter_small = _small_cache_map.erase(iter_small);
                continue;
            }
            ++iter_small;
        }

        return total_free_bytes;
    }

    size_t free_unused_large_cache_map()
    {
        size_t total_free_bytes = 0;

        // large
        auto iter_large = _large_cache_map.begin();
        auto iter_large_end = _large_cache_map.end();
        while (iter_large != iter_large_end)
        {
            size_t free_bytes = iter_large->second->active_bytes();
            if (free_bytes)
            {
                total_free_bytes += free_bytes;
                iter_large = _large_cache_map.erase(iter_large);
                continue;
            }
            ++iter_large;
        }

        return total_free_bytes;
    }

    size_t free_unused_cache_map()
    {
        size_t small_size = free_unused_small_cache_map();
        size_t large_size = free_unused_large_cache_map();

        if (_delegate) 
        {
            _delegate->on_free_unused_cache_map("small");
            _delegate->on_free_unused_cache_map("large");
        }

        return small_size + large_size;
    }

    /* ユーティリティ */

    void print_unused_cache_map();

    // small
    size_t get_max_small_cache_size()
    {
        size_t max_cache_size = 0;
        // small cache
        for (auto &kv : _small_cache_map)
        {
            max_cache_size = std::max(max_cache_size, kv.second->bytes());
        }
        return max_cache_size;
    }

    // large
    size_t get_max_large_cache_size()
    {
        size_t max_cache_size = 0;
        // large cache
        for (auto &kv : _large_cache_map)
        {
            max_cache_size = std::max(max_cache_size, kv.second->bytes());
        }
        return max_cache_size;
    }

    size_t get_max_cache_size()
    {
        size_t max_cache_size = 0;
        // small cache
        max_cache_size = std::max(max_cache_size, get_max_small_cache_size());
        // large cache
        max_cache_size = std::max(max_cache_size, get_max_large_cache_size());
        return max_cache_size;
    }

    // small
    size_t get_total_small_cache_size()
    {
        size_t total_size = 0;
        for (auto& kv : _small_cache_map)
        {
            total_size += kv.second->bytes();
        }
        return total_size;
    }

    // large
    size_t get_total_large_cache_size()
    {
        size_t total_size = 0;
        for (auto& kv : _large_cache_map)
        {
            total_size += kv.second->bytes();
        }
        return total_size;
    }

    size_t get_total_cache_size()
    {
        size_t total_size = 0;
        total_size += get_total_small_cache_size();
        total_size += get_total_large_cache_size();
        return total_size;
    }

    // small
    size_t get_max_avairable_small_cache_size()
    {
        size_t max_size = 0;
        for (auto& kv : _small_cache_map)
        {
            max_size = std::max(max_size, kv.second->bytes());
        }
        return max_size;
    }

    // large
    size_t get_max_avairable_large_cache_size()
    {
        size_t max_size = 0;
        for (auto& kv : _large_cache_map)
        {
            max_size = std::max(max_size, kv.second->bytes());
        }
        return max_size;
    }
    
    size_t get_max_avairable_cache_size()
    {
        size_t small_max = get_max_avairable_small_cache_size();
        size_t large_max = get_max_avairable_large_cache_size();
        return small_max < large_max ? small_max : large_max;
    }

    size_t get_fragmentation_small_cache_size()
    {
        return get_total_small_cache_size() - get_max_avairable_small_cache_size();
    }

    size_t get_fragmentation_large_cache_size()
    {
        return get_total_large_cache_size() - get_max_avairable_large_cache_size();
    }

    
    std::vector<int> get_used_memory_counts()
    {
        std::vector<int> ret(2);
        
        // 先頭ブロックからリストを操作(O(N))
        int small_blocks = 0;
        for (auto &head : _small_cache_chain)
        {
            Memory *mem = head;
            while (mem->next())
            {
                small_blocks++;
                mem = mem->next();
            }
            small_blocks++;
        }

        int large_blocks = 0;
        for (auto &head : _large_cache_chain)
        {
            Memory *mem = head;
            while (mem->next())
            {
                large_blocks++;
                mem = mem->next();
            }
            large_blocks++;
        }

        ret.at(0) = small_blocks;
        ret.at(1) = large_blocks;

        return ret;
    }

    void show_status_caching()
    {
        std::printf("====== Small blocks ======\n");
        std::printf("| memory | status | round bytes | required bytes | fragment bytes |\n");
        std::printf("-------------------------------------------------------------------\n");

        int index = 0;
        for (auto &head : _small_cache_chain)
        {
            index = 1;
            Memory *mem = head;
            bool is_head = true;
            while (mem->next())
            {
                std::printf("| %s Mem%d | %s | %lu | %lu | %lu |\n",
                            is_head ? "*" : "↑",
                            index,
                            mem->locked() ? "lock" : "unlock",
                            mem->bytes(),
                            mem->required_bytes(),
                            mem->bytes() - mem->required_bytes());

                mem = mem->next();
                is_head = false;
                index++;
            }

            std::printf("| %s Mem%d | %s | %lu | %lu | %lu |\n",
                        is_head ? "*" : "↑",
                        mem->prev() ? -1 : 1,
                        mem->locked() ? "lock" : "unlock",
                        mem->bytes(),
                        mem->required_bytes(),
                        mem->bytes() - mem->required_bytes());            
        }
        std::printf("====== Large blocks ======\n");
        std::printf("| memory | status | round bytes | requred bytes | fragment bytes | \n");
        std::printf("-------------------------------------------------------------------\n");

        for (auto &head : _large_cache_chain)
        {
            index = 1;
            Memory *mem = head;
            bool is_head = true;
            while (mem->next())
            {
                std::printf("| %s Mem%d | %s | %lu | %lu | %lu |\n",
                            is_head ? "*" : "↑",
                            index,
                            mem->locked() ? "lock" : "unlock",
                            mem->bytes(),
                            mem->required_bytes(),
                            mem->bytes() - mem->required_bytes());

                mem = mem->next();
                is_head = false;
                index++;
            }

            std::printf("| %s Mem%d | %s | %lu | %lu | %lu |\n",
                        is_head ? "*" : "↑",
                        mem->prev() ? -1 : 1,
                        mem->locked() ? "lock" : "unlock",
                        mem->bytes(),
                        mem->required_bytes(),
                        mem->bytes() - mem->required_bytes());
        }
    }
};

