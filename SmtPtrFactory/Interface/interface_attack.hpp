/**
 * @file interface_attack.hpp
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

class IAttack : public IUnknown
{
public:
    virtual ~IAttack() = default;
    IAttack() = default;
    IAttack(const IAttack &) = delete;
    IAttack &operator=(const IAttack &) = delete;
    IAttack(IAttack &&) = default;
    IAttack &operator=(IAttack &&) = default;

    virtual void Attack() = 0;
};