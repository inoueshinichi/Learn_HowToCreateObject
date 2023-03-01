/**
 * @file interface_view.hpp
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

class IView : public IUnknown
{
public:
    virtual ~IView() = default;
    IView() = default;
    IView(const IView &) = delete;
    IView &operator=(const IView &) = delete;
    IView(IView &&) = default;
    IView &operator=(IView &&) = default;

    virtual void View() = 0;
};