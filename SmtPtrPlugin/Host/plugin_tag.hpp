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
    constexpr static const char* str = STRMACRO(BasicPlugin);
};

class MacPlugin;
template <> struct PluginTag<MacPlugin>
{
    constexpr static const char *str = STRMACRO(MacPlugin);
};

class LinuxPlugin;
template <>
struct PluginTag<LinuxPlugin>
{
    constexpr static const char *str = STRMACRO(LinuxPlugin);
};

class WinApiPlugin;
template <>
struct PluginTag<WinApiPlugin>
{
    constexpr static const char *str = STRMACRO(WinApiPlugin);
};

class WinMfcPlugin;
template <>
struct PluginTag<WinMfcPlugin>
{
    constexpr static const char *str = STRMACRO(WinMfcPlugin);
};