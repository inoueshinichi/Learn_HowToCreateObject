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

#include <template_distributed_registry.hpp>
#include <template_distributed_database.hpp>
#include <template_distributed_dbitem.hpp>


/* Generic Function Headers */
#include <function_sin.hpp>

#include <memory>
#include <functional>
#include <mutex>


void SystemInitForRegister()
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

    /* Register generic func as dbitem to template style distributed database. */
    using ItemT = DataBaseItem<Function>;

    // Unary ope func
    {
        // Zero arguments
        {
            // Sin<float>
            {
                std::function<std::shared_ptr<Function>()> creator = []() -> auto {
                    return std::make_shared<Sin<float>>(); };

                Get_SinRegistry().Register(std::make_shared<ItemT>(creator, "float"));
            }

            // Sin<double>
            {
                std::function<std::shared_ptr<Function>()> creator = []() -> auto {
                    return std::make_shared<Sin<double>>(); };

                Get_SinRegistry().Register(std::make_shared<ItemT>(creator, "double"));
            }
        }
        
        // One arguments
        {

        }

        // Two argments
        {

        }
    }

    // Binary ope func
    {
        // Zero arguments
        {

        }

        // One arguments
        {

        }

        // Two argments
        {
            
        }
    }
}