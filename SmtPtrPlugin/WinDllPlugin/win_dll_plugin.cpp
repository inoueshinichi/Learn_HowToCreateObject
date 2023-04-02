#include <win_dll_plugin.hpp>

#include <winapi_plugin.hpp>

// WinApi Plugin
PLUGIN_API std::shared_ptr<Plugin> CreateWinApiPlugin(PluginManager<WinApiPlugin> &manager)
{
    return std::make_shared<WinApiPlugin>(manager);
}