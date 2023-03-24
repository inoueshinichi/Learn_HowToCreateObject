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
#include <defs_plugin.hpp>

// Host
#include <plugin.hpp>
#include <plugin_manager_host.hpp>
#include <winapi_plugin_manager_host.hpp>

// Plugin C Interface
extern "C"
{
    // Basic Module
    PLUGIN_API std::shared_ptr<IPlugin> CreatePlugin(std::shared_ptr<PluginManager> manager);

    // WinApi Module
    PLUGIN_API std::shared_ptr<IPlugin> CreateWinApiPlugin(std::shared_ptr<WinApiPluginManager> manager);
}