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
#if defined(_MSC_VER) // Windows

        // nothing

#elif defined(__APPLE__) && defined(__MACH__) // macOS

        MacPluginManager macPluginManager;
        auto macPlugin = macPluginManager.GetPlugin("./MacDllPlugin/libmac_plugind.so", "CreateMacPlugin");
        if (macPlugin)
        {
            macPlugin->About();
        }

#else // Linux

        // nothing

#endif
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}