#include <win_dll_plugin.hpp>

std::shared_ptr<WinApiPlugin> CreateWinApiPlugin(PluginManager<WinApi>& manager)
{
    return std::make_shared<WinApiPlugin>(manager);
}
