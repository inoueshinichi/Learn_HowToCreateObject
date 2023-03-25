/**
 * @file winapi_plugin.hpp
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

class WinApiPluginManager;

class WinApiPlugin final : public Plugin
{
public:
    WinApiPlugin(WinApiPluginManager& manager) : mManager(manager) {}
    ~WinApiPlugin() {}

    void About() const override final;
    int MajorVersion() const override final;
    int MinorVersion() const override final;
    int PatchVersion() const override final;
    const char *CompiledDatetime() const override final;
    const char *CompiledTime() const override final;
    const char *FilePath() const override final;

protected:
    WinApiPluginManager& mManager;
};