/**
 * @file allocator.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include <mutex>

#include <functional>

#include <allocator_callback.hpp>
#include <allocator_memory.hpp>
#include <memory>


// アロケータインターフェース
class Allocator : std::enable_shared_from_this<Allocator>
{
protected:
    std::unique_ptr<AllocatorCallback> callback_;
    std::unordered_map<std::string, size_t> device_memory_used_in_bytes_;
    std::mutex mutex_;
public:
    using MemCountMap = std::unordered_map<std::string, size_t>;
    std::function<void(void)> callback_tmp_ {nullptr};

    Allocator();
    virtual ~Allocator();

    AllocatorMemoryPtr alloc(size_t bytes, const std::string& device_id); // std::make_shared<AllocatorMemory>
    void free(std::shared_ptr<Memory>); // AllocatorMemoryのデストラクタで呼ぶ

    size_t free_unused_caches();
    size_t free_unused_device_caches(const std::string& device_id);
    void print_memory_cache_map() { print_memory_cache_map_impl(); }

    virtual size_t get_fragmentation_bytes(const std::string& device_id) { return 0; }
    virtual size_t get_max_available_bytes(const std::string& device_id) { return 0; }
    virtual std::vector<int> get_used_memory_count(const std::string& device_id) { return {}; }

protected:
    void alloc_retry(std::shared_ptr<Memory> mem); // memに生メモリを割り当てる

    virtual std::shared_ptr<Memory> alloc_impl(size_t bytes, const std::string& device_id) = 0;
    virtual void free_impl(std::shared_ptr<Memory> mem) = 0;
    virtual size_t free_unused_device_caches_impl(const std::string& device_id) = 0;
    virtual void print_memory_cache_map_impl() {}

private:
    Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete;
};