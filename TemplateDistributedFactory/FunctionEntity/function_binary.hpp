/**
 * @file function_binary.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <function_base_binary.hpp>
#include <array.hpp>

#include <type_tag.hpp>

template <typename T, typename BinaryOp, typename... Args>
class BinaryFunction : public BinaryBaseFunction<Args...>
{
    BinaryOp mBinaryOp;
public:
    BinaryFunction(const std::string &type, Args... args)
        : BinaryBaseFunction<Args...>(type, args...), mBinaryOp(args...) {}
    virtual ~BinaryFunction() {}

    virtual void ExecuteImpl(const ArrPtrVec &inputs, const ArrPtrVec &outputs) override final;
};

template <typename T, typename BinaryOp, typename... Args>
void BinaryFunction<T, BinaryOp, Args...>::ExecuteImpl(const ArrPtrVec &inputs, const ArrPtrVec &outputs)
{
    /* z = x op y */
    const T* x = inputs.at(0)->GetConstPtr<T>();
    const T* y = inputs.at(1)->GetConstPtr<T>();
    T* z = outputs.at(0)->GetPtr<T>();
    unsigned int size = inputs.at(0)->Size();
    TransformBinary(size, x, y, z, mBinaryOp);
}

template <typename T, typename BinaryOp>
void TransformBinary(unsigned int size, const T* x, const T* y, T* z, BinaryOp op)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        z[i] = op(x[i], y[i]);
    }
}