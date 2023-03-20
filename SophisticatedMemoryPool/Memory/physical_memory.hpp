/**
 * @file physical_memory.hpp
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
#include <vector>

class PhysicalMemory
{
protected:
    bool allocated_;
    size_t bytes_;
    std::string device_id_;
public:
    explicit PhysicalMemory(size_t bytes, const std::string& device_id)
        : allocated_(false), bytes_(bytes), device_id_(device_id) {}

    ~PhysicalMemory() = default;

    virtual size_t alloc() = 0;
    
    inline size_t bytes() { return bytes_; }
    inline bool allocated() { return allocated_; }
};

using PhysicalMemoryPtr = std::shared_ptr<PhysicalMemory>;
using VecPhysicalMemoryPtr = std::vector<PhysicalMemoryPtr>;