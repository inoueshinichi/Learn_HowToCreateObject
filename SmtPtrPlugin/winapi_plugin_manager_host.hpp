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

class WinApiPluginManager final : public PluginManager
{
public:
   

    
protected:
    WinApiPluginManager() {}
    ~WinApiPluginManager() {}

private:
    static unsigned int sNextID;
};