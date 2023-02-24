/**
 * @file interface_gettable.hpp
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

class IGettable : public IUnknown
{
public:
    virtual ~IGettable() {};

    virtual int Get() const = 0;
};