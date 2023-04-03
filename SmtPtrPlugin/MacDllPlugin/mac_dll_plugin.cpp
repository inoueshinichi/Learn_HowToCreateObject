#include <mac_dll_plugin.hpp>
#include <mac_plugin.hpp>

// Host
#include <derived_plugin_manager_host.hpp>

// Mac Plugin
PLUGIN_API std::shared_ptr<Plugin> CreateMacPlugin(MacPluginManager &manager)
{
    return std::make_shared<MacPlugin>(manager);
}