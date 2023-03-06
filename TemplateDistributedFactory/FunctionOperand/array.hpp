/**
 * @file array.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <memory>
#include <array_derived.hpp>


class Array : public std::enable_shared_from_this<Array>
{
public:
    ~Array() {}
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;
    Array(Array&&) = default;
    Array& operator=(Array&&) = default;
    Array() : mArrayBase(nullptr) {}

    using Ptr = std::shared_ptr<Array>;

    template <typename T>
    T* GetPtr()
    {
        if (!mArrayBase)
        {
            mArrayBase = std::make_shared<ArrayDerived<T> >();
        }
        return std::static_pointer_cast<ArrayDerived<T> >(mArrayBase)->GetPtr();
    }

    template <typename T>
    const T* GetConstPtr()
    {
       return static_cast<const T*>(GetPtr<T>());
    }

    unsigned int Size() const
    {
        unsigned int size = 0;
        if (mArrayBase)
        {
            size = mArrayBase->Size();
        }
        return size;
    }

    // void CopyTo(Ptr other) const
    // {
    //     this->mArrayBase->CopyTo(other->GetArrayBase());
    // }

    // Ptr Clone() const
    // {
    //     Ptr retPtr = std::make_shared<Array>();
    //     ArrayBasePtr content = mArrayBase->Clone();
    //     retPtr->SetArrayBase(content);
    // }

protected:

    ArrayBasePtr GetArrayBase() const
    {
        return mArrayBase;
    }

    void SetArrayBase(ArrayBasePtr arrayBase)
    {
        mArrayBase = arrayBase;
    }

private:
    ArrayBasePtr mArrayBase;
};

using ArrayPtr = Array::Ptr;




