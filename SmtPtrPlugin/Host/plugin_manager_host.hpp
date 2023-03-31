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

#include <plugin_manager_base_host.hpp>

#define STRMACRO(name) #name


class PluginManager final : public PluginManagerBase
{
public:
    template <typename PLUGIN>
    std::shared_ptr<PLUGIN> GetPlugin(const std::string &path, const std::string &exportFactoryName)
    {
        std::shared_ptr<PLUGIN> pluginPtr;
        std::intptr_t id = PluginManagerBase::AddPlugin(path, exportFactoryName, pluginPtr, *this);
        if (id == NULL)
        {
            std::ostringstream oss;
            oss << "No plugin creator: "
                << "std::shared_ptr<" << STRMACRO(PLUGIN) << "> "
                << exportFactoryName << "(PluginManager&) in " << path;
            throw std::runtime_error(oss.str());
        }

        return pluginPtr;
    }

// protected:
    PluginManager() {}
    ~PluginManager() {}

private:
};