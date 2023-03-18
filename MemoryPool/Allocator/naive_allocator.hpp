/**
 * @file naive_allocator.hpp
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

#include <allocator.hpp>

template <typename MemoryType>
class NaiveAllocator final : public Allocator // Type Erasure (Allocator <- template NaiveAllocator)
{
public:
    using memory_type = MemoryType;
    NaiveAllocator();
private:
    void free_impl(std::shared_ptr<Memory> memory) override final;
    std::shared_ptr<Memory> alloc_impl(size_t orig_bytes, const std::string& device_id) override final;
    size_t free_unused_device_caches_impl(const std::string& device_id) override final;
};