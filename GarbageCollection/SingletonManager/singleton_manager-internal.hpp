/**
 * @file singleton_manager-internal.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

////////////////////////////////////
// C++ Demangle for Singleton Class
////////////////////////////////////
#include <typeinfo>
#include <iostream>

#if defined(_MSC_VER)
    #if !defined(_DEBUG) || defined(NDEBUG)
        #define SINGLETON_LOG(message, singleton)
    #else
        #define SINGLETON_LOG(message singleton)                  \
            std::cout << "[" << #message << "]: a singleton `" << \
            typeid(singleton).name() << "`" << std::endl;
    #endif
#else
    #if defined(NDEBUG)
        #define SINGLETON_LOG(message, singleton)
    #else
        // UNIX系だとlibstdc++に含まれる.
        // Windowsはvcruntime14X.dllかな...
        #if __has_include(<cxxabi.h>)
            #include <cxxabi.h>
            #define SINGLETON_LOG(message, singleton)                                                        \
            std::string singleton_name;                                                                      \
            const std::type_info &type_id = typeid(singleton); /* RTTI */                                    \
            int stat{-1};                                                                                    \
            char *name = abi::__cxa_demangle(type_id.name(), 0, 0, &stat);                                   \
            if (name != nullptr)                                                                             \
            {                                                                                                \
                if (stat == 0) /* success: stat == 0 */                                                      \
                {                                                                                            \
                    singleton_name = name;                                                                   \
                    std::cout << "[" << #message << "] a singleton `" << singleton_name << "`" << std::endl; \
                    ::free(name);                                                                            \
                }                                                                                            \
            }                                                                                                \
            if (stat != 0)                                                                                   \
            {                                                                                                \
                std::cout << "[" << #message << "] a singleton `" <<                                         \
                    typeid(SINGLETON).name() << "`" << std::endl;                                            \
            }
        #else
            #define SINGLETON_LOG(message, singleton)                 \
                std::cout << "[" << #message << "]: a singleton `" << \
                typeid(singleton).name() << "`" << std::endl;
        #endif
    #endif
#endif

#include <singleton_manager.hpp>

template <typename SINGLETON>
SINGLETON *SingletonManager::Get()
{
    // 内部リンケージのstatic変数は必ずdll側の*.cppで定義すること.
    // https://qiita.com/Chironian/items/3fb61cffa2a20dbee5c2
    std::mutex &mtx = SingletonManager::GetMutex();

    // クリティカルセクション作成
    std::lock_guard<std::mutex> locker(mtx);

    static SINGLETON *instance = nullptr; // 初回だけnullptrで初期化される
    if (instance)
    {
        return instance;
    }

    SingletonManager &self = SingletonManager::GetSelf();
    instance = new SINGLETON();
    SINGLETON_LOG(Create, SINGLETON)

    auto deleter = []() -> void
    {
        SINGLETON_LOG(Delete, SINGLETON)
        delete instance;
        instance = nullptr;
    };

    int id = self.mCount;
    self.mSingletons.insert({id, {(uintptr_t)instance, deleter}}); // Register deleter
    self.mAddress2Id.insert({(uintptr_t)instance, id});            // Register ID
    self.mCount += 1;
    return instance;
}

template <typename SINGLETON>
int SingletonManager::GetId()
{
    SingletonManager &self = SingletonManager::GetSelf();
    auto address = (uintptr_t)(Get<SINGLETON>());
    return self.mAddress2Id[address];
}

template <typename SINGLETON>
void SingletonManager::Erase()
{
    EraseById(GetId<SINGLETON>());
}