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

class PluginManagerBase;

class Plugin
{
public:
    Plugin(PluginManagerBase& manager) : mManager(manager) {}
    virtual ~Plugin() {};

    virtual const char *PluginName() const = 0;
    virtual int MajorVersion() const = 0;
    virtual int MinorVersion() const = 0;
    virtual int PatchVersion() const = 0;
    virtual const char *CompiledDatetime() const = 0;
    virtual const char *CompiledTime() const = 0;
    virtual void About() const = 0;

protected:

private:
    PluginManagerBase &mManager;
};