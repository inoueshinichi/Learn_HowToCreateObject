/**
 * @file game_enemy.hpp
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

class GameEnemy : public GameEntity
{
public:
    virtual ~GameEnemy() {}
    GameEnemy() = default;
    GameEnemy(const GameEnemy &) = delete;
    GameEnemy &operator=(const GameEnemy &) = delete;
    GameEnemy(GameEnemy &&) = default;
    GameEnemy &operator=(GameEnemy &&) = default;

    virtual const char *GetClassName() const { return "GameEnemy"; }
};