/**
 * @file array_base.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <memory>
#include <stdexcept>

class ArrayBase : public std::enable_shared_from_this<ArrayBase>
{
public:
    ArrayBase() {}
    virtual ~ArrayBase() {}
    ArrayBase(const ArrayBase &) = delete;
    ArrayBase &operator=(const ArrayBase &) = delete;
    ArrayBase(ArrayBase &&) = default;
    ArrayBase &operator=(ArrayBase &&) = default;

    using Ptr = std::shared_ptr<ArrayBase>;

    unsigned int Size() const { return this->SizeImpl(); }
    // void CopyTo(Ptr other) const { this->CopyToImpl(other); }
    // Ptr Clone() const { /* Cast ArrayDerivedPtr -> ArrayBasePtr */ return this->CloneImpl(); }

protected:
    virtual unsigned int SizeImpl() const
    {
        throw std::runtime_error("No implementation SizeImpl func of ArrayBase");
    }

    // virtual void CopyToImpl(Ptr other) const
    // {
    //     throw std::runtime_error("No implementation CopyToImpl func of ArrayBase");
    // }

    // virtual Ptr CloneImpl() const 
    // {
    //     throw std::runtime_error("No implementation CloneImpl func of ArrayBase");
    // }
};

using ArrayBasePtr = ArrayBase::Ptr;