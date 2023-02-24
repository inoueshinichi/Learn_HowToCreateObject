/**
 * @file sp_unique_ptr.cpp
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

int main(int, char**)
{
    try
    {
        /**
         * @brief std::unique_ptr
         * 所有権を移譲させないスマートポインタ
         * std::move()を使うと移譲できるが, 元インスタンスが無効になる.
         * https://cpprefjp.github.io/reference/memory/unique_ptr.html
         */
        // i. 通常の使い方
        std::unique_ptr<XYZ> uPtr = std::make_unique<XYZ>(3,2,1);
        std::cout << "mX " << uPtr->mX << " mY " << uPtr->mY << " mZ " << uPtr->Z() << std::endl;

        // ii. 配列バージョン
        int N = 5;
        std::unique_ptr<XYZ[]> uPtrArray = std::make_unique<XYZ[]>(N);
        for (int i = 0; i < N; ++i)
        {
            std::cout << "mX " << uPtrArray[i].mX << " mY " << uPtrArray[i].mY << " mZ " << uPtrArray[i].Z() << std::endl;
        }

        std::printf("---\n");

        // iii. ムーブ構築すると元インスタンスは無効になる
        std::unique_ptr<XYZ> uMovePtr(std::move(uPtr));
        std::cout << "mX " << uMovePtr->mX << " mY " << uMovePtr->mY << " mZ " << uMovePtr->Z() << std::endl;

        if (uPtr)
        {
            std::abort(); // uPtrは空
        }
        std::printf("Moved uPtr: %p\n", (void*)uPtr.get());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}