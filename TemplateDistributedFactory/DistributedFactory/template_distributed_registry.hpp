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
        return mDataBase.Query(type)(type, args...); // creator functor is done.
    }

    // DataBaseItem<Base, Args...>クラスのスマートポインタを登録する
    void Register(std::shared_ptr<DBItem> item)
    {
        mDataBase.Registry(item);
    }

private:
    DataBase<DBItem> mDataBase;
};

#include <preprocessor_magic.hpp>

#if defined(_MSC_VER) && defined(_MSVC_TRADITIONAL) && (_MSVC_TRADITIONAL)
// MSVCのC++準拠していないコンパイラバージョンに対する対策(問題: マクロ内の可変長マクロが自動展開されない)
#   define VA_EXPAND(...) __VA__ARGS__
#endif

// 今回は, >= C++20 でも__VA_OPT__(,)は使わない. 代わりに`, ##__VA_ARGS__`を使う
#define VA_ARGS(...) , ##__VA_ARGS__

// `<=C++17`と`>=C++20`の両方に対応
#if defined(_MSVC_TRADITIONAL) && (_MSVC_TRADITIONAL)
    // MSVC traditional preprocesor (No compliance of c++ standard)
    // Use `VA_EXPAND(...)` for unpack va macro in macro
    
#else
    // MSVC (new preprocessor), GNU, Clang, AppleClang
    // Use `VA_ARGS(...)` for va macro in case of zero arg in macro

    #define DEFINE_DECLARE_OF_DISTRIBUTED_REGISTRY(class_name, ...)                                         \
        Registry<Function VA_ARGS(__VA_ARGS__)> &Get_##class_name##Registry();                              \
        FunctionPtr Create_##class_name(DEFINE_ARGS(const std::string& VA_ARGS(__VA_ARGS__)));

    #define DEFINE_IMPLEMENTATION_OF_DISTRIBUTED_REGISTRY(class_name, ...)                                  \
        Registry<Function VA_ARGS(__VA_ARGS__)> &Get_##class_name##Registry()                               \
        {                                                                                                   \
            struct class_name##RegistryHolder /* Get_RegistryとRegistryHolderでIDを実現している */ \
            {                                                                                               \
                Registry<Function VA_ARGS(__VA_ARGS__)> instance;                                           \
            };                                                                                              \
            return SingletonManager::Get<class_name##RegistryHolder>()->instance;                           \
        }                                                                                                   \
                                                                                                            \
        FunctionPtr Create_##class_name(DEFINE_ARGS(const std::string& VA_ARGS(__VA_ARGS__)))               \
        {                                                                                                   \
            SystemInitForRegister();                                                                        \
            /* new std::shared_ptr<class_name> */                                                           \
            return Get_##class_name##Registry().Create(ARGS(const std::string& VA_ARGS(__VA_ARGS__)));      \
        }

#define DEFINE_REGISTER_OF_DISTRIBUTED_REGISTRY(class_name, type, ...)                                \
    {                                                                                                 \
        std::function<FunctionPtr(const std::string &VA_ARGS(__VA_ARGS__))> creator =                 \
            [](DEFINE_ARGS(const std::string &VA_ARGS(__VA_ARGS__))) -> auto {                        \
            return std::make_shared<class_name<type>>(ARGS(const std::string &VA_ARGS(__VA_ARGS__))); \
        };                                                                                            \
        using ItemT = DataBaseItem<Function VA_ARGS(__VA_ARGS__)>;                                    \
        Get_##class_name##Registry().Register(std::make_shared<ItemT>(#type, creator));               \
    }
#endif
