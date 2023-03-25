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
#include <memory>

// Plugin
#include <defs.hpp>

// Host
#include <plugin_host.hpp>
#include <basic_plugin_manager_host.hpp>
#include <winapi_plugin_manager_host.hpp>

// Plugin C Interface
extern "C"
{
    // Basic Plugin
    PLUGIN_API std::shared_ptr<Plugin> CreateBasicPlugin(BasicPluginManager& manager);

    // WinApi Plugin
    PLUGIN_API std::shared_ptr<Plugin> CreateWinApiPlugin(WinApiPluginManager& manager);
}