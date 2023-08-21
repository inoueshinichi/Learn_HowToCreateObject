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

#include <string>
#include <thread>
// #include <unordered_map> // hash
#include <unordered_set>
#include <map> // dict
#include <functional>

class PoolAllocator
{
private:
    static constexpr int _ROUND_SMALL = 512;       // 512B
    static constexpr int _ROUND_LARGE = 128 << 10; // 128KB
    static constexpr int _SMALL_ALLOC = 1 << 20;   // 1MB

protected:
    std::mutex _mtx;

public:
    using Key = size_t;
    using Value = std::tuple<size_t, Memory *>;
    using CacheMap = std::map<Key, Value>;
    CacheMap _cache_map;

    using MemoryCountMap = std::unordered_set<int>;

    std::function<void(void)> _callfack_func { nullptr };


};