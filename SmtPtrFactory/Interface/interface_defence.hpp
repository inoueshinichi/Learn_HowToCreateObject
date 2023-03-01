/**
 * @file interface_defence.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <interface_unknown.hpp>

class IDefence : public IUnknown
{
public:
    virtual ~IDefence() = default;
    IDefence() = default;
    IDefence(const IDefence &) = delete;
    IDefence &operator=(const IDefence &) = delete;
    IDefence(IDefence &&) = default;
    IDefence &operator=(IDefence &&) = default;

    virtual void Defence() = 0;
};