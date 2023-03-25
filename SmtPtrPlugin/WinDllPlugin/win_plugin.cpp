#include <win_plugin.hpp>


std::shared_ptr<Plugin> CreatePlugin(BasicPluginManager& manager)
{
    return std::make_shared<BasicPlugin>(manager);
}

std::shared_ptr<Plugin> CreateWinApiPlugin(WinApiPluginManager& manager)
{
    return std::make_shared<WinApiPlugin>(manager);
}
