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
    BasicPlugin(BasicPluginManager& manager) : mManager(manager) {}
    ~BasicPlugin() {}

    void About() const override final;
    int MajorVersion() const override final;
    int MinorVersion() const override final;
    int PatchVersion() const override final;
    const char *CompiledDatetime() const override final;
    const char *CompiledTime() const override final;
    const char *FilePath() const override final;

protected:
    BasicPluginManager& mManager;
};