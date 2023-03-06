/**
 * @file singleton_manager.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <singleton_manager-internal.hpp>

// 静的初期化
SingletonManager *SingletonManager::sSelf = nullptr;

std::mutex &SingletonManager::GetMutex()
{
    // 内部リンケージ
    static std::mutex sMtx;
    return sMtx;
}

SingletonManager::SingletonManager()
    : mCount(0)
{
    std::cout << "[Create] SingletonManager" << std::endl;
}

SingletonManager::~SingletonManager()
{
    Clear();
    std::cout << "[Delete] SingletonManager" << std::endl;
}

SingletonManager &SingletonManager::GetSelf()
{
    if (!sSelf)
    {
        sSelf = new SingletonManager{};
    }
    return *sSelf;
}

void SingletonManager::Clear()
{
    SingletonManager &self = SingletonManager::GetSelf();
    for (int i = 0; i < self.mCount; ++i)
    {
        // Singletonインスタンスを破棄
        EraseById(i);
    }

    // clear all
    self.mSingletons.clear();
    self.mAddress2Id.clear();
    self.mCount = 0;
}

void SingletonManager::DeleteSelf()
{
    if (sSelf)
    {
        delete sSelf;
        sSelf = nullptr;
    }
}

void SingletonManager::EraseById(int id)
{
    SingletonManager &self = SingletonManager::GetSelf();
    auto iter = self.mSingletons.find(id);
    if (iter == self.mSingletons.end())
    {
        return;
    }

    iter->second.second(); // Invoke deleter
    self.mAddress2Id.erase(iter->second.first);
    self.mSingletons.erase(iter);
}