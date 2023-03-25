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

#include <plugin_manager_host.hpp>

class BasicPluginManager final : public PluginManager
{
public:
    

    bool Load(const std::string &path) override final;
    void Unload(const std::string &path) override final;
    void UnloadAll() override final;

protected:
    BasicPluginManager();
    ~BasicPluginManager();

private:
    static unsigned int sNextID;
};