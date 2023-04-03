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

#include <derived_plugin_manager_host.hpp>

int main(int, char**)
{
    try
    {
        MacPluginManager macPluginManager;
        auto macPlugin = macPluginManager.GetPlugin("path/to", "CreateMacPlugin");
        // std::intptr_t id;
        // auto macPlugin = macPluginManager.AddPlugin(id, "path/to", "CreateMacPlugin", macPluginManager);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}