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

#include <plugin_manager_base_host.hpp>

#include <plugin_tag.hpp>

template <typename PLUGIN>
class PluginManager final : public PluginManagerBase
{
public:
    std::shared_ptr<PLUGIN> GetPlugin(const std::string &path, const std::string &exportFactoryName)
    {
        std::intptr_t id;
        std::shared_ptr<Plugin> pluginPtr = PluginManagerBase::AddPlugin(id, path, exportFactoryName, *this);
        if (id == NULL)
        {
            std::ostringstream oss;
            oss << "No plugin creator: "
                << "std::shared_ptr<" << PluginTag<PLUGIN>::str << "> "
                << exportFactoryName << "(PluginManager<" << PluginTag<PLUGIN>::str ">&) in " << path;
            throw std::runtime_error(oss.str());
        }

        if (!std::dynamic_pointer_cast<PLUGIN>(pluginPtr))
        {
            std::ostringstream oss;
            oss << "Can't dynamic_pointer_cast<" << PluginTag<PLUGIN>::str << ">(std::shared_ptr<Plugin>)";
            throw std::runtime_error(oss.str());
        }

        return std::dynamic_pointer_cast<PLUGIN>(pluginPtr);
    }

    void ErasePlugin(std::shared_ptr<PLUGIN> &plugin)
    {
        std::intptr_t id = GetIdFromPlugin(std::static_pointer_cast<Plugin>(plugin));
        if (!id)
        {
            return;
        }
        RemovePlugin(id);
    }

// protected:
    PluginManager() { std::cout << "Constructor PluginManager<" << PluginTag<PLUGIN>::str << ">" << std::endl; }
    ~PluginManager() { std::cout << "Destructor ~PluginManager<" << PluginTag<PLUGIN>::str << ">" << std::endl;
        
    }

private:
};