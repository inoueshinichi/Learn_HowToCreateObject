/**
 * @file function_base.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <function.hpp>
#include <tuple>
#include <type_traits>
#include <iostream>



/**
 * @brief Type Eraser
 * 非テンプレート基底クラスとテンプレート派生クラスによる
 * 型推論と継承を利用して型情報をカプセル化しつつ基底クラスから
 * 自由にアクセスできる. 結果, ポリモーフィズムを実現できる.
 */

// コンストラクタ引数を保持するクラス
template <typename... Args>
class FunctionBase : public Function
{
protected:
    using TupleArgs = std::tuple<std::remove_reference_t<Args>...>;
    TupleArgs mArgs;

    // constexpr void CheckTupleArgs(TupleArgs args, int index)
    // {
    //     if constexpr (index < sizeof...(Args))
    //     {
    //         std::cout << "tuple[" << index << "]: " << std::get<index>(args) << std::endl;
    //         CheckTupleArgs(args, ++index);
    //     }
    // }

public:
    FunctionBase(const std::string& type, Args... args) : Function(type), mArgs(std::make_tuple(args...))
    {
        // CheckTupleArgs(mArgs, 0);
    }
    virtual ~FunctionBase() {}

    constexpr int NumArgs() const { return sizeof...(Args); }
    constexpr const TupleArgs &GetArgs() const { return mArgs; }

    template <int Index>
    constexpr auto GetIndexArg() -> decltype(std::get<Index>(this->mArgs))
    {
        return std::get<Index>(this->mArgs);
    }    
};