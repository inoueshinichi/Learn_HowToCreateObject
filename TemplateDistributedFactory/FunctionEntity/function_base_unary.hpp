/**
 * @file function_base_unary.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <function_base.hpp>

template <typename... Args>
class UnaryBaseFunction : public FunctionBase<Args...>
{
public:
    UnaryBaseFunction(const std::string& type, Args... args) : FunctionBase<Args...>(type, args...) {}
    virtual ~UnaryBaseFunction() {}

    int MinInputs() override final { return 1; }
    int MaxOutputs() override final { return 1; }

protected:
    void SetupImpl(const ArrPtrVec &inputs, const ArrPtrVec &outputs) override final
    {
        // nothing
    }
};