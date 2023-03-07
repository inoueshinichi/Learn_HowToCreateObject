/**
 * @file function_add.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <function_binary.hpp>
#include <operator_base_binary.hpp>

#include <template_distributed_registry.hpp>
#include <template_distributed_database.hpp>
#include <template_distributed_dbitem.hpp>

#include <string>

/* Get_AddRegistry */
Registry<Function>& Get_AddRegistry();

/* Create_Add */
FunctionPtr Create_Add(const std::string& type);


/* AddBinaryOp */
class AddBinaryOp : public BaseBinaryOp
{
public:
    AddBinaryOp() {}

    template <typename T>
    inline T operator()(const T x, const T y)
    {
        return x + y;
    }
};


template <typename T>
class Add final : public BinaryFunction<T, AddBinaryOp>
{
public:
    ~Add() {}
    const char* GetClassName() const override final { return "Add"; }

    Add(const std::string& a0) : BinaryFunction<T, AddBinaryOp>(a0) {}

    FunctionPtr Clone() const override final
    {
        return Create_Add(this->mType);
    }
};