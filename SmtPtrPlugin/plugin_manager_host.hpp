/**
 * @file plugin_manager_host.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include <functional>
#include <utility> // std::pair
#include <mutex>

#include <exception>
#include <stdexcept>

#include <plugin_host.hpp>

struct PluginInfo
{
    std::shared_ptr<Plugin> mPluginPtr; // この変数でプラグインのスマートポインタの生存期間を縛る.
    std::intptr_t mHandle;
    std::string mDllPath;
    std::string mCompileDatetime;
    std::string mCompileTime;
    int mMajorVersion;
    int mMinorVersion;
    int mPatchVersion;
};

/**
 * @brief PluginManagerは抽象インターフェース
 * @note 派生クラスはシングルトン
 *
 */
class PluginManager
{
public:
    using PluginMap = std::map<size_t, PluginInfo>;

protected:
    PluginMap mPluginMap;

    PluginManager();
    virtual ~PluginManager();

    template <typename PLUGIN_MANAGER>
    bool AddPlugin(size_t id, const std::string &path, const std::string &exportFactoryName, PLUGIN_MANAGER &manager);
    void RemovePlugin(size_t id);
    void ClearPlugins();

    static std::intptr_t(const std::string &path);
    static bool UnloadDll(cLoadDllonst std::string &path);
    static void UnloadDllAll();
    static void ReloadDll(const std::string &path);

private:
    using DllHandle = std::pair<std::intptr_t, int>;
    using DllMap = std::unordered_map<std::string, DllHandle>;

    static DllMap &GetDllMap();
    static std::recursive_mutex &GetRecMutex();

    static void AddRef(const std::string &keyPath, DllMap &dllMap);
    static bool Release(const std::string &keyPath, DllMap &dllMap);
};

void PluginManager::RemovePlugin(size_t id)
{
    auto iter = std::find(mPluginMap.begin(), mPluginMap.end(), id);
    if (iter != mPluginMap.end())
    {
        mPluginMap.erase(iter);
    }
}

template <typename PLUGIN_MANAGER>
bool PluginManager::AddPlugin(size_t id, const std::string &path, const std::string &exportFactoryName, PLUGIN_MANAGER &manager)
{
    std::intptr_t handle;
    handle = PluginManager::LoadDll(path);
    if (handle == NULL)
    {
        return false;
    }

    // ファクトリ関数
    std::function<std::shared_ptr<Plugin>(PLUGIN_MANAGER &)> creator;

#if defined(_MSC_VER)

    // プラグインオブジェクトのファクトリ関数のポインタを取得
    creator = ::GetProcAddress((HMODULE)handle, TEXT(exportFactoryName.c_str()));
    if (creator == NULL)
    {
        return false;
    }

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

    std::shared_ptr<Plugin> pluginPtr;
    pluginPtr = creator(manager); // New

    PluginInfo info;
    info.mPluginPtr = pluginPtr;
    info.mHandle = handle;
    info.mDllPath = path;
    info.mCompiledDatatime = plugin->CompiledDatatime();
    info.mCompiledTime = plugin->CompiledTime();
    info.mMajorVersion = plugin->MajorVersion();
    info.mMinorVersion = plugin->MinorVersion();
    info.mPatchVersion = plugin->PatchVersion();

    mPluginMap[id] = info; // Register

    return true;
}