/**
 * @file interface_settable.hpp
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

class ISettable : public IUnknown
{
public:
    virtual ~ISettable() {};

    virtual void Set(int value) = 0;
};