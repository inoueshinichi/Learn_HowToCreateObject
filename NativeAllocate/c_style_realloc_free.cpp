/**
 * @file c_style_realloc_free.cpp
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
        /**
         * @brief realoc関数
         * https://marycore.jp/prog/c-lang/realloc/
         * reallocはメモリを再割り当て／再確保するための関数です。
         * 既存のメモリ領域のサイズを拡張／縮小する用途に利用します。
         * void *realloc(void *ptr, size_t size);
         * 第一引数	ptr	メモリオブジェクトへのポインタ（NULL可）
         * 第二引数	size	変更後のメモリサイズ（バイト単位）
         * 戻り値	──	再確保後のアドレス（失敗時NULL）
         * なお第一引数にNULLポインタが渡された場合には、malloc関数と同等の働きをします
         *
         * 仕様
         * reallocは第一引数に与えられたメモリオブジェクトを解放し、
         * 新たなオブジェクトへのポインタを戻り値として返します。
         * その際、元オブジェクトの値は新たなオブジェクトへとコピーされます。
         * ただし、第一引数に指定したポインタのアドレスと戻り値のアドレスが同一のアドレスになる場合もあります。
         * また、メモリ確保の失敗時にはNULLが返されるため、reallocの使い方にはちょっとしたコツや注意が必要となります。
         */

        int N = 5;
        int N_GT = 10;
        int N_LS = 3;

        float *memFloatArray = (float *)std::malloc(sizeof(float) * N);
        memFloatArray[0] = 2;
        memFloatArray[1] = 4;
        memFloatArray[2] = 8;
        memFloatArray[3] = 16;
        memFloatArray[4] = 32;
        std::ostringstream oss;
        for (int i = 0; i < N; ++i)
        {
            oss << "[" << i << "] " << memFloatArray[i] << "\n";
        }
        std::cout << oss.str();


        // メモリ拡張
        std::printf("メモリ拡張を行います\n");
        float* memFloatArrayGT = (float*)std::realloc(memFloatArray, sizeof(float) * N_GT);

        /* 失敗時 */
        if (memFloatArrayGT == nullptr) 
        {
            std::free(memFloatArrayGT); // 必要に応じて元のアドレスを手動で解放
            std::exit(EXIT_FAILURE); // 必要に応じてプログラムを終了
        }

        /* アドレスが変化した場合 */
        if (memFloatArray != memFloatArrayGT) 
        {
            // std::memcpy(memFloatArrayGT, memFloatArray, sizeof(float) * N); // コピーは不要（既にコピー済み）
            // std::free(memFloatArray); // 元オブジェクトは解放済みのため不要
            memFloatArray = memFloatArrayGT; // 必要に応じてポインタ変数を更新(もうmemFloatArrayが指すオブジェクトは破棄済み)
            std::cout << "reallocにより元アドレスが変化しました." << std::endl;
        }

        std::ostringstream oss2;
        for (int i = 0; i < N_GT; ++i)
        {
            oss2 << "[" << i << "] " << memFloatArray[i] << "\n";
        }
        std::cout << oss2.str();


        // メモリ縮小
        std::printf("メモリ縮小を行います\n");
        float* memFloatArrayLS = (float*)std::realloc(memFloatArray, sizeof(float) * N_LS);

        /* 失敗時 */
        if (memFloatArrayLS == nullptr)
        {
            std::free(memFloatArrayLS); // 必要に応じて元のアドレスを手動で解放
            std::exit(EXIT_FAILURE);    // 必要に応じてプログラムを終了
        }

        /* アドレスが変化した場合 */
        if (memFloatArray != memFloatArrayLS)
        {
            // std::memcpy(memFloatArrayLS, memFloatArray, sizeof(float) * N_LS); // コピーは不要（既にコピー済み）
            // std::free(memFloatArray); // 元オブジェクトは解放済みのため不要
            memFloatArray = memFloatArrayLS; // 必要に応じてポインタ変数を更新(もうmemFloatArrayが指すオブジェクトは破棄済み)
            std::cout << "reallocにより元アドレスが変化しました." << std::endl;
        }

        std::ostringstream oss3;
        for (int i = 0; i < N_LS; ++i)
        {
            oss3 << "[" << i << "] " << memFloatArray[i] << "\n";
        }
        std::cout << oss3.str();

        std::cout << "out of range を参照(未定義)" << std::endl;
        std::cout << "[3] " << memFloatArray[3] << std::endl;

        std::free(memFloatArray);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}