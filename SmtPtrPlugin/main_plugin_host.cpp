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

auto main(int, char**) -> int
{
    try
    {
        MacPluginManager macPluginManager;
        auto macPlugin = macPluginManager.GetPlugin("./MacDllPlugin/libmac_plugind.so", "CreateMacPlugin");
        if (macPlugin)
        {
            macPlugin->About();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}