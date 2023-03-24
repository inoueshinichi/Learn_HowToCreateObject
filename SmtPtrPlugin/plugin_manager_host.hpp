/**
 * @file plugin_manager_host.hpp
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
// #include <utility> // std::pair
#include <unordered_map>

class Plugin;

struct PluginInfo
{
    std::shared_ptr<Plugin> mPluginPtr;
    unsigned int mID;
    std::intptr_t mHandle;
};

/**
 * @brief PluginManagerはstd::shared_ptrでメモリ確保すること.
 * 
 */
class PluginManager : public std::enable_shared_from_this<PluginManager>
{
public:
    PluginManager() = default;
    ~PluginManager() = default;
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;
    PluginManager(PluginManager&& rhs);
    PluginManager& operator=(PluginManager& rhs);

    using PluginMap = std::unordered_map<std::string, PluginInfo>;

private:
    PluginMap mPluginMap;
};