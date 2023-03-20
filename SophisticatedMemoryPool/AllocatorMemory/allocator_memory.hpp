/**
 * @file allocator_memory.hpp
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

#include <memory.hpp>

class Allocator; // 前方宣言. AllocatorはAllocatorMemoryへの依存性の注入に使う.

// Memoryクラスのラッパー(RAII): std::shared_ptr<Memory>の生存期間を遅延させる役割を持つ
class AllocatorMemory
{
public:
    AllocatorMemory();
    AllocatorMemory(std::shared_ptr<Memory> memory, std::shared_ptr<Allocator> allocator);

    AllocatorMemory(AllocatorMemory&& rhs);
    AllocatorMemory& operator=(AllocatorMemory&& rhs);

    AllocatorMemory(const AllocatorMemory&) = delete;
    AllocatorMemory& operator=(const AllocatorMemory&) = delete;

    inline std::shared_ptr<Memory> memory() { return memory_; }
    inline std::string device_id() const { return memory_->device_id(); }
    inline void* pointer() { return memory_->pointer(); }
    inline const void* const_pointer() const { return memory_->const_pointer(); }

private:
    // ラップ対象のMemoryクラス
    std::shared_ptr<Memory> memory_;
    // 依存性の注入
    std::shared_ptr<Allocator> allocator_;

    void release();
};

using AllocatorMemoryPtr = std::shared_ptr<AllocatorMemory>;