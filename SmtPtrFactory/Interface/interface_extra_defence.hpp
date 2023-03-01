/**
 * @file interface_extra_defence.hpp
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

class IExtraDefence : public IUnknown /*IDefence*/
{
public:
    virtual ~IExtraDefence() = default;
    IExtraDefence() = default;
    IExtraDefence(const IExtraDefence &) = delete;
    IExtraDefence &operator=(const IExtraDefence &) = delete;
    IExtraDefence(IExtraDefence &&) = default;
    IExtraDefence &operator=(IExtraDefence &&) = default;

    virtual void ExtraDefence() = 0;
};