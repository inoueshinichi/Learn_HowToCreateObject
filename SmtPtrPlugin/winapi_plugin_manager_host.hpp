/**
 * @file winapi_plugin_manager_host.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <plugin_manager_host.hpp>

class WinApiPlugin;

class WinApiPluginManager final : public PluginManager
{
public:
    std::shared_ptr<WinApiPlugin> GetPlugin(const std::string &path, const std::string &exportFactoryName)
    {
        std::intptr_t id = PluginManager::AddPlugin(path, exportFactoryName, *this);
        return std::static_pointer_cast<WinApiPlugin>(mPluginMap[id].mPluginPtr);
    }

// protected:
    WinApiPluginManager() {}
    ~WinApiPluginManager() {}

private:

};