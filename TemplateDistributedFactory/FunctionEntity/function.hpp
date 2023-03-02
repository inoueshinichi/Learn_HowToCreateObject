/**
 * @file entity.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <exception> // std::exception + function
#include <stdexcept> // 標準的な例外を提供

#include <memory>
#include <vector>
#include <tuple>

#include <array.hpp>

using ArrayVec = std::vector<ArrayPtr>;

// インターフェース
class Function
{
public:
    typedef std::shared_ptr<Function> Ptr;

    Function() {}
    virtual ~Function() = 0;
    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;
    Function(Function&&) = default;
    Function& operator=(Function&&) = default;

    void Setup(const ArrayVec& inputs, const ArrayVec& outputs) { this->SetupImpl(inputs, outputs); }
    void Execute(const ArrayVec& inputs, const ArrayVec& outputs) { this->ExecuteImpl(inputs, outputs); }

    virtual const char* GetObjectName() = 0;
    virtual Ptr Clone() = 0;
    virtual int MinInputs() = 0;
    virtual int MaxOutputs() = 0;

protected:
    virtual void SetupImpl(const ArrayVec &inputs, const ArrayVec &outputs) = 0;
    virtual void ExecuteImpl(const ArrayVec &inputs, const ArrayVec &outputs) = 0;
};

using FunctionPtr = Function::Ptr;

