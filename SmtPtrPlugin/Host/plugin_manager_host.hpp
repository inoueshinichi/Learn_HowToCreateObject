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
#else // Unix // #elif defined(__APPLE__) && defined(__MACH__)
#include <dlfcn.h>
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
#include <sstream>

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

// Operator == for Hash (std::unordered_map)
bool operator==(const DllInfo &lhs, const DllInfo &rhs)
{
    return (lhs.mHandle == rhs.mHandle) && (lhs.mPluginInfos == rhs.mPluginInfos);
}

// // Hash func for Hash (std::unordered_map)
// struct DllInfoHash
// {
//     std::size_t operator()(DllInfo dllInfo) const
//     {
//         std::size_t hs1 = std::hash<std::intptr_t>()(dllInfo.mHandle);
//         std::size_t hs2 = std::hash<std::vector<PluginInfo *>>()(dllInfo.mPluginInfos);
//         return hs1 ^ hs2;
//     }
// };

struct PluginMapHash
{
    std::size_t operator()(std::intptr_t key) const
    {
        std::size_t hs = std::hash<std::intptr_t>()(key);
        return hs;
    }
};

/**
 * @brief PluginManager
 * @note  シングルトン派生クラスの基底クラス
 *
 */
class PluginManager
{
public:
    using DllMap = std::unordered_map<std::string, DllInfo>; // static
    using PluginMap = std::map<std::intptr_t, PluginInfo>; // un-static(key: plugin-pointer)

    // protected:
    PluginManager() { std::cout << "Constructor PluginManager" << std::endl; }
    virtual ~PluginManager() { std::cout << "Destructor ~PluginManager" << std::endl; }

    static bool LoadDll(const std::string &path);   
    static void UnloadDll(const std::string &path); 
    static void UnloadDllAll();                     

    std::shared_ptr<Plugin> GetPlugin(const std::string &path, const std::string &exportFactoryName);

    void ErasePlugin(std::shared_ptr<Plugin> &pluginPtr);
    void ClearPlugins();
    std::intptr_t GetHandleFromPlugin(std::shared_ptr<Plugin> plugin);

protected:
    friend class Plugin;
    PluginMap mPluginMap;

    template <typename PLUGIN_MANAGER>
    std::shared_ptr<Plugin> 
    AddPlugin(std::intptr_t handle,
              const std::string &path,
              const std::string &exportFactoryName,
              PLUGIN_MANAGER &manager);

    virtual std::shared_ptr<Plugin> 
    AddPluginImpl(std::intptr_t handle,
                  const std::string &path, 
                  const std::string &exportFactoryName) = 0;

    void RemovePlugin(std::intptr_t handle);

    static DllMap*& GetDllMap(); // inline
    static std::mutex &GetMutex(); // inline

private:
};

// protected
template <typename PLUGIN_MANAGER>
std::shared_ptr<Plugin> 
PluginManager::AddPlugin(std::intptr_t handle,
                         const std::string &path,
                         const std::string &exportFactoryName,
                         PLUGIN_MANAGER &manager)
{
    // ファクトリ関数
    using CreatorPtr = std::shared_ptr<Plugin> (*)(PLUGIN_MANAGER &);
    CreatorPtr creator = nullptr;

    // プラグインオブジェクトのファクトリ関数のポインタを取得
#if defined(_MSC_VER)
    creator = (CreatorPtr)::GetProcAddress((HMODULE)handle, exportFactoryName.c_str());
#else // Unix // #elif defined(__APPLE__) && defined(__MACH__)
    creator = (CreatorPtr)dlsym((void *)handle, exportFactoryName.c_str());
#endif

    if (!creator)
    {
        std::ostringstream oss;
        oss << "No plugin creator func: " << exportFactoryName;
        oss << " in " << path << std::endl;
        oss << "OS error message: ";

#if defined (_MSC_VER)
        LPVOID lpMsgBuf;
        ::FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0,
            NULL);

        oss << (char *)lpMsgBuf;

        LocalFree(lpMsgBuf);
#else
        char* errorMsg = dlerror();
        if (errorMsg)
        {
            oss << errorMsg;
        }
#endif
        oss << std::endl;
        
        throw std::runtime_error(oss.str());
    }
    
    return creator(manager); // New Plugin Instance
    /**
     * @note この時点でプラグインインスタンスはDLLファイルから切り離される.
     */
}