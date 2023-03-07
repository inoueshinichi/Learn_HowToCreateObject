/**
 * @file function_sin.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <function_unary.hpp>
#include <operator_base_unary.hpp>

#include <template_distributed_registry.hpp>
#include <template_distributed_database.hpp>
#include <template_distributed_dbitem.hpp>

#include <cmath>
#include <string>

/* Get_SinRegistry */
Registry<Function>& Get_SinRegistry();

/* Create_Sin */
FunctionPtr Create_Sin(const std::string& type);


/* SinUnaryOp */
class SinUnaryOp : public BaseUnaryOp
{
public:
    SinUnaryOp() {}

    template <typename T>
    inline T operator() (const T x)
    {
        return std::sin(x);
    }
};

/* Sin */
template <typename T>
class Sin final : public UnaryFunction<T, SinUnaryOp>
{
public:
    ~Sin() {}
    const char* GetClassName() const override final { return "Sin"; }

    Sin(const std::string& a0) : UnaryFunction<T, SinUnaryOp>(a0) {}

    FunctionPtr Clone() const override final
    {
        return Create_Sin(this->mType);
    }

};
