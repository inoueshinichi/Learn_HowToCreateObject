/**
 * @file gettable_object.hpp
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

#include <iostream>

class GettableObject : public Object, public IGettable
{
public:
    virtual ~GettableObject() { std::cout << "[Dest] GettableObject" << std::endl; }
    virtual const char *GetClassName() const { return "GettableObject"; }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(IGETTABLE, IGettable, GettableObject)
    QUERYINTERFACE_END

    // virtual std::shared_ptr<IUnknown> QueryInterface(IID iid) override
    // {
    //     switch (iid)
    //     {

    //     case (IID::IGETTABLE):
    //     {
    //         std::shared_ptr<IGettable> pointer;
    //         pointer = std::static_pointer_cast<IGettable>(std::static_pointer_cast<GettableObject>(shared_from_this()));
    //         return std::static_pointer_cast<IUnknown>(pointer);
    //     }
    //     break;
    //     }
    //     return std::shared_ptr<IUnknown>();
    // }

    int Get() const override { return mValue; }
};