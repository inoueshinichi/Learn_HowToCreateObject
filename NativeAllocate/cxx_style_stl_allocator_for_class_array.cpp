/**
 * @file cxx_style_stl_allocator_for_class_array.cpp
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
    XYZ(float z) : XY(0.f, 0.f), mZ(z) {}
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
         * @brief STLのstd::allocatorクラスを使った引数付き配列クラスの作成
         *  1. std::Allocator<class_type>インスタンスを生成
         *  2. メンバメソッドallocate()でメモリ確保
         *  3. std::uninitialized_fill_n()関数で引数付きコンストラクタを実行. (引数なしの場合, メンバメソッドconstruct()を使う)
         *  4. オブジェクトの使用
         *  5. std::destroy_n()関数でデストラクタを実行
         *  5. メンバメソッドdeallocate()でメモリ解放
         */
        int N = 10;

        // 1. std::Allocatorインスタンスを生成
        std::allocator<XYZ> xyzAllocator;

        // 2. メモリ確保
        XYZ *objArray = xyzAllocator.allocate(N); // 10要素の配列

        // 3. コンストラクタ実行
        // std::uninitialized_fill_n(objArray, N, 3); // 引数一つだけしかできない
        // std::uninitialized_value_construct_n(objArray, N); // デフォルトコンストラクタ呼び出し
        // 全要素, 複数のコンストラクタ引数対応
        for (int i = 0; i != N; ++i)
        {
            xyzAllocator.construct((objArray + i), 1*i, 2*i, 3*i); // ::operator new(objArray+i) XYZ(1*i,2*i,3*i)
        }

        // 3. オブジェクトの使用
        for (int k = 0; k < N; ++k)
        {
            std::cout << "mX " << objArray[k].mX << " mY " << objArray[k].mY << " mZ " << objArray[k].Z() << std::endl;
        }

        // 4. デストラクタ実行
        // std::destroy_n(objArray, N); // こっちでもOK
        for (int i = 0; i != N; ++i)
        {
            xyzAllocator.destroy((objArray + i));
        }

        // 5. メモリ解放
        xyzAllocator.deallocate(objArray, N);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}