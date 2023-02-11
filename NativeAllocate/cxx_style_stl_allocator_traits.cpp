/**
 * @file cxx_style_std_allocator_trait.cpp
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
         * @brief std::allocator_traits<allocator_type>クラスは,
         *  STLのstd::allocatorの方式に準拠していれば, traitsクラスを通して,
         *  型依存をなくしてallocator関数にアクセスできるポリシー的な使い方ができる.
         *  ただし、記述が複雑になるのでtypedef, using, decltypeなどを適宜使うこと.
         */
        std::allocator<XYZ> xyzAllocator;
        using alloc_traits = std::allocator_traits<decltype(xyzAllocator)>;
        int N = 5;
        
        XYZ* objArray = alloc_traits::allocate(xyzAllocator, N);
        for (int i = 0; i < N; ++i)
        {
            alloc_traits::construct(xyzAllocator, (XYZ*)(objArray + i), i*1, i*2, i*3);
        }

        for (int k = 0; k < N; ++k)
        {
            std::cout << "mX " << objArray[k].mX << " mY " << objArray[k].mY << " mZ " << objArray[k].Z() << std::endl;
        }

        for (auto i = objArray; i != (objArray + N); ++i)
        {
            alloc_traits::destroy(xyzAllocator, i);
        }

        std::cout << "check" << std::endl;

        alloc_traits::deallocate(xyzAllocator, objArray, N);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}