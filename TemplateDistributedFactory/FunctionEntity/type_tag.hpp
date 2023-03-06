/**
 * @file type_to_string.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <type_traits>
#include <string>

template <typename T = void>
struct TypeTag
{
    static constexpr const char* value = "void";
};

template <>
struct TypeTag<float>
{
    static constexpr const char* value = "float";
};

template <>
struct TypeTag<double>
{
    static constexpr const char* value = "double";
};

template <>
struct TypeTag<long double>
{
    static constexpr const char* value = "ldouble";
};

template<>
struct TypeTag<char>
{
    static constexpr const char* value = "char";
};

template <>
struct TypeTag<unsigned char>
{
    static constexpr const char* value = "uchar";
};

template <>
struct TypeTag<short>
{
    static constexpr const char* value = "short";
};

template <>
struct TypeTag<unsigned short>
{
    static constexpr const char* value = "ushort";
};

template <>
struct TypeTag<int>
{
    static constexpr const char* value = "int";
};

template <>
struct TypeTag<unsigned int>
{
    static constexpr const char* value = "uint";
};

template <>
struct TypeTag<long long>
{
    static constexpr const char* value = "llong";
};

template <>
struct TypeTag<unsigned long long>
{
    static constexpr const char* value = "ullong";
};

