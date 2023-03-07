/**
 * @file function_pow.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <function_unary.hpp>
#include <operator_base_unary.hpp>

#include <template_distributed_registry.hpp>
#include <template_distributed_database.hpp>
#include <template_distributed_dbitem.hpp>

#include <cmath>
#include <string>
#include <iostream>
#include <tuple>

/* Get_PowRegistry */
/* Create_Pow */
DEFINE_DECLARE_OF_DISTRIBUTED_REGISTRY(Pow, double)

/* PowUnaryOp */
class PowUnaryOp : public BaseUnaryOp
{
public:
    double mA1;
    PowUnaryOp(double a1) : mA1(a1) {}

    template <typename T>
    inline T operator()(const T x)
    {
        return (T)std::pow(x, mA1);
    }
};

/* Pow */
template <typename T>
class Pow final : public UnaryFunction<T, PowUnaryOp, double>
{
public:
    Pow(const std::string & a0, double a1) : UnaryFunction<T, PowUnaryOp, double>(a0, a1) {}
    ~Pow() {}
    const char *GetClassName() const override final { return "Pow"; }

    FunctionPtr Clone() const override final
    {
        /**
         * @brief A. テンプレートの二段階名前解決に引っかかっていた.
         * @note https://qiita.com/kaityo256/items/2f24662a9ab8341ad6f4
         * C++ではコンパイル時にテンプレートに関連するものは, コンパイラに使用されるまで実体化が遅延される.
         * 一方, テンプレート関係ないもの(例えテンプレートクラスの非テンプレートメンバ関数でも)は,
         * テンプレートに関係なく即実体化される. この仕様によりテンプレートクラスよりも先に実体化してしまい,
         * テンプレート親クラスがわからない迷子なテンプレートクラスの非テンプレートメンバ関数が出現してしまう.
         * @note 解決策として所属するテンプレートクラスの実体化まで遅延させるためにthis->をつける.
         */
        auto& args = this->GetArgs();
        // std::cout << "tupoe[0] :"  << std::get<0>(args) << std::endl;
        return Create_Pow(this->mType, std::get<0>(args));
        // return Create_Pow(this->mType, (*this). template GetIndexArg<0>());
    }
};
