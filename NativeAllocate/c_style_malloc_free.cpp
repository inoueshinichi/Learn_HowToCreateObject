/**
 * @file c_style_malloc_free.cpp
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
#include <cstdlib> // malloc, free, calloc, realloc
#include <cstdio>

#define SIZEOF(value) std::printf("sizeof(%s) %ld\n", #value, sizeof(value));

int main(int, char**)
{
    try
    {
        /*sizeof*/
        double x = 3.14;
        SIZEOF(x)  // 8byte
        SIZEOF(&x) // 8byte(with 64bit)
        SIZEOF(char) // 1byte
        SIZEOF(short) // 2byte
        SIZEOF(int) // 4byte
        SIZEOF(long) // 8byte
        SIZEOF(float) // 4byte
        SIZEOF(double) // 8byte

        /*malloc/free*/
        int *memInt = (int *)std::malloc(sizeof(int)); // (型名*)malloc(確保する領域のサイズ)
        std::cout << "memInt " << *memInt << std::endl;
        std::free(memInt);

        int N = 5;
        int*memIntArray = (int*)std::malloc(sizeof(int) * N); // int[5]を確保
        std::ostringstream oss;
        for (int i = 0; i < N; ++i)
        {
            oss << "[" << i << "] " << memIntArray[i] << "\n";
        }
        std::cout << oss.str();
        std::free(memIntArray);

        /*calloc/free*/
        std::cout << "-----" << std::endl;
        float* memFloatArray = (float *)std::calloc(N, sizeof(float)); // 確保したメモリはゼロクリアされる
        std::ostringstream oss2;
        for (int i = 0; i < N; ++i)
        {
            oss2 << "[" << i << "] " << memFloatArray[i] << "\n";
        }
        std::cout << oss2.str();
        std::free(memFloatArray);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}