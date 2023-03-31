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

#include <plugin_host.hpp>

class DllInfo;

struct PluginInfo
{
    std::shared_ptr<Plugin> mPluginPtr; // この変数でプラグインのスマートポインタの生存期間を縛る.
    DllInfo *mDllInfo;                  // 所属するDllファイル情報へのポインタ
    std::string mDllFilePath;
    std::string mCompiledDatetime;
    std::string mCompiledTime;
    int mMajorVersion;
    int mMinorVersion;
    int mPatchVersion;
};

struct DllInfo
{
    std::intptr_t mHandle;
    std::vector<PluginInfo*> mPluginInfos; // Dllを参照しているPluginへのポインタ
};

/**
 * @brief PluginManagerは抽象インターフェース
 * @note 派生クラスはシングルトン
 *
 */
class PluginManagerBase
{
public:
    using PluginMap = std::map<std::intptr_t, PluginInfo>;
    using DllMap = std::unordered_map<std::string, DllInfo>;

    static bool LoadDll(const std::string &path);
    static void UnloadDll(const std::string &path);
    static void UnloadDllAll();

// protected:
    PluginManagerBase() {}
    virtual ~PluginManagerBase() { /*ClearPlugins();*/ }

protected:
    friend class Plugin;
    PluginMap mPluginMap;

    template <typename PLUGIN, typename PLUGIN_MANAGER>
    std::intptr_t AddPlugin(const std::string &path, const std::string &exportFactoryName, std::shared_ptr<PLUGIN> &pluginPtr, PLUGIN_MANAGER &manager);
    
    void RemovePlugin(std::intptr_t id);

    void ClearPlugins();

private:
    static DllMap &GetDllMap();
    static std::mutex &GetMutex();

    // static std::recursive_mutex &GetRecMutex();
    // static void AddRef(const std::string &keyPath, DllMap &dllMap);
    // static bool Release(const std::string &keyPath, DllMap &dllMap);
};

template <typename PLUGIN, typename PLUGIN_MANAGER>
std::intptr_t 
PluginManagerBase::AddPlugin(const std::string &path, const std::string &exportFactoryName, 
    std::shared_ptr<PLUGIN> &pluginPtr, PLUGIN_MANAGER &manager)
{
    std::intptr_t handle;
    handle = PluginManagerBase::LoadDll(path); // Dllファイルは, 確保 or 再利用
    if (handle == NULL)
    {
        return NULL;
    }

    // ファクトリ関数
    using CreatorPtr = std::shared_ptr<PLUGIN> (*)(PLUGIN_MANAGER &);
    CreatorPtr creator;

#if defined(_MSC_VER)

    // プラグインオブジェクトのファクトリ関数のポインタを取得
    creator = (CreatorPtr)::GetProcAddress((HMODULE)handle, _T(exportFactoryName.c_str()));
    if (creator == NULL)
    {
        return NULL;
    }

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

    // std::shared_ptr<PLUGIN> pluginPtr;
    pluginPtr = creator(manager); // New Plugin Instance
    /**
     * @note この時点でプラグインインスタンスはDLLファイルから切り離されている
     */

    auto &dllMap = GetDllMap();
    
    PluginInfo info;
    info.mPluginPtr = std::static_pointer_cast<Plugin>(pluginPtr); // Pluginスマートポインタを外部に公開しても生存期間は, このポインタが管理する.
    info.mDllInfo = &dllMap[path]; // 所属先のDllInfoへのポインタ
    info.mDllFilePath = path;
    info.mCompiledDatetime = pluginPtr->CompiledDatetime();
    info.mCompiledTime = pluginPtr->CompiledTime();
    info.mMajorVersion = pluginPtr->MajorVersion();
    info.mMinorVersion = pluginPtr->MinorVersion();
    info.mPatchVersion = pluginPtr->PatchVersion();

    std::intptr_t id = reinterpret_cast<std::intptr_t>((void *)pluginPtr.get()); // 生ポインタ(アドレス)をIDとする
    mPluginMap[id] = info; // Register

    dllMap[path].mPluginInfos.push_back(&mPluginMap[id]); // 所属するPluginInfoへのポインタ

    return id;
}