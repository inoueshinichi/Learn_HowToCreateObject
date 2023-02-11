/**
 * @file cxx_style_void_shared_ptr.cpp
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

class XY
{
public:
    float mX;
    float mY;
    XY() : mX(0.f), mY(0.f) { std::cout << "constructor XY" << std::endl; }
    XY(float x, float y) : mX(x), mY(y) { std::cout << "constructor XY" << std::endl; }
    virtual ~XY() { std::cout << "destructor XY" << std::endl; }
    virtual float Z() const { return 0; }
};

class XYZ : public XY
{
public:
    float mZ;
    XYZ() : XY(0.f, 0.f), mZ(0.f) { std::cout << "constructor XYZ" << std::endl; }
    XYZ(float x, float y, float z) : XY(x, y), mZ(z) { std::cout << "constructor XYZ" << std::endl; }
    ~XYZ() { std::cout << "destructor XYZ" << std::endl; }
    virtual float Z() const { return mZ; }
};

int main(int, char **)
{
    try
    {
        /**
         * @brief shared_ptr<void>に、あらゆる型のポインタを格納できる
         * voidをテンプレート引数とするshared_ptrに対してどんな型のポインタを代入したとしても、
         * 代入した型のデストラクタは、正しく実行される。通常、void*に型変換して代入されたポインタは、
         * delete演算子を呼んだとしても元の型のデストラクタは呼び出されない。
         * しかしshared_ptrの場合は、代入されたポインタの型が持つデストラクタが正しく実行されることが保証される。
         * 保証の文面はデストラクタのページを参照。
         * https://cpprefjp.github.io/reference/memory/shared_ptr.html
         */
        std::shared_ptr<void> sVoidPtr = std::make_shared<XYZ>(7, 8, 9);
        std::cout << "use_count " << sVoidPtr.use_count() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}