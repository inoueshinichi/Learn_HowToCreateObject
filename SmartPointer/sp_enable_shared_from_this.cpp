/**
 * @file sp_shared_ptr_from_this.cpp
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

#define SIZEOF(value) std::printf("sizeof(%s) %ld\n", #value, sizeof(value));

// 抽象インターフェースクラス
class Interface : public std::enable_shared_from_this<Interface>
{
public:
    Interface() { std::cout << "constructor Interface" << std::endl; }
    virtual ~Interface() { std::cout << "destructor Interface" << std::endl; }

    std::shared_ptr<Interface> self()
    {
        // thisを指すshared_ptrオブジェクトを作る
        return shared_from_this();
    }

    virtual float Z() const = 0;
};

// 実装クラス
class XYZ : public Interface
{
public:
    float mX;
    float mY;
    float mZ;
    XYZ() : mX(0.f), mY(0.f), mZ(0.f) { std::cout << "constructor XYZ" << std::endl; }
    XYZ(float x, float y, float z) : mX(x), mY(y), mZ(z) { std::cout << "constructor XYZ" << std::endl; }
    ~XYZ() { std::cout << "destructor XYZ" << std::endl; }
    virtual float Z() const { return mZ; }
};

int main(int, char **)
{
    try
    {
        /**
         * @brief std::enable_shared_from_this
         * 
         * enable_shared_from_thisは、shared_ptrで管理しているオブジェクトのthisポインタを、
         * shared_ptrとして扱うことを可能にするためのクラスである
         *
         * thisポインタを単純にshared_ptr<T>(this)としてしまうと、参照カウントが増えず、
         * deleteが2重に呼ばれてしまいバグを引き起こすことになるため、そうならないようにこのクラスを使用してthisを扱う。
         *
         * このクラスは、shared_ptrとして管理するクラスの基底クラスとして使用する。
         * このクラスをpublic継承したクラスでpublicメンバ関数shared_from_this()を使用することで、
         * thisを指すshared_ptrオブジェクトを取得できる。
         *
         * このクラスを継承する際には、このクラスのテンプレート引数として、
         * 派生クラス(このクラスを継承するクラス自身)を指定すること。
         * (このようにテンプレート引数を指定する方法を、CRTP:Curiously Recurring Template Patternと言う)
         *
         * enable_shared_from_thisクラスの正しい使い方は、
         * 1. shared_ptrで管理するクラスが
         * 2. このクラスをpublic継承して、
         * 3. shared_from_this()メンバ関数を使用して、thisを指すshared_ptrオブジェクトを取得する。
         *
         * https://cpprefjp.github.io/reference/memory/enable_shared_from_this.html
         */

        /**
         * @brief 誤った使い方
         * struct X : public std::enable_shared_from_this<X> 
         * {
         *  void func() 
         *  {
         *      auto self = shared_from_this();  // (2) NG : thisがshared_ptrで管理されていない
         *  }
         * };
         * 
         * auto ptr = new X();  // (1) shared_ptr管理でない場合...
         * ptr->func();
         */

        std::shared_ptr<Interface> sPtr = std::make_shared<XYZ>(1,2,3);
        std::shared_ptr<Interface> ssPtr = sPtr->self(); // this

        std::cout << "ssPtr->Z(): " << ssPtr->Z() << std::endl;

        assert(sPtr == ssPtr);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}