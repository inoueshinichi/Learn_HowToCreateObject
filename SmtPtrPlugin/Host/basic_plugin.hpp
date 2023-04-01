/**
 * @file basic_plugin.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <plugin_host.hpp>
#include <plugin_manager_host.hpp>

#include <memory>
#include <iostream>
#include <sstream>
#include <string>

class BasicPlugin final : public Plugin
{
public:
    BasicPlugin(PluginManager<BasicPlugin>& manager) : Plugin(manager), mManager(manager) {}
    ~BasicPlugin() {}

    const char *PluginName() const override final { return "BasicPlugin"; }
    int MajorVersion() const override final { return 0; }
    int MinorVersion() const override final { return 1; }
    int PatchVersion() const override final { return 0; }
    const char *CompiledDatetime() const override final { return "Host Compliled Datetime"; }
    const char *CompiledTime() const override final { return "Host Compiled Time"; }
    void About() const override final { std::cout << "About " << PluginName() << std::endl; }

protected:
    PluginManager<BasicPlugin>& mManager;
};