/**
 * @file test_interface_exe.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <interface_id.hpp>
#include <interface_unknown.hpp>
#include <interface_settable.hpp>
#include <interface_gettable.hpp>

#include <object.hpp>
#include <settable_object.hpp>
#include <gettable_object.hpp>
#include <get_set_able_object.hpp>

#include <iostream>
#include <exception>
#include <string>

#include <memory>
#include <vector>

/**
 * @brief std::shared_ptr<T>を用いたインターフェース実装
 *
 * @warning std::enable_shared_from_this<T>はObject派生クラスの基底であるObjectクラスのみが継承する.
 * @warning Object派生クラスは必ずstd::shared_ptrで管理する.
 * @warning Object派生クラスは外部からnew演算子でstd::shared_ptr管轄外のヒープを生成させないために, コンストラクタをprivateにしてオブジェクト生成クラスに生成を一任させる.(Factoryパターン)
 * @warning std::make_shared<T>()は内部ポインタがNULLでない. 内部がNULLのstd::shared_ptr生成はstd::shared_ptr<T>()とする.
 *
 * @note Objectクラスから派生クラスのSettableObject, GettableObject 等へのダウンキャストは shared_from_this()をstaticキャストしてよい.
 * @note インターフェースの基底はIUnknownにする.
 * @note Object基底クラスはIUnknownを継承しない.
 * @note Object基底クラスも派生クラスへのQueryInterface(IDD)の実装を強制させるため, 抽象基底クラスにする.
 * @note std::dynamic_cast, std::dynamic_pointer_castを使用しないため多分早い. (未検証)
 *
 * @return int
 */

int main(int, char **)
{
    try
    {
        /* code */
        std::cout << "Hello World" << std::endl;

        std::shared_ptr<SettableObject> ptr1 = std::make_shared<SettableObject>();

        std::cout << "[Check 1] ISettable " << std::static_pointer_cast<ISettable>(ptr1).use_count() << std::endl;

        std::shared_ptr<GettableObject> ptr2 = std::make_shared<GettableObject>();
        std::shared_ptr<GetSetAbleObject> ptr3 = std::make_shared<GetSetAbleObject>();

        std::cout << "[Check 2] ISettable " << std::static_pointer_cast<ISettable>(ptr1).use_count() << std::endl;

        std::vector<std::shared_ptr<Object>> ptr_vec;
        ptr_vec.emplace_back(ptr1);
        ptr_vec.emplace_back(ptr2);
        ptr_vec.emplace_back(ptr3);

        std::shared_ptr<IUnknown> interface;

        for (auto& ptr : ptr_vec)
        {
            if ((interface = ptr->QueryInterface(IID::ISETTABLE)) != nullptr)
            {
                std::cout << "I have ISettable: ";
                std::static_pointer_cast<ISettable>(interface)->Set(100);
                std::cout << "ISettable Set(100)" << std::endl;
            }

            if ((interface = ptr->QueryInterface(IID::IGETTABLE)) != nullptr)
            {
                std::cout << "I have IGettable: ";
                std::cout << "IGettable Get() " << std::static_pointer_cast<IGettable>(interface)->Get()
                          << std::endl;
            }

            if ((interface = ptr->QueryInterface(IID::ISETTABLE)) != nullptr &&
                ((interface = ptr->QueryInterface(IID::IGETTABLE)) != nullptr))
            {
                std::cout << "I have ISettable & IGettable: ";
                std::static_pointer_cast<ISettable>(interface)->Set(300);
                std::cout << "SetGetAbleObject Set(300) Get() "
                          << std::static_pointer_cast<IGettable>(interface)->Get() << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}