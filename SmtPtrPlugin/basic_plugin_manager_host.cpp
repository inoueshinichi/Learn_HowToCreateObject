#include <basic_plugin_manager_host.hpp>

#include <algorithm>


bool BasicPluginManager::Load(const std::string &path)
{
    Unload(path);

    auto creator = [this](std::intptr_t handle, std::shared_ptr<Plugin> pluginPtr) -> bool
    {
        
        auto func = ::GetProcAddress((HMODULE)handle, TEXT("CreateBasicPlugin"));
        if (func == NULL)
        {
            return false;
        }

        // プラグインオブジェクトの生成
        pluginPtr = func(*this);
        
        return true;
    };

    return PluginManager::AddPlugin(path, sNextID++, creator);
}


void BasicPluginManager::Unload(const std::string &path)
{

}

void BasicPluginManager::UnloadAll()
{
    auto iter = mPluginMap.begin();
    for (; iter != mPluginMap.end(); ++iter)
    {
#if defined(_MSC_VER)
        ::FreeLibrary((*iter).second.mHandle);
#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif
    }

    mPluginMap.clear()
}