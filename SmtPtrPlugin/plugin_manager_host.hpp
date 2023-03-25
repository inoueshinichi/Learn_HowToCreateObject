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

#if defined(_MSC_VER)
#define NOMAXMIN // Workaround for windows max, min macro
#include <windows.h>
#include <atlstr.h> // CString (ATL)
#include <tchar.h>  // _TCHAR

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

#include <memory>
#include <string>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <functional>
// #include <utility> // std::pair
#include <mutex>
#include <vector>

// #include <exception>
// #include <stdexcept>

#include <plugin_host.hpp>

struct PluginInfo
{
    std::shared_ptr<Plugin> mPluginPtr; // この変数でプラグインのスマートポインタの生存期間を縛る.
    DllInfo *mDllInfo;                  // 所属するDllファイル情報へのポインタ
    std::string mDllFilePath;
    std::string mCompileDatetime;
    std::string mCompileTime;
    int mMajorVersion;
    int mMinorVersion;
    int mPatchVersion;
};

struct DllInfo
{
    std::intptr_t mHandle;
    // int mRefCount; // Pluginから参照されている数
    std::vector<PluginInfo*> mPluginInfos; // Dllを参照しているPluginへのポインタ
};

/**
 * @brief PluginManagerは抽象インターフェース
 * @note 派生クラスはシングルトン
 *
 */
class PluginManager
{
public:
    using PluginMap = std::map<std::intptr_t, PluginInfo>;
    using DllMap = std::unordered_map<std::string, DllInfo>;

    static std::intptr_t LoadDll(const std::string &path);
    static void UnloadDll(const std::string &path);
    static void UnloadDllAll();

protected:
    friend class Plugin;
    PluginMap mPluginMap;

    PluginManager() {}
    virtual ~PluginManager() { ClearPlugins(); }

    template <typename PLUGIN_MANAGER>
    std::intptr_t AddPlugin(const std::string &path, const std::string &exportFactoryName, PLUGIN_MANAGER &manager);
    void RemovePlugin(std::intptr_t id);
    void ClearPlugins();

private:
    static DllMap &GetDllMap();
    static std::mutex &GetMutex();
    // static std::recursive_mutex &GetRecMutex();

    // static void AddRef(const std::string &keyPath, DllMap &dllMap);
    // static bool Release(const std::string &keyPath, DllMap &dllMap);
};

void PluginManager::ClearPlugins()
{
    std::vector<std::intptr_t> ids;
    ids.reserve(mPluginMap.size());
    for (const auto& pair : mPluginMap)
    {
        ids.push_back(pair.first);
    }

    for (const auto& id : ids)
    {
        RemovePlugin(ids);
    }

    mPluginMap.clear();
}

void PluginManager::RemovePlugin(std::intptr_t id)
{
    auto iter = std::find(mPluginMap.begin(), mPluginMap.end(), id);
    if (iter != mPluginMap.end())
    {
        PluginInfo &info = (*iter).second;
        if (info.mDllInfo) // Dllファイルがまだ有効
        {
            std::iter_swap(&info, info.mDllInfo->mPluginInfos.end());
            info.mDllInfo->mPluginInfos.pop_back(); // 削除
        }

        // 関連するPluginがないDllファイルは解放
        if (info.mDllInfo->mPluginInfos.size() == 0)
        {
            UnloadDll(info.mDllFilePath);
        }

        mPluginMap.erase(iter);
    }
}

template <typename PLUGIN_MANAGER>
std::intptr_t PluginManager::AddPlugin(const std::string &path, const std::string &exportFactoryName, PLUGIN_MANAGER &manager)
{
    std::intptr_t handle;
    handle = PluginManager::LoadDll(path); // Dllファイルは, 確保 or 再利用
    if (handle == NULL)
    {
        return NULL;
    }

    // ファクトリ関数
    std::function<std::shared_ptr<Plugin>(PLUGIN_MANAGER &)> creator;

#if defined(_MSC_VER)

    // プラグインオブジェクトのファクトリ関数のポインタを取得
    creator = ::GetProcAddress((HMODULE)handle, _T(exportFactoryName.c_str()));
    if (creator == NULL)
    {
        return NULL;
    }

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

    std::shared_ptr<Plugin> pluginPtr;
    pluginPtr = creator(manager); // New Plugin Instance
    /**
     * @note この時点でプラグインインスタンスはDLLファイルから切り離されている
     */

    auto &dllMap = GetDllMap();
    
    PluginInfo info;
    info.mPluginPtr = pluginPtr;   // Pluginスマートポインタを外部に公開しても生存期間は, このポインタが管理する.
    info.mDllInfo = &dllMap[path]; // 所属先のDllInfoへのポインタ
    info.mDllFilePath = path;
    info.mCompiledDatatime = plugin->CompiledDatatime();
    info.mCompiledTime = plugin->CompiledTime();
    info.mMajorVersion = plugin->MajorVersion();
    info.mMinorVersion = plugin->MinorVersion();
    info.mPatchVersion = plugin->PatchVersion();

    std::intptr_t id = static_cast<std::intptr_t>(pluginPtr.get()); // 生ポインタ(アドレス)をIDとする
    mPluginMap[id] = info; // Register

    dllMap[path].mPluginInfos.push_back(&mPluginMap[id]); // 所属するPluginInfoへのポインタ

    return id;
}