/**
 * @file main_plugin_host.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <exception>
#include <memory>

#include <plugin_manager_host.hpp>
#include <winapi_plugin_manager_host.hpp>

int main(int, char**)
{
    try
    {
       auto pluginManager = std::make_shared<PluginManager>();
       auto winapiPluginManager = std::make_shared<WinApiPluginManager>();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}