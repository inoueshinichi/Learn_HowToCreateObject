#include <plugin_manager_host.hpp>



#include <algorithm>
#include <sstream>
#include <iostream>

// static public
PluginManager::DllMap &PluginManager::GetDllMap()
{
    static PluginManager::DllMap sDllMap;
    return sDllMap;
}

// static public
std::mutex &PluginManager::GetMutex()
{
    static std::mutex sMtx;
    return sMtx;
}

// static public
std::intptr_t PluginManager::LoadDll(const std::string &path)
{
    auto &sMtx = GetMutex();
    std::lock_guard<std::mutex> locker(sMtx);

    auto &dllMap = GetDllMap();
    auto iter = dllMap.find(path);
    if (iter != dllMap.end())
    {
        return dllMap[path].mHandle;
    }

    std::intptr_t handle;
    std::ostringstream oss;

#if defined(_MSC_VER)

    handle = (std::intptr_t)::LoadLibraryA(path.c_str());

    if (!handle)
    {
        oss << "Failed to load plugin(*.dll) on Windows: " << path << std::endl;
        oss << "OS error message: ";

        LPVOID lpMsgBuf;
        ::FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPASTR)&lpMsgBuf,
            0,
            NULL);

        if (lpMsgBuf)
        {
            oss <<  << (char *)lpMsgBuf;
        }
        
#if defined(_UNICODE) || defined(UNICODE)
        auto utf16Str = is::CvtShiftJisToUtf16(oss.str());
        std::wcerr << utf16Str << std::wendl;
#else
        std::cerr << oss.str() << std::endl;
#endif

        LocalFree(lpMsgBuf);

        return (std::intptr_t)NULL;
    }

// #elif defined(__APPLE__) && defined(__MACH__)
#else // Unix

    handle = (std::intptr_t)dlopen(path.c_str(), RTLD_LAZY /*| RTLD_LOCAL*/);
    if (!handle)
    {
        oss << "Failed to load plugin(*.so) on macOS: " << path << std::endl;
        oss << "OS error message: ";
        char *errorMsg = dlerror();
        
        if (errorMsg)
        {
            oss << errorMsg;
        }

        std::cerr << oss.str() << std::endl;

        dlclose((void *)handle);

        return (std::intptr_t)NULL;
    }

#endif

#if defined(_MSC_VER) && (defined(_UNICODE) || defined(UNICODE))
    auto pathL = is::CvtShiftJisToUtf16(path);
    std::wcout << L"Load plugin file: " << pathL << std::wendl;
#else
    std::cout << "Load plugin file: " << path << std::endl;
#endif

    // Register
    DllInfo info;
    info.mHandle = handle;
    dllMap.insert({path, info});
    return handle;
}

// static public
void PluginManager::UnloadDll(const std::string &path)
{
    auto &sMtx = GetMutex();
    std::lock_guard<std::mutex> locker(sMtx);

    auto &dllMap = GetDllMap();
    auto iter = dllMap.find(path);

    if (iter != dllMap.end())
    {
        for (size_t i = 0; i < (*iter).second.mPluginInfos.size(); ++i)
        {
            (*iter).second.mPluginInfos[i]->mDllInfo = nullptr; // Plugin(Info)とDllの関連を切る
        }
        (*iter).second.mPluginInfos.clear();

        std::intptr_t handle = (*iter).second.mHandle;

        // Plugin(Dll)を解放
#if defined(_MSC_VER)
        ::FreeLibrary((HMODULE)handle);
#else // Unix // #elif defined(__APPLE__) && defined(__MACH__)
        dlclose((void *)handle);
#endif
        (*iter).second.mHandle = NULL;
        dllMap.erase(iter);
    }
}

// static public
void PluginManager::UnloadDllAll()
{
    auto &dllMap = GetDllMap();
    std::unordered_set<std::string> dllFilePaths;
    for (auto &pair : dllMap)
    {
        dllFilePaths.insert(pair.first);
    }

    for (auto &path : dllFilePaths)
    {
        UnloadDll(path);
    }
}

// public
std::shared_ptr<Plugin> 
PluginManager::GetPlugin(const std::string &path, const std::string &exportFactoryName)
{
    // Dllハンドルの取得
    std::intptr_t handle;
    handle = PluginManager::LoadDll(path); // Dllファイルは, 新規or再利用.
    if (handle == (std::intptr_t)NULL)
    {
        return std::shared_ptr<Plugin>(nullptr);
    }
    std::printf("Valid handle: %p\n", (void*)handle);

    // プラグインの生成は派生クラスにおまかせ.
    std::shared_ptr<Plugin> pluginPtr = this->AddPluginImpl(handle, path, exportFactoryName);

    // 新規生成したプラグインに関する情報を格納.
    auto &dllMap = PluginManager::GetDllMap();
    PluginInfo info;
    info.mPluginPtr = pluginPtr;   // Pluginスマートポインタを外部に公開しても生存期間は, このポインタが管理する.
    info.mDllInfo = &dllMap[path]; // 所属先のDllInfoへのポインタ
    info.mDllFilePath = path;
    info.mPluginName = pluginPtr->PluginName();
    info.mCompiledDatetime = pluginPtr->CompiledDatetime();
    info.mCompiledTime = pluginPtr->CompiledTime();
    info.mMajorVersion = pluginPtr->MajorVersion();
    info.mMinorVersion = pluginPtr->MinorVersion();
    info.mPatchVersion = pluginPtr->PatchVersion();

    std::intptr_t id = GetPluginId(pluginPtr);
    mPluginMap.insert({id, info}); // Register

    dllMap[path].mPluginInfos.push_back(&mPluginMap[id]); // 所属するPluginInfoへのポインタ

    return pluginPtr;
}

// public
void PluginManager::ClearPlugins()
{
    std::vector<std::intptr_t> handles;
    handles.reserve(mPluginMap.size());
    for (auto &pair : mPluginMap)
    {
        handles.push_back(pair.first);
    }

    for (auto &handle : handles)
    {
        RemovePlugin(handle);
    }

    mPluginMap.clear(); // 保険
}

// public
void PluginManager::ErasePlugin(std::shared_ptr<Plugin> &plugin)
{
    std::intptr_t handle = GetPluginId(plugin);
    if (!handle)
    {
        return;
    }
    RemovePlugin(handle);
}

// public
std::intptr_t PluginManager::GetPluginId(std::shared_ptr<Plugin> plugin)
{
    if (plugin)
    {
        return reinterpret_cast<std::intptr_t>((void *)plugin.get()); // 生ポインタ(アドレス)をIDとする
    }
    return (std::intptr_t)NULL;
}

// protected
void PluginManager::RemovePlugin(std::intptr_t id)
{
    auto iter = mPluginMap.find(id);
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