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
#include <defs.hpp>

#include <plugin_host.hpp>

#include <memory>

class BasicPluginManager;

class BasicPlugin final : public Plugin
{
public:
    BasicPlugin(BasicPluginManager& manager) : Plugin(manager), mManager(manager) {}
    ~BasicPlugin() {}

protected:
    BasicPluginManager& mManager;
};