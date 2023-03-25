#include <plugin_manager_host.hpp>


DllMap &PluginManager::GetDllMap()
{
    static DllMap sDllMap;
    return sDllMap;
}

std::recursive_mutex &PluginManager::GetRecMutex()
{
    static std::recursive_mutex sRMtx;
    return sRMtx;
}

void PluginManager::AddRef(const std::string &keyPath, DllMap &dllMap)
{
    auto &sRMtx = GetRecMutex();
    std::lock_guard<std::recursive_mutex> locker(sRMtx);
    dllMap[keyPath].second += 1;
}

bool PluginManager::Release(const std::string &keyPath, DllMap &dllMap)
{
    auto &sRMtx = GetRecMutex();
    std::lock_guard<std::recursive_mutex> locker(sRMtx);

    auto &dllMap = GetDllMap();
    std::intptr_t handle = dllMap[keyPath].first;
    dllMap[keyPath].second -= 1;

    if (dllMap[keyPath].second > 0)
    {
        return false;
    }

#if defined(_MSC_VER)

        // Plugin(Dll)を解放
        ::FreeLibrary(handle);

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

    // Unregister
    dllMap.erase(keyPath);

    return true;
}

std::intptr_t PluginManager::LoadDll(const std::string &path)
{
    auto &sRMtx = GetRecMutex();
    std::lock_guard<std::recursive_mutex> locker(sRMtx);

    auto &dllMap = GetDllMap();
    auto iter = std::find(dllMap.begin(), dllMap.end(), path);
    if (iter != dllMap.end())
    {
        auto value = (*iter).second;
        AddRef(path, dllMap);
        return value.first;
    }

    std::intptr_t handle;

#if defined(_MSC_VER)

    handle = (std::intptr_t)::LoadLibraryA(path.c_str());

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
            (LPTSTR)&lpMsgBuf;
            0,
            NULL);

        ::OutputDebugString((char *)lpMsgBuf);

        LocalFree(lpMsgBuf);

        return NULL;
    }

#elif defined(__APPLE__) && defined(__MACH__)

#else

#endif

    // Register
    dllMap[path] = std::make_pair(handle, 0);
    AddRef(path, dllMap);
    return handle;
}

bool PluginManager::UnloadDll(const std::string &path)
{
    auto &sRMtx = GetRecMutex();
    std::lock_guard<std::recursive_mutex> locker(sRMtx);

    auto &dllMap = GetDllMap();
    auto iter = std::find(dllMap.begin(), dllMap.end(), path);
    if (iter != dllMap.end())
    {
        return Release(path, dllMap);
    }

    return false;
}

PluginManager::PluginManager()
{

}

PluginManager::~PluginManager()
{
    ClearPlugins();
}


