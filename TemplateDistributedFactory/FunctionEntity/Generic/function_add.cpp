/**
 * @file function_add.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <function_add.hpp>

#include <singleton_manager-internal.hpp>
#include <system_init_for_register.hpp>

/* Get_AddRegistry */
Registry<Function> &Get_AddRegistry()
{
    struct AddRegistryHolder
    {
        Registry<Function> instance;
    };
    return SingletonManager::Get<AddRegistryHolder>()->instance;
}

/* Create_Add */
FunctionPtr Create_Add(const std::string &type)
{
    SystemInitForRegister();
    return Get_AddRegistry().Create(type);
}
