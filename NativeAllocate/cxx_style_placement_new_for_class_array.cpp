/**
 * @file cxx_style_placement_new_for_class_array.cpp
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
         * @brief new演算子&配置new演算子による引数付きコンストラクタをもつオブジェクトの配列生成
         *  1. new演算子で配列で必要なバイト数を確保する(sizeof(CLASS) * N)
         *  2. forループ内で配置new演算子を使ってコンストラクターを呼び出す
         *  3. オブジェクトの使用
         *  4. forループ内でデストラクタを呼び出す
         *  5. delete演算子を呼び出す
         */
        int N = 10;
        // 1. メモリ確保
        XYZ* objArray = static_cast<XYZ*>(::operator new(sizeof(XYZ) * N)); // ::operator new演算子の実装は, おおよそstd::malloc()関数
        
        // 2. コンストラクタ実行
        for (int i = 0; i < N; ++i)
        {
            ::new (objArray + i)XYZ(i*1, i*2, i*3);
            // (objArray + i)->XYZ(i*1, i*2, i*3); // エラー
            // *(objArray + i).XYZ(i*1, i*2, i*3); // エラー
        }

        // 3. オブジェクトの使用
        for (int k = 0; k < N; ++k)
        {
            std::cout << "mX " << objArray[k].mX << " mY " << objArray[k].mY << " mZ " << objArray[k].Z() << std::endl;
        }


        // 4. デストラクタ実行
        for (int i = 0; i < N; ++i)
        {
            objArray[i].~XYZ();
        }

        // 5. メモリ解放
        ::operator delete(objArray);

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}