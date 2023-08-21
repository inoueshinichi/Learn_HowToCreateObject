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

/**
 * @brief Basicクラス
 * 
 */
class BasicPluginManager final : public PluginManager
{
public:
    // protected:
    BasicPluginManager() { std::cout << "Constructor " << STRMACRO(BasicPluginManager) << std::endl; }
    ~BasicPluginManager() { std::cout << "Destructor ~" << STRMACRO(BasicPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> 
    AddPluginImpl(std::intptr_t handle, 
                  const std::string &path, 
                  const std::string &exportFactoryName) override final
    {
        return PluginManager::AddPlugin(handle, path, exportFactoryName, *this);
    }
};

/**
 * @brief macOS専用クラス
 * 
 */
class MacPluginManager final : public PluginManager
{
public:
    
// protected:
    MacPluginManager() { std::cout << "Constructor " << STRMACRO(MacPluginManager) << std::endl; }
    ~MacPluginManager() { std::cout << "Destructor ~" << STRMACRO(MacPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> 
    AddPluginImpl(std::intptr_t handle,
                  const std::string &path,
                  const std::string &exportFactoryName) override final
    {
        return PluginManager::AddPlugin(handle, path, exportFactoryName, *this);
    }
};

/**
 * @brief Linux専用クラス
 * 
 */
class LinuxPluginManager final : public PluginManager
{
public:
    // protected:
    LinuxPluginManager() { std::cout << "Constructor " << STRMACRO(LinuxPluginManager) << std::endl; }
    ~LinuxPluginManager() { std::cout << "Destructor ~" << STRMACRO(LinuxPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> 
    AddPluginImpl(std::intptr_t handle,
                  const std::string &path,
                  const std::string &exportFactoryName) override final
    {
        return PluginManager::AddPlugin(handle, path, exportFactoryName, *this);
    }
};

/**
 * @brief Windows専用クラス
 * 
 */
class WinPluginManager final : public PluginManager
{
public:
    // protected:
    WinPluginManager() { std::cout << "Constructor " << STRMACRO(WinPluginManager) << std::endl; }
    ~WinPluginManager() { std::cout << "Destructor ~" << STRMACRO(WinPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> 
    AddPluginImpl(std::intptr_t handle,
                  const std::string &path,
                  const std::string &exportFactoryName) override final
    {
        return PluginManager::AddPlugin(handle, path, exportFactoryName, *this);
    }
};

/**
 * @brief Windows API専用クラス
 * 
 */
class WinApiPluginManager final : public PluginManager
{
public:
    // protected:
    WinApiPluginManager() { std::cout << "Constructor " << STRMACRO(WinApiPluginManager) << std::endl; }
    ~WinApiPluginManager() { std::cout << "Destructor ~" << STRMACRO(WinApiPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> 
    AddPluginImpl(std::intptr_t handle,
                  const std::string &path,
                  const std::string &exportFactoryName) override final
    {
        return PluginManager::AddPlugin(handle, path, exportFactoryName, *this);
    }
};

/**
 * @brief Windows MFC専用クラス
 * 
 */
class WinMfcPluginManager final : public PluginManager
{
public:
    // protected:
    WinMfcPluginManager() { std::cout << "Constructor " << STRMACRO(WinMfcPluginManager) << std::endl; }
    ~WinMfcPluginManager() { std::cout << "Destructor ~" << STRMACRO(WinMfcPluginManager) << std::endl; }

protected:
    std::shared_ptr<Plugin> 
    AddPluginImpl(std::intptr_t handle,
                  const std::string &path,
                  const std::string &exportFactoryName) override final
    {
        return PluginManager::AddPlugin(handle, path, exportFactoryName, *this);
    }
};