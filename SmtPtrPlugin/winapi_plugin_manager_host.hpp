/**
 * @file winapi_plugin_manager_host.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <plugin_manager_host.hpp>

class WinApiPluginManager : public PluginManager
{
public:
    WinApiPluginManager() = default;
    ~WinApiPluginManager() = default;
    WinApiPluginManager(const WinApiPluginManager&) = delete;
    WinApiPluginManager& operator=(const WinApiPluginManager&) = delete;
    WinApiPluginManager(WinApiPluginManager&& rhs) = default;
    WinApiPluginManager& operator=(WinApiPluginManager&& rhs) = default;  
private:
};