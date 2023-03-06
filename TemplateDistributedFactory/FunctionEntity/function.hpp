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
#include <string>

#include <array.hpp>

using ArrPtrVec = std::vector<ArrayPtr>;

// インターフェース
class Function
{
public:
    typedef std::shared_ptr<Function> Ptr;

    explicit Function(const std::string& type) : mType(type) {}
    virtual ~Function() {}/*= 0*/;
    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;
    Function(Function&&) = default;
    Function& operator=(Function&&) = default;

    void Setup(const ArrPtrVec& inputs, const ArrPtrVec& outputs) { this->SetupImpl(inputs, outputs); }
    void Execute(const ArrPtrVec& inputs, const ArrPtrVec& outputs) { this->ExecuteImpl(inputs, outputs); }

    virtual const char* GetClassName() const = 0;
    virtual Ptr Clone() const = 0;
    virtual int MinInputs() = 0;
    virtual int MaxOutputs() = 0;

    const std::string& GetType() const { return mType; }

protected:
    virtual void SetupImpl(const ArrPtrVec &inputs, const ArrPtrVec &outputs) = 0;
    virtual void ExecuteImpl(const ArrPtrVec &inputs, const ArrPtrVec &outputs) = 0;

    std::string mType;
};

using FunctionPtr = Function::Ptr;

