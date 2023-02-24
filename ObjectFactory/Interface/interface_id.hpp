/**
 * @file interface_id.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief インターフェースのID
 * @version 0.1
 * @date 2023-02-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <iostream>

enum class IID
{
    IUNKNOWN = 0,
    ISETTABLE,
    IGETTABLE,
};

#define QUERYINTERFACE_ADD(interface_id, interface_class, class_name)            \
    case (IID::interface_id):                                                    \
    {                                                                            \
        std::shared_ptr<interface_class> pointer;                                \
        pointer = std::static_pointer_cast<interface_class>(std::enable_shared_from_this<class_name>::shared_from_this()); \
        std::cout << #interface_class << " use_count: " << pointer.use_count() << std::endl; \
        return std::static_pointer_cast<IUnknown>(pointer);                      \
    }                                                                            \
    break;

#define QUERYINTERFACE_BEGIN                                           \
    virtual std::shared_ptr<IUnknown> QueryInterface(IID iid) override \
    {                                                                  \
        switch (iid)                                                   \
        {

#define QUERYINTERFACE_END                    \
        }                                     \
        return std::make_shared<IUnknown>();  \
    }



