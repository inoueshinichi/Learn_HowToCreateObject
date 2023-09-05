/**
 * @file memory.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief メモリオブジェクト
 * @version 0.1
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <exception>
#include <stdexcept>

#if 1
#include <cstdio>
#define DEBUG_LOG(...) printf(__VA_ARGS__);
#else
#define DEBUG_LOG(...)
#endif

/**
 * @brief Only CPU memory class
 * 
 */
class Memory
{
private:
    size_t _bytes; // 保持するメモリ量(バイト)
    void *_ptr;    // 先頭ポインタ
    bool _lock;    // ロック中か否か
    Memory *_prev; // 隣接する一つ前のMemoryオブジェクトへのポインタ
    Memory *_next; // 隣接する一つ後のMemoryオブジェクトへのポインタ

    inline void disable() { _ptr = (void *)nullptr; }

    void merge_prev(Memory* from) 
    {
        // マージされたメモリブロックの開始位置を表すthis->ptr_として開始ポインタを使用する
        _ptr = from->pointer();
    }

    void merge_next(Memory* from) 
    {
        // this->ptr_　は既にマージされたメモリブロックの初期位置を指している.
        // nothing
    }

public:
    Memory(size_t bytes) 
        : _bytes(bytes)
        , _ptr(nullptr)
        , _lock(false)
        , _prev(nullptr)
        , _next(nullptr)
    {}

    Memory(size_t bytes, void *ptr)
        : _bytes(bytes)
        , _ptr(nullptr)
        , _lock(false)
        , _prev(nullptr)
        , _next(nullptr)
    {
        _ptr = ptr;
    }

    ~Memory()
    {
        if (!_ptr)
        {
            return;
        }

        if (prev())
        {
            throw std::runtime_error(
                "Trying to free memory which has a prev (allocated by another memory and split previously)."
                );
        }

        DEBUG_LOG("%s: %zu at %p\n", __func__, bytes(), _ptr);
        dealloc();
    }

    // disable copy constructor and assignment
    Memory(const Memory&) = delete;
    Memory& operator=(const Memory&) = delete;

    // enable move constructor and assignment
    Memory(Memory&& other) = default;
    Memory& operator=(Memory&& rhs) = default;;

    inline size_t bytes() const { return _bytes; }
    inline void *pointer() { return _ptr; }
    inline const void *const_pointer() const { return _ptr; }
    inline bool locked() const { return _lock; }
    inline bool disabled() const { return !_ptr; }
    inline Memory *prev() const { return _prev; }
    inline Memory *next() const { return _next; }
    inline void lock() { _lock = true; }
    inline void release() { _lock = false; }

    void dealloc() 
    { 
        ::free(_ptr); 
    }

    void alloc()
    {
        if (_ptr)
        {
            throw std::runtime_error("Allocation called twice.");
        }

        _ptr = ::malloc(_bytes);

        DEBUG_LOG("%s: %zu at %p\n", __func__, bytes(), _ptr);
    }

    // 新規でMemoryインスタンスが生成される(Factory関数)
    Memory *divide(size_t second_start)
    {
        size_t out_bytes = bytes() - second_start;
        void *out_ptr = (void *)((uint8_t *)_ptr + second_start);
        Memory * new_mem = new Memory(out_bytes, out_ptr); // Memoryインスタンスを生成

        // 分割したので保持するメモリ量が減少する
        _bytes = second_start;

        // 両方向リンクリストを形成
        Memory::associate_consective(new_mem, this->_next);
        Memory::associate_consective(this, new_mem);

        return new_mem;
    }

    void try_merge(Memory *from)
    {
        if (!from || from->locked())
        {
            // fromが指すMemoryインスタンスは, 存在しない or 使用中.
            return;
        }

        if (from == this->_next)
        {
            this->merge_next(from);
            Memory::associate_consective(this, from->_next); // 両方向リンクリストからfromを除外
        }
        else if (from == this->_prev)
        {
            this->merge_prev(from);
            Memory::associate_consective(from->_prev, this); // 両方向リンクリストからfromを除外
        }

        // 統合したのでメモリ量が増加する
        _bytes += from->bytes();

        // fromが指すMemoryインスタンスを無効にする
        from->disable(); 
    }

    // アクティブバイト数
    size_t active_bytes()
    {
        if (!_ptr || _next || _prev) {
            return 0;
        }
        /* 隣接ノードがなく, まだメモリを確保していない未確保領域のバイト数 */
        return _bytes;
    }

    // 両方向リンクリストの参照先を設定する
    static void associate_consective(Memory *left, Memory *right)
    {
        if (left) { left->_next = right; }
        if (right) { right->_prev = left; }
    }
};