/**
 * @file plugin_tag.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#define STRMACRO(name) #name

template <typename PLUGIN>
struct PluginTag;

class BasicPlugin;
template <> struct PluginTag<BasicPlugin>
{
    constexpr static char* str = STRMACRO(BasicPlugin);
};

class WinApiPlugin;
template <> struct PluginTag<WinApiPlugin>
{
    constexpr static char* str = STRMACRO(WinApiPlugin);
};