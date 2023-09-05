/**
 * @file pool_allocator.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <pool_allocator.hpp>

#define DEBUG_DEVICE_CACHES


#if defined(DEBUG_DEVICE_CACHES)
#define DEBUG_LOG(...) std::printf(__VA_ARGS__)
#define DEBUG_PRINT_CACHES(M, S) print_cache_map(M, S)
#else
#define DEBUG_LOG(...)
#define DEBUG_PRINT_CACHES(M, S)
#endif

#if defined(DEBUG_DEVICE_CACHES)


#endif

// MemoryインスタンスからKeyを作成
inline PoolAllocator::Key create_key_by_memory(Memory *mem)
{
    return std::make_tuple(mem->bytes(), mem);
}

#if defined(DEBUG_DEVICE_CACHES)
static void print_cache_map(const PoolAllocator::CacheMap &m, bool is_small)
{
    std::printf("cache_map(%s)\n", is_small ? "small" : "large");
    int index = 0;
    for (auto &i : m)
    {
        std::printf("[%d] %lu [bytes]\n", index, i.second->bytes());
        index++;
    }
}
#endif



size_t PoolAllocator::round_size(size_t bytes)
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

void PoolAllocator::try_erase_cache(CacheMap &cache_map, Memory *mem)
{
    if (!mem /* nullptr */ || !mem->disabled() /* enabled = !_ptr */)
    {
        return;
    }

    Key key = create_key_by_memory(mem);
    cache_map.erase(key);
}

void PoolAllocator::free(Memory *mem)
{
    // クリティカルセクション
    std::lock_guard<std::mutex> locker(_mtx);
    size_t bytes = mem->bytes();

    /* キャッシュに戻す前に隣接するオブジェクトとマージする */
    bool is_small = bytes <= _SMALL_ALLOC;
    auto &cache_map = is_small ? _small_cache_map : _large_cache_map;
    
    // try merge
    Memory *next = mem->next(); // コピー
    mem->try_merge(next);
    Memory *prev = mem->prev(); // コピー
    mem->try_merge(prev);

    // キャッシング済メモリをキャッシュマップから除外する
    try_erase_cache(cache_map, next);
    try_erase_cache(cache_map, prev);

    // 再構成されたメモリ領域をキャッシュマップに登録
    cache_map[create_key_by_memory(mem)] = mem;

    // logging
    DEBUG_LOG("cache_impl\n");
    DEBUG_PRINT_CACHES(cache_map, is_small);

    // CacheChainの管理
    auto &cache_chain = is_small ? _small_cache_chain : _large_cache_chain;
    auto iter = std::find(cache_chain.begin(), cache_chain.end(), mem);
    
    // CacheChainの先頭ポインタが消せるかチェック
    if (iter != cache_chain.end())
    {
        bool is_lock = false;
        Memory *mem = *iter;
        Memory *head = *iter;
        while (mem->next())
        {
            if (mem->locked())
            {
                is_lock = true;
                break;
            }
            mem = mem->next();
        }

        if (!is_lock)
        {
            // 各Poolブロックの先頭メモリに該当するポインタを削除
            cache_chain.erase(iter);
            delete head;
        }
    }
    else
    {
        // pool_blockの先頭メモリをマージした場合,
        // CacheChainの先頭ポインタを入れ替える
        iter = std::find(cache_chain.begin(), cache_chain.end(), prev);
        if (iter != cache_chain.end())
        {
            *iter = mem;
        }
    }


    if (_delegate)
    {
        _delegate->on_free(bytes, is_small ? "small" : "large");
    }
}

Memory *PoolAllocator::alloc(size_t orig_bytes)
{
    // クリティカルセクション
    std::lock_guard<std::mutex> locker(_mtx);

    // 少なくとも1byteを確保する. これが効率がいい. (キャッシュマップ上では0byteとする)
    size_t required_bytes = std::max(orig_bytes, (size_t)1);

    // ラウンディングルールによるメモリ量の計算
    size_t round_bytes = round_size(required_bytes);

    // witch small or large?
    bool is_small = (round_bytes <= /*1MB*/ _SMALL_ALLOC);

    auto &cache_map = is_small ? _small_cache_map : _large_cache_map;
    DEBUG_PRINT_CACHES(cache_map, is_small);

    // デバッグ用メモリ配置
    auto &cache_chain = is_small ? _small_cache_chain : _large_cache_chain;

    Memory *mem = nullptr;
    Key key = std::make_tuple(round_bytes, nullptr); // Keyを作成

    // lower_boundで確保済メモリの中で使い回すメモリオブジェクトを特定する
    auto iter = cache_map.lower_bound(key);

    if (iter != cache_map.end())
    {
        /* Memoryを使い回す */
        mem = iter->second;
        cache_map.erase(iter); // キャシュマップは, 利用可能なメモリインスタンスが登録されている.
        DEBUG_LOG("[Thread]%d : Found! %zu\n", std::this_thread::get_id(), mem->bytes());
    }
    else
    {
        /* 新規にUnlockなMmeoryを作成 */

        // 新規にmallocするメモリサイズ
        size_t alloc_bytes = is_small ? _SMALL_ALLOC : round_bytes;
        mem = this->make_memory(alloc_bytes);
        DEBUG_LOG("[Thread]%d : First-alloc! : %zu bytes\n", std::this_thread::get_id(), alloc_bytes);
        alloc_retry(mem); // ここで実メモリが確保される

        // デバッグ用メモリ配置
        cache_chain.push_back(/* HeadMemoryPtr */mem);
    }

    /* 分割ルール : 確保したメモリサイズが大きすぎる場合、メモリを分割 */
    // キャッシュされたメモリの貸出(lower_bound)で取得したバイト数は, bytesと異なり, メモリ量が大きい.
    if (mem->bytes() - round_bytes >= (is_small ? _ROUND_SMALL : _SMALL_ALLOC + 1))
    {
        DEBUG_LOG("Split (%d): %zu at %zu\n", (int)is_small, mem->bytes(), round_bytes);
        Memory *remaining = mem->divide(round_bytes);
        cache_map[create_key_by_memory(remaining)] = remaining; // ここでキャシュマップに登録
    }

    DEBUG_PRINT_CACHES(cache_map, is_small);


    if (_delegate)
    {
        _delegate->on_alloc(mem->bytes(), is_small ? "small" : "large");
    }

    return mem;
}