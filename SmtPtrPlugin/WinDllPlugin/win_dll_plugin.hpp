/**
 * @file win_dll_plugin.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <defs.hpp>

// Host
#include <plugin_host.hpp>

#include <memory>

class WinApiPluginManager;
class WinMfcPluginManager;

extern "C"
{
    PLUGIN_API std::shared_ptr<Plugin> CreateWinApiPlugin(WinApiPluginManager &manager);
    PLUGIN_API std::shared_ptr<Plugin> CreateWinMfcPlugin(WinMfcPluginManager &manager);
}