/**
 * @file test_static_func_invoked_in_template_func.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief テンプレート関数内部からStatic関数を呼ぶ際にリンクエラーになる問題のチェック
 * @version 0.1
 * @date 2023-04-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <test_class.hpp>

#include <exception>
#include <iostream>

auto main(int, char**) -> int
{
    try
    {
        TestClass testClass;
        TestClass::GetStaticInt() = 100;

        std::cout << "Static Variable: " << testClass.GetVariable<double>() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}

