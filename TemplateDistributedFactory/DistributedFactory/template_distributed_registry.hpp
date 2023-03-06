/**
 * @file template_distributed_registry.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <template_distributed_database.hpp>
#include <template_distributed_dbitem.hpp>

template <typename Base, typename... Args>
class Registry
{
public:
    // DabaBaseItemの型
    typedef DataBaseItem<Base, Args...> DBItem;

    // Baseの派生クラスのスマートポインタを生成する
    std::shared_ptr<Base> Create(const std::string& type, Args... args)
    {
        return mDataBase.Query(type)(args...); // creator functor is done.
    }

    // DataBaseItem<Base, Args...>クラスのスマートポインタを登録する
    void Register(std::shared_ptr<DBItem> item)
    {
        mDataBase.Registry(item);
    }

private:
    DataBase<DBItem> mDataBase;
};



// 可変長マクロの処理方法
// #define LOG(fmt, ...) printf(fmt, __VA_ARGS__)を作ると, 
// 通常は, 可変部分は1個以上値を渡さなければならない.
// LOG("FOOBAR\n"); // コンパイルエラー
// しかし、GCCの場合... ##__VA_ARGS__を使えば、0個の場合でも受け取れる.
// #define LOG(fmt, ...) printf(fmt, ##__VA_ARGS__); // OK
// MSVCの場合... 空の場合, コンパイラがprintf(fmt, __VA_ARGS__)の`,`を気を利かせて消してくれるが...
// Visual StudioのバージョンとC++17以前とC++20以降で動作が異なるので要注意.

#if defined(_MSC_VER) && (__cplusplus <= 201703L) // <= C++17
#   define IS_EXPAND(...) __VA__ARGS__
#else
#   define IS_VA_ARGS(...) , ##__VA_ARGS__
#endif

// // Rootの子孫用
#if __cplusplus <= 201703L // <= C++17
#   if defined(_MSC_VER) // `, IS_EXPAND`で可変長マクロを処理
        // _MSVC_TRADITIONAL 従来:1, 準拠:0
#       if defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL // 従来型のプリプロセッサ: __VA__ARGSが空の場合, __VA_ARGS__の前にある`,`は自動消去可能

#           define MULTI_RTTI_IMPL(class_name, ...) \
                const MultiRtti class_name::sMultiRtti(#class_name, IS_EXPAND(CONCATENATE_MULTI_RTTI(__VA_ARGS__)));

#       else // 新しいプリプロセッサ: __VA__ARGSが空のでも, __VA_ARGS__の前にある`,`は自動消去されない. ##__VA_ARGS__ が必要.

#           define MULTI_RTTI_IMPL(class_name, ...) \
                const MultiRtti class_name::sMultiRtti(#class_name IS_VA_ARGS(CONCATENATE_MULTI_RTTI(__VA_ARGS__)));

#       endif
#   else // GNU, Clang or AppleClang `, ##__VA_ARGS__'(GNU拡張)で処理

#       define MULTI_RTTI_IMPL(class_name, ...) \
            const MultiRtti class_name::sMultiRtti(#class_name IS_VA_ARGS(CONCATENATE_MULTI_RTTI(__VA_ARGS__)));

#   endif
#else // (>= C++20)
#   if  defined(_MSC_VER)
        // Visual Studio 2019 version 16.5.1 & コンパイラオプション(C++準拠モードを有効) /Zc:preprocessor
        // 可変長マクロ引数が空でも、`,`の自動消去は行われない. GCCと同じ, ##__VA_ARGS__で消える.
        // マクロ内で別のマクロに可変長マクロ引数を渡しても、以前と異なり、パラメータアンパックされる.
        // Visual Studio 2019 version 16.5.1のC++20では__VA_OPT__(,)も使える.
        // コンパイラオプション(C++準拠モードを有効) /Zc:preprocessor
#       if _MSC_VER >= 1925 // >= Visual Studio 2019 16.5.1

#           define MULTI_RTTI_IMPL(class_name, ...) \
                const MultiRtti class_name::sMultiRtti(#class_name __VA_OPT__(, ) CONCATENATE_MULTI_RTTI(__VA_ARGS__));

#       else // < Visual Studio 2019 16.5.1
            // _MSVC_TRADITIONAL 従来:1, 準拠:0
#           if defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL // 従来型プリプロセッサ

#               define MULTI_RTTI_IMPL(class_name, ...) \
                    const MultiRtti class_name::sMultiRtti(#class_name IS_EXPAND(CONCATENATE_MULTI_RTTI(__VA_ARGS__)));

#           else // 新型プリプロセッサ
#               define MULTI_RTTI_IMPL(class_name, ...) \
                    const MultiRtti class_name::sMultiRtti(#class_name, IS_VA_ARGS(CONCATENATE_MULTI_RTTI(__VA_ARGS__)));
#           endif
#       endif // #if _MSC_VER >= 1925
#   else // GNU, Clang or AppleClang

#   define MULTI_RTTI_IMPL(class_name, ...) \
        const MultiRtti class_name::sMultiRtti(#class_name __VA_OPT__(, ) CONCATENATE_MULTI_RTTI(__VA_ARGS__));

#   endif // #ifdef _MSC_VER
#endif // #if __cplusplus < 201703L