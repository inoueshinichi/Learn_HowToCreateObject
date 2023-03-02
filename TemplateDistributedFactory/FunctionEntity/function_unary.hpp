/**
 * @file function_unary.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <function_base_unary.hpp>
#include <array.hpp>

template <typename UnaryOp, typename... Args>
class UnaryFunction : public UnaryBaseFunction<Args...>
{
    UnaryOp mUnaryOp;
public:
    UnaryFunction(Args... args) 
        : UnaryBaseFunction(args...)
        , mUnaryOp(args...) {}
    virtual ~UnaryFunction() {}

    void ExecuteImpl(const ArrayVec &inputs, const ArrayVec &outputs) override final;
};

template <typename UnaryOp, typename... Args>
void UnaryFunction::ExecuteImpl(const ArrayVec &inputs, const ArrayVec &outputs)
{
    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        outputs[0][i] = mUnaryOp(inputs[0][i]);
    }
}