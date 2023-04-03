#include <win_dll_plugin.hpp>
#include <winapi_plugin.hpp>
#include <winmfc_plugin.hpp>

// Host
#include <derived_plugin_manager_host.hpp>

// WinApi Plugin
PLUGIN_API std::shared_ptr<Plugin> CreateWinApiPlugin(WinApiPluginManager &manager)
{
    return std::make_shared<WinApiPlugin>(manager);
}

// WinMFC Plugin
PLUGIN_API std::shared_ptr<Plugin> CreateWinMfcPlugin(WinMfcPluginManager &manager)
{
    return std::make_shared<WinMfcPlugin>(manager);
}