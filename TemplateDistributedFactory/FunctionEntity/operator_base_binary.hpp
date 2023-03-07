/**
 * @file operator_base_binary.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <exception>
#include <stdexcept>

class BaseBinaryOp
{
public:
    BaseBinaryOp() {}
    virtual ~BaseBinaryOp() {}

    template <typename T>
    inline T operator()(const T x, const T y)
    {
        throw std::runtime_error("No implementation of BaseBinaryOp");
    }
};