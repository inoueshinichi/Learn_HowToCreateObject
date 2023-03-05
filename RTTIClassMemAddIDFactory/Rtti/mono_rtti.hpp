/**
 * @file mono_rtti.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 単一継承型RTTIシステム (計算コスト小)
 * @version 0.1
 * @date 2023-03-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <string>

class MonoRtti
{
public:
    MonoRtti(const std::string &className) : mClassName(className), mMonoBaseRtti(nullptr) {}
    MonoRtti(const std::string &className, const MonoRtti &baseRtti) 
        : mClassName(className), mMonoBaseRtti(&baseRtti) {}
    ~MonoRtti() {}

    const std::string& GetClassName() const { return mClassName; }
    bool IsExactly(const MonoRtti &rtti) const { return (this == &rtti); }        // アドレス比較なので高速
    bool operator==(const MonoRtti &rtti) const { return this->IsExactly(rtti); } // アドレス比較
    bool DerivedFrom(const MonoRtti &rtti) const
    {
        const MonoRtti *compare = this;
        while (compare != nullptr)
        {
            if (compare == &rtti)
                return true;
            compare = compare->mMonoBaseRtti;
        }
        return false;
    }

private:
    std::string mClassName;
    const MonoRtti *mMonoBaseRtti;

    // コピー系は禁止
    MonoRtti(const MonoRtti&) = delete;
    MonoRtti& operator=(const MonoRtti&) = delete;
};

///////////////////////////
// Headerの適用先クラスに定義
///////////////////////////
#define MONO_RTTI_DECL                              \
    public:                                         \
        static const MonoRtti sMonoRtti;            \
        virtual const MonoRtti &GetRtti() override  \
        {                                           \
            return sMonoRtti;                       \
        }

#define MONO_RTTI_NO_PARENT_DECL                    \
    public:                                         \
        static const MonoRtti sMonoRtti;            \
        virtual const MonoRtti &GetRtti()           \
        {                                           \
            return sMonoRtti;                       \
        }

//////////////////////////
// 適用先クラスのSourceに定義
//////////////////////////
// Root用
#define MONO_RTTI_IMPL_NOPARENT(class_name)           \
    const MonoRtti class_name::sMonoRtti(#class_name);

// Rootの子孫用
#define MONO_RTTI_IMPL(class_name, base_name)                               \
    const MonoRtti class_name::sMonoRtti(#class_name, base_name::sMonoRtti);