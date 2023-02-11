/**
 * @file cxx_style_weak_ptr.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <exception>
#include <iostream>
#include <sstream>

#include <memory>
#include <cassert>

#define SIZEOF(value) std::printf("sizeof(%s) %ld\n", #value, sizeof(value));

// std::weak_ptr有効フラグ
#define MY_WEAK_PTR

class B;

class A
{
public:
#if defined(MY_WEAK_PTR)
    std::weak_ptr<B> mBPtr;
#else
    std::shared_ptr<B> mBPtr;
#endif
    ~A();
    const char* getClassName() const { return "A"; }
    void check() const;
};

class B
{
public:
    std::shared_ptr<A> mAPtr;
    ~B() { std::cout << "destructor B" << std::endl; }
    const char* getClassName() const { return "B"; }
    void check() const
    {
        std::cout << "Get shared_ptr value: " << mAPtr->getClassName() << " in B::check()." << std::endl;
    }
};

// A impl
A::~A()
{
#if defined(MY_WEAK_PTR)
    std::cout << "destructor A" << std::endl;

    if (mBPtr.expired())
    {
        std::cout << "B is expired in ~A()." << std::endl;
    }
#endif
}

void A::check() const
{
#if defined(MY_WEAK_PTR)
    if (std::shared_ptr<B> ret = mBPtr.lock())
    {
        // [WARN] このスコープの中でret.reset()を呼ぶとretがダングリングポインタになってしまう.
        std::cout << "Monitoring shared_ptr that has " << mBPtr.use_count() << " owners. in A::check()." << std::endl;
        std::cout << "Get weak_ptr value: " << ret->getClassName() << " in A::check()." << std::endl;
    }
    else
    {
        std::cout << "B is expired! in A::check()." << std::endl;
    }
#else
    std::cout << "Get shared_ptr value: " << mBPtr->getClassName() << " in A::check()." << std::endl;
#endif
}

int main(int, char **)
{
    try
    {
        /**
         * @brief std::weak_ptr
         * std::shared_ptrの循環参照を解決するスマートポインタ. つまり, shared_ptrが管理するインスタンスを覗き見する.
         *
         * 参照カウントという機構には、循環参照を解決できないという問題がある。
         * AがBへのshared_ptrを保持し、BもまたAへのshared_ptrを保持する、ということをした場合、
         * 参照カウントが永遠に0にならず、リソースリークが発生する。
         *
         * このような構造がどうしても必要な場合、一方はshared_ptrを保持し、
         * 一方はそのshared_ptrへのweak_ptrを保持する、というようにすることで、循環参照を解決できる。
         *
         * weak_ptrは、監視対象のshared_ptrオブジェクトの参照カウントを、加算も減算もしない。
         *
         * shared_ptrは、リソースを使用している間は解放されないという保証があるということも特徴の一つではあるので、
         * weak_ptrにする対象が、リソースが参照できなくなっても問題ないか、ということを確認した上で使用すること。
         *
         * https://cpprefjp.github.io/reference/memory/weak_ptr.html
         */

#if !defined(MY_WEAK_PTR)
        std::cout << "AとBで循環参照が発生しています!" << std::endl;
#endif
        // 循環参照発生テスト
        std::shared_ptr<A> objA = std::make_shared<A>();
        std::shared_ptr<B> objB = std::make_shared<B>();
        std::shared_ptr<B> otherObjB = objB;
        objA->mBPtr = objB;
        objB->mAPtr = objA;

        objA->check(); // 内部でlock()関数でobjBの参照を取得
        objB->check();

        // 所有権の放棄
        objA.reset();
        objB.reset();

        // 明示的にデストラクタを呼ぶのはダメ 
        // objA->~A();
        // objB->~B();

        assert(objA.get() == nullptr/*, "No invoke destructor A!"*/);
        assert(objB.get() == nullptr/*, "No invoke destructor B!"*/);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}