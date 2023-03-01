/**
 * @file interface_extra_attack.hpp
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

class IExtraAttack : public IUnknown /*IAttack*/
{
public:
    virtual ~IExtraAttack() = default;
    IExtraAttack() = default;
    IExtraAttack(const IExtraAttack &) = delete;
    IExtraAttack &operator=(const IExtraAttack &) = delete;
    IExtraAttack(IExtraAttack &&) = default;
    IExtraAttack &operator=(IExtraAttack &&) = default;

    virtual void ExtraAttack() = 0;
};