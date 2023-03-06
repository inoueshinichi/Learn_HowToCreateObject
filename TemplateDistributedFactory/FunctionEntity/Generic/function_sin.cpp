/**
 * @file function_sin.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <function_sin.hpp>
#include <singleton_manager-internal.hpp>

#include <system_init_for_register.hpp>


/* Get_SinRegistry */
Registry<Function>& Get_SinRegistry()
{
    struct SinRegistryHolder // Get_SinRegistry と SinRegistryHolder で オブジェクトIDを実現している
    {
        Registry<Function> instance;
        SinRegistryHolder() {}
    };
    return SingletonManager::Get<SinRegistryHolder>()->instance;
}

/* Create_Sin */
std::shared_ptr<Function> Create_Sin(const std::string &type)
{
    SystemInitForRegister();
    return Get_SinRegistry().Create(type); // new Sin class as std::shared_ptr<Sin>
}