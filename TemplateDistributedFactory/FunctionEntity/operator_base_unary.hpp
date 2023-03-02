/**
 * @file operator_base_unary.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <exception>
#include <stdexcept>

class BaseUnaryOp
{
public:
    BaseUnaryOp() {}
    virtual ~BaseUnaryOp() {}

    inline float operator()(const float x) 
    { 
        throw std::runtime_error("No implementation of BaseUnaryOp"); 
    }
};