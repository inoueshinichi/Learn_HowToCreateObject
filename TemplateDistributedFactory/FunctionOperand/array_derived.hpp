/**
 * @file array_derived.hpp
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
#include <cstring>
#include <algorithm>

#include <array_base.hpp>

template <typename T>
class ArrayDerived : public ArrayBase
{
    static const unsigned int ARRAYSIZE;
    T mElements[ARRAYSIZE];

public:
    ArrayDerived() : ArrayBase() { std::memset((void *)mElements, 0, ARRAYSIZE * sizeof(T)); }
    virtual ~ArrayDerived() {}

    ArrayDerived(const ArrayDerived<T> &) = delete;
    ArrayDerived<T> &operator=(const ArrayDerived<T> &) = delete;
    ArrayDerived(ArrayDerived<T> &&) = default;
    ArrayDerived<T> &operator=(ArrayDerived<T> &&) = default;

    using Ptr = std::shared_ptr<ArrayDerived<T> >;

    // void CopyTo(Ptr &other)
    // {
    //     std::copy(this->mElements, this->mElements + ARRAYSIZE, other->GetPtr());
    // }

    // ArrayBasePtr CloneImpl() const override final
    // {
    //     auto retArrPtr = std::make_shared<ArrayDerived<T> >();
    //     this->CopyTo(retArrPtr);
    //     return retArrPtr;
    // }

    // void CopyToImpl(ArrayBasePtr &arrayBase)
    // {
        
    // }

    T *GetPtr()
    {
        return &(mElements[0]);
    }

    const T *GetConstPtr() const
    {
        return static_cast<const T *>(this->GetPtr());
    }

    unsigned int SizeImpl() const override final
    {
        return ARRAYSIZE;
    }
};

template <typename T>
const unsigned int ArrayDerived<T>::ARRAYSIZE = 10;