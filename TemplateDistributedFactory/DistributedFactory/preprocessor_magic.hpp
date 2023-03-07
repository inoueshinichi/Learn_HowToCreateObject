/**
 * @file preprocessor_magic.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

/**
 * @brief 可変長マクロのループ処理
 * @url https://in-neuro.hatenablog.com/entry/2020/10/21/155651
 * @note 可変長引数の上限があること前提として, マクロの再帰展開を使う.
 *
 */

/*可変引数の個数を数える方法 (Max8)*/
#define INDEX_RSEQ() 8, 7, 6, 5, 4, 3, 2, 1, 0
#define VA_ARGS_SIZE_IMPL(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, N, ...) N
#define VA_ARGS_SIZE_UPD(...) VA_ARGS_SIZE_IMPL(__VA_ARGS__)
#define VA_ARGS_SIZE(...) VA_ARGS_SIZE_UPD(__VA_ARGS__, INDEX_RSEQ())
#define CONCATENATE_UPD(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_UPD(x, y)

#define RECURSION_DEF_ARGS_1(A1) A1 a1
#define RECURSION_DEF_ARGS_2(A1, A2) RECURSION_DEF_ARGS_1(A1), A2 a2
#define RECURSION_DEF_ARGS_3(A1, A2, A3) RECURSION_DEF_ARGS_2(A1, A2), A3 a3
#define RECURSION_DEF_ARGS_4(A1, A2, A3, A4) RECURSION_DEF_ARGS_3(A1, A2, A3), A4 a4
#define RECURSION_DEF_ARGS_5(A1, A2, A3, A4, A5) RECURSION_DEF_ARGS_4(A1, A2, A3, A4), A5 a5
#define RECURSION_DEF_ARGS_6(A1, A2, A3, A4, A5, A6) RECURSION_DEF_ARGS_5(A1, A2, A3, A4, A5), A6 a6
#define RECURSION_DEF_ARGS_7(A1, A2, A3, A4, A5, A6, A7) RECURSION_DEF_ARGS_6(A1, A2, A3, A4, A5, A6), A7 a7
#define RECURSION_DEF_ARGS_8(A1, A2, A3, A4, A5, A6, A7, A8) RECURSION_DEF_ARGS_7(A1, A2, A3, A4, A5, A6, A7), A8 a8

/*__VA_ARGS__用 可変長引数定義マクロ */
#define DEFINE_ARGS(...) CONCATENATE(RECURSION_DEF_ARGS_, VA_ARGS_SIZE(__VA_ARGS__))(__VA_ARGS__)

#define RECURSION_ARGS_1(A1) a1
#define RECURSION_ARGS_2(A1, A2) RECURSION_ARGS_1(A1), a2
#define RECURSION_ARGS_3(A1, A2, A3) RECURSION_ARGS_2(A1, A2), a3
#define RECURSION_ARGS_4(A1, A2, A3, A4) RECURSION_ARGS_3(A1, A2, A3), a4
#define RECURSION_ARGS_5(A1, A2, A3, A4, A5) RECURSION_ARGS_4(A1, A2, A3, A4), a5
#define RECURSION_ARGS_6(A1, A2, A3, A4, A5, A6) RECURSION_ARGS_5(A1, A2, A3, A4, A5), a6
#define RECURSION_ARGS_7(A1, A2, A3, A4, A5, A6, A7) RECURSION_ARGS_6(A1, A2, A3, A4, A5, A6), a7
#define RECURSION_ARGS_8(A1, A2, A3, A4, A5, A6, A7, A8) RECURSION_ARGS_7(A1, A2, A3, A4, A5, A6, A7), a8

/*__VA_ARGS__用 可変長変数マクロ*/
#define ARGS(...) CONCATENATE(RECURSION_ARGS_, VA_ARGS_SIZE(__VA_ARGS__))(__VA_ARGS__)