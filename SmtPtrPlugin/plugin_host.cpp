#include <plugin_host.hpp>
#include <plugin_manager_host.hpp>

int Plugin::MajorVersion() const
{
    std::intptr_t id = static_cast<std::intptr_t>(this);
    return mManager.mPluginMap[id].mMajorVersion;
}

int Plugin::MinorVersion() const
{
    std::intptr_t id = static_cast<std::intptr_t>(this);
    return mManager.mPluginMap[id].mMinorVersion;
}

int Plugin::PatchVersion() const
{
    std::intptr_t id = static_cast<std::intptr_t>(this);
    return mManager.mPluginMap[id].mPatchVersion;
}

const char *Plugin::CompiledDatetime() const
{
    std::intptr_t id = static_cast<std::intptr_t>(this);
    return mManager.mPluginMap[id].mCompileDatetime.c_str();
}

const char *Plugin::CompiledTime() const
{
    std::intptr_t id = static_cast<std::intptr_t>(this);
    return mManager.mPluginMap[id].mCompileTime.c_str();
}

const char *Plugin::DllFilePath() const
{
    std::intptr_t id = static_cast<std::intptr_t>(this);
    return mManager.mPluginMap[id].mDllFilePath.c_str();
}

void Plugin::About() const
{

}