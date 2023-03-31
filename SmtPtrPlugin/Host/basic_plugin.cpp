#include <basic_plugin.hpp>

#include <iostream>
#include <sstream>
#include <string>

const char *BasicPlugin::PluginName() const
{
    return "BasicPlugin";
}

int BasicPlugin::MajorVersion()
{
    return 0;
}

int BasicPlugin::MinorVersion()
{
    return 1;
}

int BasicPlugin::PatchVersion()
{
    return 0;
}

const char *BasicPlugin::CompiledDatetime()
{
    return "Host Compliled Datetime";
}

const char *BasicPlugin::CompiledTime()
{
    return "Host Compiled Time";
}


void BasicPlugin::About()
{
    std::cout << "About BasicPlugin" << std::endl;
}