/**
 * @file template_distributed_database.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <exception>
#include <sstream>
#include <string>

template <typename DataBaseItem>
class DataBase
{
    std::vector<std::shared_ptr<DataBaseItem> > mItems;

public:
    typedef std::vector<std::shared_ptr<DataBaseItem> > Items;

    ~DataBase() {}

    typename DataBaseItem::Creator Query(const std::string &type)
    {
        auto iter = std::find_if(mItems.begin(), 
                                 mItems.end(), 
                                 [type](const auto &p_item) -> bool 
                                 {
                                     return p_item->mType == type;
                                 });

        if (iter == mItems.end())
        {
            std::ostringstream oss;
            oss << "No database item with type: `" << type << "`";
            throw std::runtime_error(oss.str());
        }

        return (*iter)->mCreator;
    }

    void Registry(std::shared_ptr<DataBaseItem> item)
    {
        auto iter = std::find(mItems.begin(), mItems.end(), item);
        if (iter == mItems.end())
        {
            mItems.push_back(item);
        }
    }

    typename Items::size_type GetNumOfItems()
    {
        return mItems.size();
    }
};