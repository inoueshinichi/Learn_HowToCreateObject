/**
 * @file interface_unknown.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <interface_id.hpp>

class IUnknown
{
public:
    virtual ~IUnknown() = default;
    IUnknown() = default;
    IUnknown(const IUnknown&) = delete;
    IUnknown& operator=(const IUnknown&) = delete;
    IUnknown(IUnknown&&) = default;
    IUnknown& operator=(IUnknown&&) = default;
};