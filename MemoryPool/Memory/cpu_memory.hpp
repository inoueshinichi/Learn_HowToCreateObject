/**
 * @file cpu_memory.hpp
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

#include <memory.hpp>

class CpuMemory final : public Memory
{
public:
    CpuMemory(size_t bytes, const std::string& device_id);
    ~CpuMemory();
protected:
    bool alloc_impl() override final;
    std::shared_ptr<Memory> divide_impl(size_t second_start) override final;
    void merge_prev_impl(Memory* from) override final;
    void merge_next_impl(Memory* from) override final;
private:
    CpuMemory(size_t bytes, const std::string& device_id, void* ptr);
};