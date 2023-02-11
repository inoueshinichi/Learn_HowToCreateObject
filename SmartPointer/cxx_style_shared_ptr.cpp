/**
 * @file cxx_style_shared_ptr.cpp
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
         * @brief std::shared_ptr
         * 所有権を複数で共有する. 参照カウント方式が多い. 参照カウンタはスレッドセーフ.
         * 最後の所有者が破棄したとき, ポインタが指すオブジェクトのデストラクタが呼ばれる.
         * コピーコンストラクタ, コピー代入演算子 -> 参照カウント+1 or ±0
         * ムーブコンストラクタ, ムーブ代入演算子 -> 参照カウント±0, ムーブ元は無効になる.
         *
         * 非スレッドセーフに参照カウントを増減させる方法はない。
         * シングルスレッドでのパフォーマンスが重要で、
         * スレッドセーフであることによるオーバーヘッドが問題になる場合、
         * ムーブを活用すればパフォーマンスを改善できる。
         * https://cpprefjp.github.io/reference/memory/shared_ptr.html
         */

        // i) 通常の使い方
        std::shared_ptr<XY> sPtr = std::make_shared<XYZ>(3,2,1); // ポリモーフィズム可能
        std::cout << "mX " << sPtr->mX << " mY " << sPtr->mY << " mZ " << sPtr->Z() << std::endl;
        std::cout << "use_count " << sPtr.use_count() << std::endl;

        std::shared_ptr<XY> ssPtr(sPtr);
        std::cout << "mX " << ssPtr->mX << " mY " << ssPtr->mY << " mZ " << ssPtr->Z() << std::endl;
        std::cout << "use_count " << ssPtr.use_count() << std::endl;

        std::shared_ptr<XY> sssPtr = ssPtr;
        std::cout << "mX " << sssPtr->mX << " mY " << sssPtr->mY << " mZ " << sssPtr->Z() << std::endl;
        std::cout << "use_count " << sssPtr.use_count() << std::endl;

        ssPtr = sssPtr;
        std::cout << "mX " << ssPtr->mX << " mY " << ssPtr->mY << " mZ " << ssPtr->Z() << std::endl;
        std::cout << "use_count " << ssPtr.use_count() << std::endl;

        std::shared_ptr<XY> sMovePtr(std::move(sssPtr));
        std::cout << "mX " << sMovePtr->mX << " mY " << sMovePtr->mY << " mZ " << sMovePtr->Z() << std::endl;
        std::cout << "use_count " << sMovePtr.use_count() << std::endl;

        if (!sssPtr.get())
        {
            std::cout << "ムーブ元のsssPtrは所有権を持っていません." << std::endl;
            std::printf("sssPtr.get() : %p\n", (void*)sssPtr.get());
        }

        // ii) 動的キャスト
        std::shared_ptr<XYZ> sCastPtr;
        if (std::dynamic_pointer_cast<XYZ>(sMovePtr))
        {
            std::cout << "XY -> XYZ dynamic_pointer_cast OK" << std::endl;
            sCastPtr = std::dynamic_pointer_cast<XYZ>(sMovePtr);
            std::cout << "mX " << sCastPtr->mX << " mY " << sCastPtr->mY << " mZ " << sCastPtr->Z() << std::endl;
            std::cout << "use_count " << sCastPtr.use_count() << std::endl;
        }

        // iii) reinterpret_pointer_cast
        std::shared_ptr<void> sVoidPtr;
        sVoidPtr = std::reinterpret_pointer_cast<void>(sCastPtr);
        std::cout << "use_count " << sVoidPtr.use_count() << std::endl;
        if (sVoidPtr)
        {
            std::cout << "reinterpret_pointer_cast<void> OK" << std::endl;
            std::shared_ptr<XY> sScopePtr;
            sScopePtr = std::static_pointer_cast<XY>(sVoidPtr); // std::static_pointer_cast
            std::cout << "mX " << sScopePtr->mX << " mY " << sScopePtr->mY << " mZ " << sScopePtr->Z() << std::endl;
            std::cout << "use_count " << sScopePtr.use_count() << std::endl;
        }

        // iv) const_pointer_cast
        std::shared_ptr<const XYZ> sConstPtr = std::make_shared<const XYZ>(4,5,6);
        std::cout << "mX " << sConstPtr->mX << " mY " << sConstPtr->mY << " mZ " << sConstPtr->Z() << std::endl;
        std::cout << "use_count " << sConstPtr.use_count() << std::endl;

        std::cout << "Is sConstPtr unique ? -> " << std::boolalpha << sConstPtr.unique() << std::noboolalpha << std::endl;

        std::shared_ptr<XYZ> ssssPtr = std::const_pointer_cast<XYZ>(sConstPtr);
        std::cout << "mX " << ssssPtr->mX << " mY " << ssssPtr->mY << " mZ " << ssssPtr->Z() << std::endl;
        std::cout << "use_count " << ssssPtr.use_count() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}