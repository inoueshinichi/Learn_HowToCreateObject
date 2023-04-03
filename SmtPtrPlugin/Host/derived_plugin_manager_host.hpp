/**
 * @file basic_plugin_manager_host.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <iostream>

#include <plugin_manager_host.hpp>


class BasicPluginManager final : public PluginManager
{
public:
    // protected:
    BasicPluginManager() { std::cout << "Constructor " << STRMACRO(BasicPluginManager) << std::endl; }
    ~BasicPluginManager() { std::cout << "Destructor ~" << STRMACRO(BasicPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> GetPluginImpl(const std::string &path, const std::string &exportFactoryName) override final
    {
        std::intptr_t id;
        std::shared_ptr<Plugin> pluginPtr = AddPlugin(id, path, exportFactoryName, *this);
        if (id == std::intptr_t(NULL))
        {
            std::ostringstream oss;
            oss << "No plugin creator: "
                << "std::shared_ptr<MacPlugin> "
                << exportFactoryName << "(" << STRMACRO(BasicPluginManager) << "&) in " << path;
            throw std::runtime_error(oss.str());
        }

        return pluginPtr;
    }
};

class MacPluginManager final : public PluginManager
{
public:
    
// protected:
    MacPluginManager() { std::cout << "Constructor " << STRMACRO(MacPluginManager) << std::endl; }
    ~MacPluginManager() { std::cout << "Destructor ~" << STRMACRO(MacPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> GetPluginImpl(const std::string &path, const std::string &exportFactoryName) override final
    {
        std::intptr_t id;
        std::shared_ptr<Plugin> pluginPtr = AddPlugin(id, path, exportFactoryName, *this);
        if (id == std::intptr_t(NULL))
        {
            std::ostringstream oss;
            oss << "No plugin creator: "
                << "std::shared_ptr<MacPlugin> "
                << exportFactoryName << "(" << STRMACRO(MacPluginManager) << "&) in " << path;
            throw std::runtime_error(oss.str());
        }

        return pluginPtr;
    }
};

class LinuxPluginManager final : public PluginManager
{
public:
    // protected:
    LinuxPluginManager() { std::cout << "Constructor " << STRMACRO(LinuxPluginManager) << std::endl; }
    ~LinuxPluginManager() { std::cout << "Destructor ~" << STRMACRO(LinuxPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> GetPluginImpl(const std::string &path, const std::string &exportFactoryName) override final
    {
        std::intptr_t id;
        std::shared_ptr<Plugin> pluginPtr = AddPlugin(id, path, exportFactoryName, *this);
        if (id == std::intptr_t(NULL))
        {
            std::ostringstream oss;
            oss << "No plugin creator: "
                << "std::shared_ptr<MacPlugin> "
                << exportFactoryName << "(" << STRMACRO(LinuxPluginManager) << "&) in " << path;
            throw std::runtime_error(oss.str());
        }

        return pluginPtr;
    }
};


class WinPluginManager final : public PluginManager
{
public:
    // protected:
    WinPluginManager() { std::cout << "Constructor " << STRMACRO(WinPluginManager) << std::endl; }
    ~WinPluginManager() { std::cout << "Destructor ~" << STRMACRO(WinPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> GetPluginImpl(const std::string &path, const std::string &exportFactoryName) override final
    {
        std::intptr_t id;
        std::shared_ptr<Plugin> pluginPtr = AddPlugin(id, path, exportFactoryName, *this);
        if (id == std::intptr_t(NULL))
        {
            std::ostringstream oss;
            oss << "No plugin creator: "
                << "std::shared_ptr<MacPlugin> "
                << exportFactoryName << "(" << STRMACRO(WinPluginManager) << "&) in " << path;
            throw std::runtime_error(oss.str());
        }

        return pluginPtr;
    }
};

class WinApiPluginManager final : public PluginManager
{
public:
    // protected:
    WinApiPluginManager() { std::cout << "Constructor " << STRMACRO(WinApiPluginManager) << std::endl; }
    ~WinApiPluginManager() { std::cout << "Destructor ~" << STRMACRO(WinApiPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> GetPluginImpl(const std::string &path, const std::string &exportFactoryName) override final
    {
        std::intptr_t id;
        std::shared_ptr<Plugin> pluginPtr = AddPlugin(id, path, exportFactoryName, *this);
        if (id == std::intptr_t(NULL))
        {
            std::ostringstream oss;
            oss << "No plugin creator: "
                << "std::shared_ptr<MacPlugin> "
                << exportFactoryName << "(" << STRMACRO(WinApiPluginManager) << "&) in " << path;
            throw std::runtime_error(oss.str());
        }

        return pluginPtr;
    }
};

class WinMfcPluginManager final : public PluginManager
{
public:
    // protected:
    WinMfcPluginManager() { std::cout << "Constructor " << STRMACRO(WinMfcPluginManager) << std::endl; }
    ~WinMfcPluginManager() { std::cout << "Destructor ~" << STRMACRO(WinMfcPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> GetPluginImpl(const std::string &path, const std::string &exportFactoryName) override final
    {
        std::intptr_t id;
        std::shared_ptr<Plugin> pluginPtr = AddPlugin(id, path, exportFactoryName, *this);
        if (id == std::intptr_t(NULL))
        {
            std::ostringstream oss;
            oss << "No plugin creator: "
                << "std::shared_ptr<MacPlugin> "
                << exportFactoryName << "(" << STRMACRO(WinMfcPluginManager) << "&) in " << path;
            throw std::runtime_error(oss.str());
        }

        return pluginPtr;
    }
};