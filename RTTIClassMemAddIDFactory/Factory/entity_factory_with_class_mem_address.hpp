/**
 * @file entity_factory_with_class_mem_address.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <memory>
#include <map>
#include <functional>
#include <iostream>
#include <type_traits>
#include <cstdint> // std::intptr_t

#include <mono_game_entity.hpp>

// Singletonが望ましい
class GameMonoEntityFactoryWithClassMemAdd
{
public:
    GameMonoEntityFactoryWithClassMemAdd() = default;
    ~GameMonoEntityFactoryWithClassMemAdd() = default;
    GameMonoEntityFactoryWithClassMemAdd(const GameMonoEntityFactoryWithClassMemAdd &) = delete;
    GameMonoEntityFactoryWithClassMemAdd &operator=(const GameMonoEntityFactoryWithClassMemAdd &) = delete;
    GameMonoEntityFactoryWithClassMemAdd(GameMonoEntityFactoryWithClassMemAdd &&) = default;
    GameMonoEntityFactoryWithClassMemAdd &operator=(GameMonoEntityFactoryWithClassMemAdd &&) = default;

    std::shared_ptr<GameMonoEntity> CreateEntity(std::intptr_t rtti_address)
    {
        auto iter = mRegistry.find(rtti_address);
        if (iter == mRegistry.end())
        {
            return std::shared_ptr<GameMonoEntity>(); // null. nullはsegmentationfaultになってデバッグしずらい
        }
        return (*iter).second(); // creator() -> std::shared_ptr<Derived>
    }

    // std::is_comvertible_v : >= C++17
    template <typename ENTITY, typename std::enable_if_t<std::is_convertible_v<ENTITY, GameMonoEntity>> * = nullptr>
    void Register(std::intptr_t rtti_address)
    {
        auto iter = mRegistry.find(rtti_address);
        if (iter != mRegistry.end())
        {
            return;
        }

        std::cout << "Register creator of " << ENTITY::GetClassName() << std::endl;

        auto deleter = [](ENTITY *ptr) -> void {
            std::cout << "[Delete] " << ENTITY::GetClassName() << std::endl;
            if (ptr) 
            {
                delete ptr;
                ptr = nullptr;
            }
        };

        auto creator = [deleter]() -> std::shared_ptr<GameMonoEntity> {
            std::cout << "[Create] " << ENTITY::GetClassName() << std::endl;
            return std::shared_ptr<ENTITY>(new ENTITY(), deleter);
        };

        mRegistry.emplace(rtti_address, creator); // 登録
    }

private :
    typedef std::map<std::intptr_t, std::function<std::shared_ptr<GameMonoEntity>()>> CreatorMap;
    CreatorMap mRegistry;
};
