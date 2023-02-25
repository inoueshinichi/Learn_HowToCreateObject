/**
 * @file factory_string.hpp
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

#include <string>

GameEntity *EntityFactoryString(const std::string &name)
{
    GameEntity *entity = nullptr;
#define ENTITY_CASE_STRING(CLASS)  \
    if (name == #CLASS)            \
    {                              \
        entity = new CLASS();      \
    }                              \
    
    ENTITY_CASE_STRING(GameEntity)
    ENTITY_CASE_STRING(GameCamera)
    ENTITY_CASE_STRING(GamePlayer)
    ENTITY_CASE_STRING(GameActor)
    ENTITY_CASE_STRING(GameEnemy)

    return entity;
#undef ENTITY_CASE_STRING
}