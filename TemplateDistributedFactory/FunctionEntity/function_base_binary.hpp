/**
 * @file function_base_binary.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <function_base.hpp>


template <typename... Args>
class BinaryBaseFunction : public FunctionBase<Args...>
{
public:
    BinaryBaseFunction(const std::string& type, Args... args) : FunctionBase<Args...>(type, args...) {}
    virtual ~BinaryBaseFunction() {}

    int MinInputs() override final { return 2; }
    int MinOutputs() override final { return 1; }

protected:
    void SetupImpl(const ArrPtrVec &inputs, const ArrPtrVec &outputs) override final
    {
        // nothing
    }
};