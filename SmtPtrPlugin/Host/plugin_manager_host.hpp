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
#include <iostream>

#include <plugin_host.hpp>
#include <plugin_tag.hpp>

struct DllInfo;

struct PluginInfo
{
    std::shared_ptr<Plugin> mPluginPtr; // この変数でプラグインのスマートポインタの生存期間を縛る.
    DllInfo *mDllInfo;                  // 所属するDllファイル情報へのポインタ
    std::string mDllFilePath;
    std::string mPluginName;
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
 * @brief PluginManager
 * @note  シングルトン派生クラスの基底クラス
 *
 */
class PluginManager
{
public:
    using PluginMap = std::map<std::intptr_t, PluginInfo>;
    using DllMap = std::unordered_map<std::string, DllInfo>;

    // protected:
    PluginManager() { std::cout << "Constructor PluginManager" << std::endl; }
    virtual ~PluginManager() { std::cout << "Destructor ~PluginManager" << std::endl; }

    static bool LoadDll(const std::string &path);   // inline
    static void UnloadDll(const std::string &path); // inline
    static void UnloadDllAll();                     // inline

    std::shared_ptr<Plugin> GetPlugin(const std::string &path, const std::string &exportFactoryName)
    {
        return this->GetPluginImpl(path, exportFactoryName);
    }

    void ClearPlugins();
    void ErasePlugin(std::shared_ptr<Plugin> &pluginPtr);

protected:
    friend class Plugin;
    PluginMap mPluginMap;

    template <typename PLUGIN_MANAGER>
    std::shared_ptr<Plugin> AddPlugin(std::intptr_t &id,
                                      const std::string &path,
                                      const std::string &exportFactoryName,
                                      PLUGIN_MANAGER &manager);

    virtual std::shared_ptr<Plugin> GetPluginImpl(const std::string &path, 
                                                  const std::string &exportFactoryName) = 0;

    void RemovePlugin(std::intptr_t id);

    std::intptr_t GetIdFromPlugin(std::shared_ptr<Plugin> plugin)
    {
        if (plugin)
        {
            return reinterpret_cast<std::intptr_t>((void *)plugin.get()); // 生ポインタ(アドレス)をIDとする
        }
        return (std::intptr_t)NULL;
    }

    static DllMap*& GetDllMap();
    static std::mutex &GetMutex();

private:
    

    // static std::recursive_mutex &GetRecMutex();
    // static void AddRef(const std::string &keyPath, DllMap &dllMap);
    // static bool Release(const std::string &keyPath, DllMap &dllMap);
};

template <typename PLUGIN_MANAGER>
std::shared_ptr<Plugin> PluginManager::AddPlugin(std::intptr_t &id,
                                                 const std::string &path,
                                                 const std::string &exportFactoryName,
                                                 PLUGIN_MANAGER &manager)
{
    std::intptr_t handle;
    handle = this->LoadDll(path); // Dllファイルは, 確保 or 再利用
    if (handle == (std::intptr_t)NULL)
    {
        return std::shared_ptr<Plugin>(); // nullptr
    }

    // ファクトリ関数
    using CreatorPtr = std::shared_ptr<Plugin> (*)(PLUGIN_MANAGER &);
    CreatorPtr creator = nullptr;

#if defined(_MSC_VER)

    // プラグインオブジェクトのファクトリ関数のポインタを取得
    creator = (CreatorPtr)::GetProcAddress((HMODULE)handle, exportFactoryName.c_str());

    if (creator == NULL)
    {
        id = NULL;
        return std::shared_ptr<Plugin>(); // nullptr
    }

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

    std::shared_ptr<Plugin> pluginPtr;
    pluginPtr = creator(manager); // New Plugin Instance
    /**
     * @note この時点でプラグインインスタンスはDLLファイルから切り離されている
     */

    auto &dllMap = this->GetDllMap();

    PluginInfo info;
    info.mPluginPtr = pluginPtr;   // Pluginスマートポインタを外部に公開しても生存期間は, このポインタが管理する.
    info.mDllInfo = &(*dllMap)[path]; // 所属先のDllInfoへのポインタ
    info.mDllFilePath = path;
    info.mPluginName = pluginPtr->PluginName();
    info.mCompiledDatetime = pluginPtr->CompiledDatetime();
    info.mCompiledTime = pluginPtr->CompiledTime();
    info.mMajorVersion = pluginPtr->MajorVersion();
    info.mMinorVersion = pluginPtr->MinorVersion();
    info.mPatchVersion = pluginPtr->PatchVersion();

    id = this->GetIdFromPlugin(info.mPluginPtr); // 生ポインタ(アドレス)をIDとする
    mPluginMap[id] = info;                                                  // Register

    (*dllMap)[path].mPluginInfos.push_back(&mPluginMap[id]); // 所属するPluginInfoへのポインタ

    return pluginPtr;
}