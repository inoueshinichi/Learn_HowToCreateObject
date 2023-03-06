/**
 * @file gc_object.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <gc_object_base.hpp>

template <typename T>
class GCObject final : public GCObjectBase
{
public:
    GCObject();
    ~GCObject();
    GCObject(const GCObject&);
    GCObject& operator=(const GCObject& rhs);

    T* Get() const;
    T& operator*() const;
    T* operator->() const;

protected:

private:
    
};