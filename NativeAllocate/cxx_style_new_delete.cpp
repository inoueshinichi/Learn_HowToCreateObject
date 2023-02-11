/**
 * @file cpp_style_new_delete.cpp
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

#define SIZEOF(value) std::printf("sizeof(%s) %ld\n", #value, sizeof(value));

class XY
{
public:
    float mX;
    float mY;
    XY() : mX(0.f), mY(0.f) {}
    XY(float x, float y) : mX(x), mY(y) {}
    virtual ~XY() {}
    virtual float Z() const { return 0; }
};

class XYZ : public XY
{
public:
    float mZ;
    XYZ() : XY(0.f, 0.f), mZ(0.f) {}
    XYZ(float x, float y, float z) : XY(x, y), mZ(z) {}
    ~XYZ() {}
    virtual float Z() const { return mZ; }
};

int main(int, char **)
{
    try
    {
        SIZEOF(XY)
        SIZEOF(XYZ)

        /**
         * @brief new演算子の役割
         * 1. メモリ確保を行う
         * 2. コンストラクターを呼び出す
         *    i. （非配列版）引数を指定できる
         *    ii. (配列版）引数を指定できず、デフォルトコンストラクターが呼ばれる
         */

        /*new/delete*/
        XY* xyObj = new XY(1, 2);
        std::cout << "mX " << xyObj->mX << " mY " << xyObj->mY << std::endl;
        delete xyObj;

        // ポリモーフィズム
        XY* xyzObj = new XYZ(1,2,3);
        std::cout << "mX " << xyzObj->mX << " mY " << xyzObj->mY << " mZ " << xyzObj->Z() << std::endl;
        delete xyzObj; 

        // new演算子によるコンストラクタはデフォルトコンストラクタしか呼べない(C++の仕様)
        // 回避策として, new演算子の機能を分割してplacement new(配置new)を使う or STLのサポートに頼る.
        int N = 3;
        XY* xyObjArray = new XY[N]; 
        delete[] xyObjArray;

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
