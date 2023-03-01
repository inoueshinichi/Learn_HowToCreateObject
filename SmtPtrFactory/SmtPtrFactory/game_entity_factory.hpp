/**
 * @file game_entity_factory.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <memory>
#include <map>
#include <functional>
#include <iostream>

#include <game_base_entity.hpp>

// enum class EntityType
// {
//     ID_GameBaseEntity = 0,
//     ID_GameCamera,
//     ID_GameFpsCamera,
//     ID_GamePlayer,
//     ID_GameLegendPlayer,
//     ID_GameActor,
//     ID_GameEnemy,
//     ID_GameBossEnemy,
//     ID_GameFinalEnemy,
// };

// inline std::string GetEntityTypeString(EntityType type_id)
// {
//     std::string name;
// #define GEN_STRING(enitiy_type)           \
//     case EntityType::entity_type:         \
//     {                                     \
//         name = #entity_type;              \
//     }                                     \
//     break;

//     switch(type_id)
//     {
//         GEN_STRING(ID_GameBaseEntity)
//         GEN_STRING(ID_GameCamera)
//         GEN_STRING(ID_GameFpsCamera)
//         GEN_STRING(ID_GamePlayer)
//         GEN_STRING(ID_GameLegendPlayer)
//         GEN_STRING(ID_GameActor)
//         GEN_STRING(ID_GameEnemy)
//         GEN_STRING(ID_GameBossEnemy)
//         GEN_STRING(ID_GameFinalEnemy)
//     default:
//         name = "";
//     }

//     return name;
// #undef GEN_STRING
// }

// #define ENTITY_STRING(entity) #entity
#include <type_traits>


// Singletonが望ましい
class GameEntityFactory
{
public:
    GameEntityFactory() = default;
    ~GameEntityFactory() = default;
    GameEntityFactory(const GameEntityFactory &) = delete;
    GameEntityFactory &operator=(const GameEntityFactory &) = delete;
    GameEntityFactory(GameEntityFactory &&) = default;
    GameEntityFactory &operator=(GameEntityFactory &&) = default;

    std::shared_ptr<GameBaseEntity> CreateEntity(const std::string& type)
    {
        auto iter = mRegistry.find(type);
        if (iter == mRegistry.end())
        {
            return std::shared_ptr<GameBaseEntity>(); // null. nullはsegmentationfaultになってデバッグしずらい
        }
        return (*iter).second(); // creator() -> std::shared_ptr<Derived>
    }

    // std::is_comvertible_v : >= C++17
    template <typename ENTITY, typename std::enable_if_t<std::is_convertible_v<ENTITY, GameBaseEntity>> * = nullptr>
    void Register(/*EntityType*/ const std::string &type)
    {
        auto iter = mRegistry.find(type);
        if (iter != mRegistry.end())
        {
            return;
        }

        std::cout << "Register creator of " << /*GetEntityTypeString(type)*/ /*ENTITY_STRING(ENTITY)*/ ENTITY::GetClassName() << std::endl;

        auto deleter = [](ENTITY *ptr) -> void {
            std::cout << "[Delete] " << /*ENTITY_STRING(ENTITY)*/ ENTITY::GetClassName() << std::endl;
            delete ptr;
            ptr = nullptr;
        };

        auto creator = [deleter]() -> std::shared_ptr<GameBaseEntity> {
            std::cout << "[Create] " << /*ENTITY_STRING(ENTITY)*/ ENTITY::GetClassName() << std::endl;
            return std::shared_ptr<ENTITY>(new ENTITY(), deleter);
        };

        mRegistry.emplace(type, creator); // 登録

// #define ENTITY_CREATOR(entity_type, class_name)                                      \
//     case EntityType::entity_type:                                                    \
//     {                                                                                \
//         auto deleter = [entity_type](class_name *ptr) -> void {                      \
//             std::cout << "[Delete] " <<                                              \
//                 GetEntityTypeString(entity_type) <<                                  \
//                     std::endl;                                                       \
//             delete ptr;                                                              \
//             ptr = nullptr;                                                           \
//         };                                                                           \
//         auto creator = [entity_type, deleter]() -> std::shaed_ptr<GameBaseEntity> {  \
//             std::cout << "[Create] " <<                                              \
//                 GetEntityTypeString(entity_type) <<                                  \
//                     std::endl;                                                       \
//             return std::shared_ptr<class_name>(                                      \
//                 new class_name(), deleter);                                          \
//         };                                                                           \
//         mRegistry.emplace(entity_type, creator);                                     \
//     }                                                                                \
//     break;

    // switch (type)
    // {
    //         ENTITY_CREATOR(ID_GameBaseEntity, GameBaseEntity)
    //         ENTITY_CREATOR(ID_GameCamera, GameCamera)
    //         ENTITY_CREATOR(ID_GameFpsCamera, GameFpsCamera)
    //         ENTITY_CREATOR(ID_GamePlayer, GamePlayer)
    //         ENTITY_CREATOR(ID_GameLegendPlayer, GameLegendPlayer)
    //         ENTITY_CREATOR(ID_GameActor, GameActor)
    //         ENTITY_CREATOR(ID_GameEnemy, GameEnemy)
    //         ENTITY_CREATOR(ID_GameBossEnemy, GameBossEnemy)
    //         ENTITY_CREATOR(ID_GameFinalBossEnemy, GameFinalBossEnemy)
    // }

// #undef ENTITY_CREATOR
    }

private :
    typedef std::map </*EntityType*/std::string, std::function<std::shared_ptr<GameBaseEntity>()>> CreatorMap;
    CreatorMap mRegistry;
};
