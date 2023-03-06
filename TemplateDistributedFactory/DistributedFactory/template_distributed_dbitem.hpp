/**
 * @file template_distributed_dbitem.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <functional>
#include <string>

template <typename Base, typename... Args>
struct DataBaseItem
{
    typedef std::function< std::shared_ptr<Base> (Args...) > Creator;

    Creator mCreator; // functor for creation for derived class based on Base.
    std::string mType;

    DataBaseItem(const Creator& creator, const std::string& type) : mCreator(creator), mType(type) {}
};