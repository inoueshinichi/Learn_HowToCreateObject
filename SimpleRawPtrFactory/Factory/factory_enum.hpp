/**
 * @file factory_enum.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <game_entity.hpp>
#include <game_camera.hpp>
#include <game_player.hpp>
#include <game_actor.hpp>
#include <game_enemy.hpp>

enum class EntityType
{
    ID_GameEntity = 0,
    ID_GameCamera,
    ID_GamePlayer,
    ID_GameActor,
    ID_GameEnemy,
};

GameEntity* EntityFactoryEnum(EntityType type)
{
    GameEntity* entity;
#define ENTITY_CASE_ENUM(TYPE, CLASS) \
    case EntityType::TYPE:            \
    {                                 \
        entity = new CLASS();         \
    }                                 \
    break;

    switch(type)
    {
        ENTITY_CASE_ENUM(ID_GameEntity, GameEntity)
        ENTITY_CASE_ENUM(ID_GameCamera, GameCamera)
        ENTITY_CASE_ENUM(ID_GamePlayer, GamePlayer)
        ENTITY_CASE_ENUM(ID_GameActor, GameActor)
        ENTITY_CASE_ENUM(ID_GameEnemy, GameEnemy)

        default: entity = nullptr;
    }

    return entity;
#undef ENTITY_CASE_ENUM
}