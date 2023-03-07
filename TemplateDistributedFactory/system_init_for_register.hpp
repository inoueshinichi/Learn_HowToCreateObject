/**
 * @file system_init_for_register.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <memory>
#include <functional>
#include <mutex>

/* Distributed factory system */
#include <template_distributed_registry.hpp>
#include <template_distributed_database.hpp>
#include <template_distributed_dbitem.hpp>

/* Generic Function Headers */
#include <function_sin.hpp> // Unary zero args
#include <function_add.hpp> // Binary zero args
#include <function_pow.hpp> // Unary one args

/**
 * @brief 重複定義によるリンクエラーが起きたらinlneをつける.
 * 参考: https://qiita.com/Luke02561/items/43bed4008dd707617a94
 *
 */
inline void SystemInitForRegister()
{
    static std::mutex sMtx;
    std::lock_guard<std::mutex> locker(sMtx);

    static volatile bool initialized = false;

    if (initialized)
    {
        return;
    }

    initialized = true;

    /* プロセスで初回限りの起動を行う処理を下記に追加 */

    
    // Unary ope func
    {
        // Zero arguments
        {
            // Sin<float>
            {
                std::function<FunctionPtr(const std::string&)> creator = 
                [](const std::string& a0) -> auto {
                    return std::make_shared<Sin<float>>(a0); 
                };
                using ItemT = DataBaseItem<Function>;
                Get_SinRegistry().Register(std::make_shared<ItemT>("float", creator));
            }

            // Sin<double>
            {
                std::function<FunctionPtr(const std::string&)> creator = 
                [](const std::string& a0) -> auto {
                    return std::make_shared<Sin<double>>(a0); 
                };
                using ItemT = DataBaseItem<Function>;
                Get_SinRegistry().Register(std::make_shared<ItemT>("double", creator));
            }
        }
        
        // One arguments
        {
            // Pow<float>
            {
                std::function<FunctionPtr(const std::string&, double)> creator =
                [](const std::string& a0, double a1) -> auto {
                    return std::make_shared<Pow<float>>(a0, a1);
                };
                using ItemT = DataBaseItem<Function, double>;
                Get_PowRegistry().Register(std::make_shared<ItemT>("float", creator));
            }

            // Pow<double>
            // DEFINE_REGISTER_OF_DISTRIBUTED_REGISTRY(Pow, double, double)

        }

        // Two argments
        {
        }
    }

    // Binary ope func
    {
        // Zero arguments
        {
            // Add<float>
            {
                std::function<FunctionPtr(const std::string&)> creator = 
                [](const std::string& a0) -> auto {
                    return std::make_shared<Add<float>>(a0); 
                };
                using ItemT = DataBaseItem<Function>;
                Get_AddRegistry().Register(std::make_shared<ItemT>("float", creator));
            }

            // Add<double>
            {
                std::function<FunctionPtr(const std::string&)> creator = 
                [](const std::string& a0) -> auto {
                    return std::make_shared<Add<double>>(a0); 
                };
                using ItemT = DataBaseItem<Function>;
                Get_AddRegistry().Register(std::make_shared<ItemT>("double", creator));
            }
        }

        // One arguments
        {

        }

        // Two argments
        {
            
        }
    }
}