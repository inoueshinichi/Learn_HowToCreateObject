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
    Memory *next = mem->next();
    mem->try_merge(next);
    Memory *prev = mem->prev();
    mem->try_merge(prev);

    // キャッシング済メモリをキャッシュマップから除外する
    try_erase_cache(cache_map, next);
    try_erase_cache(cache_map, prev);

    // 再構成されたメモリ領域をキャッシュマップに登録
    cache_map[create_key_by_memory(mem)] = mem;

    // logging
    DEBUG_LOG("cache_impl\n");
    DEBUG_PRINT_CACHES(device_cache_map, is_small);

    if (_delegate)
    {
        _delegate->on_free(bytes, is_small : "small" : "large");
    }
}

Memory *PoolAllocator::alloc(size_t orig_bytes)
{
    // クリティカルセクション
    std::lock_guard<std::mutex> locker(_mtx);

    // 少なくとも1byteを確保する. これが効率がいい. (キャッシュマップ上では0byteとする)
    size_t bytes = std::max(bytes, (size_t)1);

    // ラウンディングルールによるメモリ量の計算
    bytes = round_size(bytes);

    // witch small or large?
    bool is_small = (bytes <= /*1MB*/ _SMALL_ALLOC);

    auto &cache_map = is_small ? _small_cache_map : _large_cache_map;
    DEBUG_PRINT_CACHES(cache_map, is_small);

    Memory *mem = nullptr;
    Key key = std::make_tuple(bytes, nullptr); // Keyを作成

    // lower_boundで確保済メモリの中で使い回すメモリオブジェクトを特定する
    auto iter = cache_map.lower_bound(key);

    if (iter != cache_map.end())
    {
        /* Memoryを使い回す */
        mem = iter->second;
        cache_map.erase(iter); // キャシュマップは, 利用可能なメモリインスタンスが登録されている.
        DEBUG_LOG("[Thread]%d : Found %zu\n", std::this_thread::get_id(), mem->bytes());
    }
    else
    {
        /* 新規にUnlockなMmeoryを作成 */

        // 新規にmallocするメモリサイズ
        size_t alloc_bytes = is_small ? _SMALL_ALLOC : bytes;
        mem = this->make_memory(alloc_bytes);
        DEBUG_LOG("[Thread]%d : Alloc : %zu\n", std::this_thread::get_id(), alloc_bytes);
        alloc_retry(mem); // ここで実メモリが確保される
    }

    /* 分割ルール : 確保したメモリサイズが大きすぎる場合、メモリを分割 */
    // キャッシュされたメモリの貸出(lower_bound)で取得したバイト数は, bytesと異なり, メモリ量が大きい.
    if (mem->bytes() - bytes >= (is_small ? _ROUND_SMALL : _SMALL_ALLOC + 1))
    {
        DEBUG_LOG("Split (%d): %zu at %zu\n", (int)is_small, mem->bytes(), bytes);
        Memory *remaining = mem->divide(bytes);
        cache_map[create_key_by_memory(remaining)] = remaining; // ここでキャシュマップに登録
    }
    DEBUG_PRINT_CACHES(cache_map, is_small);


    if (_delegate)
    {
        _delegate->on_alloc(mem->bytes(), is_small ? "small" : "large");
    }

    return mem;
}