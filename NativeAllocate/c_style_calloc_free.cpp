/**
 * @file c_style_calloc_free.cpp
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

int main(int, char **)
{
    try
    {
        int N = 5;
        
        /*calloc/free*/
        float *memFloatArray = (float *)std::calloc(N, sizeof(float)); // 確保したメモリはゼロクリアされる
        std::ostringstream oss2;
        for (int i = 0; i < N; ++i)
        {
            oss2 << "[" << i << "] " << memFloatArray[i] << "\n";
        }
        std::cout << oss2.str();
        std::free(memFloatArray);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
