/**
 * @file memory.hpp
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
#include <exception>
#include <stdexcept>

#include <physical_memory.hpp>

enum class DeviceMemoryState
{
    Locked = 0,
    Unlocked = 1,
};

// Memoryインターフェース
class Memory
{
private:
    std::string device_id_;
    bool locked_{false};
    Memory* prev_{nullptr};
    Memory* next_{nullptr};

    inline void disable() { ptr_ = nullptr; }

protected:
    enum class MemoryType
    {
        Normal = 0,
        Virtual = 1,
    };

    size_t bytes_{0}; // 割り当て済みバイト数
    long long requested_bytes_{-1}; // 要求バイト数
    void* ptr_{nullptr}; // 先頭ポインタ
    Memory::MemoryType memory_type_{ Memory::MemoryType::Normal }; // メモリタイプ
    VecPhysicalMemoryPtr p_memories_; // 物理メモリの配列

public:
    Memory(size_t bytes, const std::string& device_id);
    virtual ~Memory();

    // Getter
    inline size_t bytes() const { return bytes_; }
    inline size_t requested_bytes_() const { return requested_bytes_ < 0 ? bytes_ : requested_bytes_; }
    inline std::string device_id() const { return device_id_; }
    inline void* pointer() { return ptr_; }
    inline const void* const_pointer() { return ptr_; }
    inline bool locked() const { return locked_; }
    inline bool disabled() { return !ptr_; } // true if nullptr
    inline Memory* prev() const { return prev_; }
    inline Memory* next() const { return next_; }
    inline VecPhysicalMemoryPtr& get_physical_memory() { return p_memories_; }
    
    // Setter
    inline void lock() { locked_ = true; }
    inline void release() { locked_ = false; }
    inline void append_physical_memories(VecPhysicalMemoryPtr& p_mems)
    {
        for (auto& m : p_mems)
        {
            p_memories_.emplace_back(m);
        }
    }
    inline void set_requested_bytes(size_t bytes) { requested_bytes_ = bytes; }

    // Other
    inline void clear_physical_memory() { p_memories_.clear(); }
    void alloc();
    size_t bytes_active();
    std::shared_ptr<Memory> divide(size_t second_start);
    void try_merge(Memory* from);
    static void associate_consective(Memory* left, Memory* right);
    void bind(); // if Virtual
    void unbind(); // if Virtual
    bool grow(VecPhysicalMemoryPtr& p_mems);

    // Virtual
    virtual DeviceMemoryState get_device_memory_state() { return DeviceMemoryState::Unlocked; }
    virtual void lock_device_memory() {}
    virtual bool alloc_impl() = 0;
    virtual std::shared_ptr<Memory> divide_impl(size_t second_start) = 0;
    virtual void merge_prev_impl(Memory* from) = 0;
    virtual void merge_next_impl(Memory* from) = 0;
    virtual void bind_impl() { throw std::runtime_error("bind_impl() is not implemented."); }
    virtual void unbind_impl() { throw std::runtime_error("unbind_impl() is not implemented."); }
    virtual bool grow(VecPhysicalMemoryPtr& p_mems) { throw std::runtime_error("grow(VecPhysicalMemoryPtr&) is not implemented."); }
};