#include <plugin_manager_host.hpp>

#include <algorithm>

// std::recursive_mutex &PluginManager::GetRecMutex()
// {
//     static std::recursive_mutex sRMtx;
//     return sRMtx;
// }

// void PluginManager::AddRef(const std::string &keyPath, DllMap &dllMap)
// {
//     auto &sRMtx = GetRecMutex();
//     std::lock_guard<std::recursive_mutex> locker(sRMtx);
//     dllMap[keyPath].mRefCount += 1;
// }

// bool PluginManager::Release(const std::string &keyPath, DllMap &dllMap)
// {
//     auto &sRMtx = GetRecMutex();
//     std::lock_guard<std::recursive_mutex> locker(sRMtx);

//     auto &dllMap = GetDllMap();
//     std::intptr_t handle = dllMap[keyPath].mHandle;
//     dllMap[keyPath].mRefCount -= 1;

//     if (dllMap[keyPath].mRefCount > 0)
//     {
//         return false;
//     }

// #if defined(_MSC_VER)

//         // Plugin(Dll)を解放
//         ::FreeLibrary(handle);

// #elif defined(__APPLE__) && defined(__MACH__)

// #else

// #endif

//     // Unregister
//     dllMap.erase(keyPath);

//     return true;
// }

// static
PluginManager::DllMap *&PluginManager::GetDllMap()
{
    static PluginManager::DllMap* sDllMap = new PluginManager::DllMap();
    return sDllMap;
}

// static
std::mutex &PluginManager::GetMutex()
{
    static std::mutex sMtx;
    return sMtx;
}

// static
bool PluginManager::LoadDll(const std::string &path)
{
    auto &sMtx = GetMutex();
    std::lock_guard<std::mutex> locker(sMtx);

    auto &dllMap = GetDllMap();
    auto iter = std::find(dllMap->begin(), dllMap->end(), path);
    if (iter != dllMap->end())
    {
        // AddRef(path, dllMap);
        // return (*iter).second.mHandle;
        return true;
    }

    std::intptr_t handle;

#if defined(_MSC_VER)

    handle = (std::intptr_t)::LoadLibrary(path.c_str());

    if (handle == NULL)
    {
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

        ::OutputDebugString((char *)lpMsgBuf);

        LocalFree(lpMsgBuf);

        // return NULL;
        return false;
    }

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

    // Register
    DllInfo info;
    info.mHandle = handle;
    // info.RefCount = 0;
    (*dllMap)[path] = info;
    // AddRef(path, dllMap);
    // return handle;
    return true;
}

// static
void PluginManager::UnloadDll(const std::string &path)
{
    auto &sMtx = GetMutex();
    std::lock_guard<std::mutex> locker(sMtx);

    auto &dllMap = GetDllMap();
    auto iter = std::find(dllMap->begin(), dllMap->end(), path);
    if (iter != dllMap->end())
    {
        // return Release(path, dllMap);
        for (size_t i = 0; i < (*iter).second.mPluginInfos.size(); ++i)
        {
            (*iter).second.mPluginInfos[i]->mDllInfo = nullptr; // Plugin(Info)とDllの関連を切る
        }
        (*iter).second.mPluginInfos.clear();

        std::intptr_t handle = (*iter).second.mHandle;

#if defined(_MSC_VER)

        // Plugin(Dll)を解放
        ::FreeLibrary((HMODULE)handle);

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

        (*iter).second.mHandle = NULL;
        dllMap->erase(iter);
    }
}

// static
void PluginManager::UnloadDllAll()
{
    auto &dllMap = GetDllMap();
    std::unordered_set<std::string> dllFilePaths;
    for (auto &pair : *dllMap)
    {
        dllFilePaths.insert(pair.first);
    }

    for (auto &path : dllFilePaths)
    {
        UnloadDll(path);
    }
}

void PluginManager::RemovePlugin(std::intptr_t id)
{
    auto iter = std::find(mPluginMap.begin(), mPluginMap.end(), id);
    if (iter != mPluginMap.end())
    {
        PluginInfo &info = (*iter).second;
        if (info.mDllInfo) // Dllファイルがまだ有効
        {
            auto iter_p_info = std::find(info.mDllInfo->mPluginInfos.begin(), info.mDllInfo->mPluginInfos.end(), &info);
            if (iter_p_info != info.mDllInfo->mPluginInfos.end())
            {
                std::iter_swap(iter_p_info, info.mDllInfo->mPluginInfos.end());
                info.mDllInfo->mPluginInfos.pop_back(); // 削除
            }
        }

        // 関連するPluginがないDllファイルは解放
        if (info.mDllInfo->mPluginInfos.size() == 0)
        {
            UnloadDll(info.mDllFilePath);
        }

        mPluginMap.erase(iter);
    }
}

void PluginManager::ClearPlugins()
{
    std::vector<std::intptr_t> ids;
    ids.reserve(mPluginMap.size());
    for (auto &pair : mPluginMap)
    {
        ids.push_back(pair.first);
    }

    for (auto &id : ids)
    {
        RemovePlugin(id);
    }

    mPluginMap.clear();
}

void PluginManager::ErasePlugin(std::shared_ptr<Plugin> &pluginPtr)
{
    std::intptr_t id = GetIdFromPlugin(pluginPtr);
    if (!id)
    {
        return;
    }
    RemovePlugin(id);
}