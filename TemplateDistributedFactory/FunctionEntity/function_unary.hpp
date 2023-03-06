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

#include <type_tag.hpp>

template <typename T, typename UnaryOp, typename... Args>
class UnaryFunction : public UnaryBaseFunction<Args...>
{
    UnaryOp mUnaryOp;
public:
    UnaryFunction(/*const std::string& type,*/ Args... args) 
        : UnaryBaseFunction<Args...>(TypeTag<T>::value/*type*/, args...)
        , mUnaryOp(args...) {}
    virtual ~UnaryFunction() {}

    void ExecuteImpl(const ArrPtrVec &inputs, const ArrPtrVec &outputs) override final;
};

template <typename T, typename UnaryOp, typename... Args>
void UnaryFunction<T, UnaryOp, Args...>::ExecuteImpl(const ArrPtrVec &inputs, const ArrPtrVec &outputs)
{
    const T* x = inputs.at(0)->GetConstPtr<T>();
    T* y = outputs.at(0)->GetPtr<T>();
    unsigned int size = inputs.at(0)->Size();
    TransformUnary(size, x, y, mUnaryOp);
}

template <typename T, typename UnaryOp>
void TransformUnary(unsigned int arraySize, const T* x, T* y, UnaryOp op)
{
    for (unsigned int i = 0; i < arraySize; ++i)
    {
        y[i] = op(x[i]);
    }
}

