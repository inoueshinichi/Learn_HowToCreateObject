/**
 * @file interface_id.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

enum class IID
{
    ID_Unkonw = 0,
    ID_Attack,
    ID_ExtraAttack,
    ID_Defence,
    ID_ExtraDefence,
    ID_Escape,
    ID_View,
    ID_FpsView,
};

#define QUERYINTERFACE_ADD(interface_id, interface_class, class_name)          \
    case (IID::interface_id) :                                                 \
    {                                                                          \
        std::shared_ptr<interface_class> pointer;                              \
        pointer = std::static_pointer_cast<interface_class>(                   \
            std::static_pointer_cast<class_name>(shared_from_this()));         \
        return std::static_pointer_cast<IUnknown>(pointer);                    \
    }                                                                          \
    break;

#define QUERYINTERFACE_BEGIN                                           \
    virtual std::shared_ptr<IUnknown> QueryInterface(IID iid) override \
    {                                                                  \
        switch (iid)                                                   \
        {

#define QUERYINTERFACE_END              \
    }                                   \
    return std::shared_ptr<IUnknown>(); \
    }