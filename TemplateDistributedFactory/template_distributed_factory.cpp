/**
 * @file template_distributed_factory.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <exception>
#include <memory>
#include <tuple>

/* Singleton manager */
#include <singleton_manager.hpp>

/* Interface of generic func */
#include <function.hpp>

/* Generic func with template distributed registry. */
#include <function_sin.hpp>
#include <function_add.hpp>
#include <function_pow.hpp>


int main(int, char**)
{
    try
    { 
        std::vector<FunctionPtr> funcs;
        funcs.push_back(Create_Sin("float"));
        funcs.push_back(Create_Sin("double"));
        funcs.push_back(Create_Add("float"));
        funcs.push_back(Create_Add("double"));
        funcs.push_back(Create_Pow("float", 3.14));

        // Clone
        FunctionPtr pow = funcs.back()->Clone();
        
        // Check
        for (auto& func : funcs)
        {
            std::printf("Name `%s`, Type `%s`\n", func->GetClassName(), func->GetType().c_str());
        }

        // Delete SingletonManager
        SingletonManager::DeleteSelf();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}