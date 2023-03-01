/**
 * @file interface_escape.hpp
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

class IEscape : public IUnknown
{
public:
    virtual ~IEscape() = default;
    IEscape() = default;
    IEscape(const IEscape &) = delete;
    IEscape &operator=(const IEscape &) = delete;
    IEscape(IEscape &&) = default;
    IEscape &operator=(IEscape &&) = default;

    virtual void Escape() = 0;
};