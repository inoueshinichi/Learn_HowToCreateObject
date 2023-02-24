/**
 * @file interface_unknown.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief Objectクラスの抽象インターフェースクラスの定義
 * @version 0.1
 * @date 2023-02-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <interface_id.hpp>

#include <memory>

class IUnknown
{
public:
    virtual ~IUnknown() {};
    //virtual std::shared_ptr<IUnknown> QueryInterface(IID iid) = 0;
};