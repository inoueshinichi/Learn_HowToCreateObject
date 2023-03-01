/**
 * @file settable_object.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <object.hpp>
#include <interface_settable.hpp>

#include <iostream>

class SettableObject : public Object, public ISettable
{
public:
    virtual ~SettableObject() { std::cout << "[Dest] SattableObject" << std::endl; }
    virtual const char *GetClassName() const override { return "SettableObject"; }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(ISETTABLE, ISettable, SettableObject)
    QUERYINTERFACE_END

    // virtual std::shared_ptr<IUnknown> QueryInterface(IID iid) override
    // {
    //     switch (iid)
    //     {
    //     case (IID::ISETTABLE):
    //     {
    //         std::shared_ptr<ISettable> pointer;
    //         pointer = std::static_pointer_cast<ISettable>(std::static_pointer_cast<SettableObject>(shared_from_this()));
    //         return std::static_pointer_cast<IUnknown>(pointer);
    //     }
    //     break;

    //      }
    //      return std::shared_ptr<IUnknown>();
    // }

    void Set(int value) override { mValue = value; }
};