/**
 * @file win_api_plugin.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// Plugin
#include <defs.hpp>

// Host
#include <plugin_host.hpp>
#include <plugin_manager_host.hpp>

#include <memory>

class WinApiPlugin;
template class PluginManager<WinApiPlugin>;

// Plugin C Interface
#if defined(__cplusplus)
extern "C"
{
#endif

    // WinApi Plugin
    WIN_PLUGIN_API std::shared_ptr<WinApiPlugin> CreateWinApiPlugin(PluginManager<WinApiPlugin> &manager);

#if defined(__cplusplus)
}
#endif