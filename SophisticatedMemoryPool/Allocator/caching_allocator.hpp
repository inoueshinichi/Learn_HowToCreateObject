/**
 * @file caching_allocator.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <string>
#include <memory>
#include <tuple>
#include <map>
#include <unordered_map>
#include <vector>

#include <allocator.hpp>
#include <memory.hpp>


/**
 * @brief メモリプール用アロケータ
 * @note メモリプールの規則
 * @note サイズ依存型メモリプール(small: <1MB=small_alloc_, large: other)
 * @note ラウンディングルール
 *  1. round_small_ 512B(by default)の整数倍で確保するメモリ数を決定
 *  2. round_large_ 128KB(by default)の整数倍で確保するメモリ数を決定
 *  3. その他
 * @note 1と2がsmall, 3がlargeに該当
 * @note メモリ確保のルール
 *  1. 以前生成したどのメモリサイズよりも大きい場合, 新規でメモリブロックを確保
 *  2. それ以外: 分割ルールを適用後, 最もサイズの小さいメモリブロックを割り当てる
 * @note 分割ルール
 *  1. smallメモリの場合, 利用可能なメモリブロックの検索の結果, round_small_以上の場合, メモリブロックを2つに分割. 分割した前半メモリブロックを使用. 後半メモリブロックはプールに戻す.
 *  2. largeメモリの場合, 同上の結果, small_alloc_(1MB by default) + 1以上の場合, 同上の処理を適用する.
 */

// CachingAllocatorのベースクラス
class CachingAllocatorBase : public Allocator
{
public:
    using Key = std::tuple<size_t, Memory*>;
    using DeviceCacheMap = std:map<Key, std::shared_ptr<Memory>>;
    using CacheMap = std::unordered_map<std::string, DeviceCacheMap>;

    CachingAllocatorBase();

    size_t get_fragmentation_bytes(const std::string& device_id) override final;
    size_t get_max_available_bytes(const std::string& device_id) override final;
    std::vector<int> get_used_memory_counts(const std::string& device_id) override final;
protected:
    std::shared_ptr<Memory> make_memory(size_t bytes, const std::string& device_id); // 再利用可能なメモリブロックがなかった場合, alloc_impl関数内部で呼ばれる
    virtual std::shared_ptr<Memory> make_memory_impl(size_t bytes, const std::string &device_id) = 0; // 純粋仮想
private:
    /* std::unordered_map<std::string, DeviceCacheMap> */
    CacheMap small_cache_map_;
    CacheMap large_cache_map_;
    /* std::unordered_map<std::string, size_t> */
    MemCountMap small_memory_counter_;
    MemCountMap large_memory_counter_;
    /* Memory pool constant variables */
    static constexpr int round_small_ = 512;       // 512B
    static constexpr int round_large_ = 128 << 10; // 128KB
    static constexpr int small_alloc_ = 1 << 20;   // 1MB

    size_t round_size(size_t bytes) const;

    // Override
    void free_impl(std::shared_ptr<Memory> memory) override final;
    std::shared_ptr<Memory> alloc_impl(size_t orig_bytes, const std::string& device_id) override final;
    size_t free_unused_device_caches_impl(const std::string& device_id) override final;
    void print_memory_cache_map_impl() override final;

    size_t get_max_cache_bytes(const std::string& device_id);
    size_t get_total_cache_bytes(const std::string& device_id);
};


// CachingAllocatorの実装クラス
template <typename MemoryType>
class CachingAllocator : public CachingAllocatorBase // Type Erasure (CachingAllocatorBase <- CachingAllocator)
{
public:

};