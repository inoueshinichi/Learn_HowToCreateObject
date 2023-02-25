/**
 * @file game_entity.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

class GameEntity
{
public:
    virtual ~GameEntity() {}
    GameEntity() = default;
    GameEntity(const GameEntity&) = delete;
    GameEntity& operator=(const GameEntity&) = delete;
    GameEntity(GameEntity&&) = default;
    GameEntity& operator=(GameEntity&&) = default;

    virtual const char* GetClassName() const { return "GameEntity"; }
};