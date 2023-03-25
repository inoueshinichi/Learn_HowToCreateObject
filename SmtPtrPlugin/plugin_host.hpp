/**
 * @file plugin.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

class PluginInfo;
class PluginManager;

class Plugin
{
public:
    Plugin(PluginManager& manager) : mManager(manager) {}
    virtual ~Plugin() {};

    virtual const char *PluginName() const = 0;

    int MajorVersion() const;
    int MinorVersion() const;
    int PatchVersion() const;
    const char *CompiledDatetime() const;
    const char *CompiledTime() const;
    const char *DllFilePath() const;
    void About() const;

protected:

private:
    PluginManager &mManager;
};