/**
 * @file mac_dll_plugin.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <defs.hpp>

// Host
#include <plugin_host.hpp>

#include <memory>

class MacPluginManager;

extern "C"
{
    PLUGIN_API std::shared_ptr<Plugin> CreateMacPlugin(MacPluginManager &manager);
}