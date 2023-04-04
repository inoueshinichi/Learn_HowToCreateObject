#include <mac_plugin.hpp>

#include <string>
#include <sstream>
#include <iostream>

void MacPlugin::About() const 
{ 
    std::intptr_t id = reinterpret_cast<std::intptr_t>((void *)this);
    std::string pluginFilePath = mManager.GetFilePath(id);
    std::ostringstream oss;
    oss << "----- About: " << PluginName() << " -----" << std::endl;
    oss << "[Author]: " << PLUGIN_AUTHOR << std::endl;
    oss << "[Email]: " << PLUGIN_EMAIL << std::endl; 
    oss << "[PluginName]: " << PluginName() << std::endl;
    oss << "[Version]: " << PLUGIN_VERSION << std::endl;
    oss << "[File]: " << pluginFilePath << std::endl;
    oss << "[CompiledDatetime]: " << CompiledDatetime() << std::endl;
    oss << "[CompiledTime]: " << CompiledTime() << std::endl;
    oss << "[Architecture]: " << PLUGIN_ARCHITECTURE << std::endl;
    oss << "[Platform]: " << PLUGIN_PLATFORM << std::endl;
    oss << "[PlatformVersion]: " << PLUGIN_PLATFORM_VERSION << std::endl;
    oss << "[Generator]: " << PLUGIN_GENERATOR << std::endl;
    oss << "[Void-Length]: " << PLUGIN_ARCHITECTURE_ADDRESS_VOID_LENGTH << std::endl;
    std::cout << oss.str();
}