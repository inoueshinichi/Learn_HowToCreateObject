/**
 * @file interface_fps_view.hpp
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

class IFpsView : public IUnknown /*IView*/
{
public:
    virtual ~IFpsView() = default;
    IFpsView() = default;
    IFpsView(const IFpsView &) = delete;
    IFpsView &operator=(const IFpsView &) = delete;
    IFpsView(IFpsView &&) = default;
    IFpsView &operator=(IFpsView &&) = default;

    virtual void FpsView() = 0;
};