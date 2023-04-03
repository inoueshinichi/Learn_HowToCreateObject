/**
 * @file winmfc_plugin.hpp
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
#include <plugin_tag.hpp>
#include <derived_plugin_manager_host.hpp>

#include <memory>
#include <iostream>
#include <string>

class WinMfcPlugin final : public Plugin
{
public:
    WinMfcPlugin(WinMfcPluginManager &manager) : Plugin(manager), mManager(manager) {}
    ~WinMfcPlugin() {}

    const char *PluginName() const override final { return PluginTag<WinMfcPlugin>::str; }
    int MajorVersion() const override final { return std::stoi(PLUGIN_VERSION_MAJOR); }
    int MinorVersion() const override final { return std::stoi(PLUGIN_VERSION_MINOR); }
    int PatchVersion() const override final { return std::stoi(PLUGIN_VERSION_PATCH); }
    const char *CompiledDatetime() const override final { return PLUGIN_COMPILED_DATETIME; }
    const char *CompiledTime() const override final { return PLUGIN_COMPILED_TIME; }
    void About() const override final { std::cout << "About " << PluginName() << std::endl; }

protected:
    WinApiPluginManager &mManager;
};