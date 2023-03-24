#include <win_plugin.hpp>


std::shared_ptr<IPlugin> CreatePlugin(std::shared_ptr<PluginManager> manager)
{
    return std::make_shared<BasicPlugin>(manager);
}

std::shared_ptr<IPlugin> CreateWinApiPlugin(std::shared_ptr<WinApiPluginManager> manager)
{
    return std::make_shared<WinApiPlugin>(manager);
}
