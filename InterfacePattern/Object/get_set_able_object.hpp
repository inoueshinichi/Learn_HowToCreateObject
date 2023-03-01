/**
 * @file get_set_able_object.hpp
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
#include <interface_gettable.hpp>
#include <interface_settable.hpp>

#include <iostream>

class GetSetAbleObject : public Object, public IGettable, public ISettable
{
public:
    virtual ~GetSetAbleObject() { std::cout << "[Dest] GetSetAbleObject" << std::endl; }
    virtual const char *GetClassName() const override { return "GetSetAbleObject"; }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(IGETTABLE, IGettable, GetSetAbleObject)
    QUERYINTERFACE_ADD(ISETTABLE, ISettable, GetSetAbleObject)
    QUERYINTERFACE_END

    // virtual std::shared_ptr<IUnknown> QueryInterface(IID iid) override
    // {
    //     switch (iid)
    //     {
    //     case (IID::ISETTABLE):
    //     {
    //         std::shared_ptr<ISettable> pointer;
    //         pointer = std::static_pointer_cast<ISettable>(std::static_pointer_cast<GetSetAbleObject>(shared_from_this()));
    //         return std::static_pointer_cast<IUnknown>(pointer);
    //     }
    //     break;

    //     case (IID::IGETTABLE):
    //     {
    //         std::shared_ptr<IGettable> pointer;
    //         pointer = std::static_pointer_cast<IGettable>(std::static_pointer_cast<GetSetAbleObject>(shared_from_this()));
    //         return std::static_pointer_cast<IUnknown>(pointer);
    //     }
    //     break;
    //     }
    //     return std::shared_ptr<IUnknown>();
    // }

    int Get() const override { return mValue; }
    void Set(int value) override { mValue = value; }
};