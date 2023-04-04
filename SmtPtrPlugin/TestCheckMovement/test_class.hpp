/**
 * @file test_class.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <mutex>

class TestClass
{
public:
    TestClass() = default;
    ~TestClass() = default;
    TestClass(const TestClass&) = default;
    TestClass& operator=(const TestClass&) = default;
    TestClass(TestClass&&) = default;
    TestClass& operator=(TestClass&&) = default;

    // テンプレートメンバ関数からstaticな関数を呼ぶ際に, inline化してないとリンクエラーになる...
    // static int& GetStaticInt();
    // static std::mutex& GetStaticMutex();

    static int &GetStaticInt()
    {
        std::mutex &sMtx = TestClass::GetStaticMutex();
        std::lock_guard<std::mutex> locker(sMtx);

        static int sIntVal = 0;
        return sIntVal;
    }

    static std::mutex &GetStaticMutex()
    {
        static std::mutex sMtx;
        return sMtx;
    }

    template <typename T>
    T GetVariable()
    {
        int& sVal = GetStaticInt();
        T retVal = static_cast<T>(sVal);
        return retVal;
    }
};