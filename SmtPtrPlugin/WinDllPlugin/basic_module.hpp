/**
 * @file basic_module.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <defs_plugin.hpp>

#include <plugin.hpp>

#include <memory>

class PluginManager;

class Plugin : public IPlugin
{
public:
    Plugin(std::shared_ptr<PluginManager> manager) : mManager(manager) {}
    ~Plugin() {}

protected:
    std::shared_ptr<PluginManager> mManager;
};