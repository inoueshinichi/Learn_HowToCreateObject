/**
 * @file object.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <interface_unknown.hpp>

class Object : public std::enable_shared_from_this<Object>
{
public:
    Object() : mValue(0) {}
    virtual ~Object() {}
    virtual const char* GetClassName() const { return "Object"; }

// QUERYINTERFACE_BEGIN
//     QUERYINTERFACE_ADD(IUNKNOWN, IUnknown)
// QUERYINTERFACE_END

    virtual std::shared_ptr<IUnknown> QueryInterface(IID iid) = 0;

protected:
    int mValue;
};