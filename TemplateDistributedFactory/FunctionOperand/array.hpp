/**
 * @file array.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <memory>
#include <cstring>

constexpr int ARRAY_SIZE = 10;

class Array
{
    float mElements[ARRAY_SIZE];
public:
    Array() { std::memset((void*)mElements, 0, ARRAY_SIZE * sizeof(float)); }
    virtual ~Array() {}

    Array(const Array &) = delete;
    Array &operator=(const Array &) = delete;
    Array(Array &&) = default;
    Array &operator=(Array &&) = default;

    void CopyTo(Array& array);
    Array Clone();

    float& operator[](int i) { assert(i >=0); assert(i < ARRAY_SIZE); return mElements[i]; }
};

using ArrayPtr = std::shared_ptr<Array>;