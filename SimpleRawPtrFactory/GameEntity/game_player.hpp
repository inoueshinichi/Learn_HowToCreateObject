/**
 * @file game_player.hpp
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

class GamePlayer : public GameEntity
{
public:
    virtual ~GamePlayer() {}
    GamePlayer() = default;
    GamePlayer(const GamePlayer &) = delete;
    GamePlayer &operator=(const GamePlayer &) = delete;
    GamePlayer(GamePlayer &&) = default;
    GamePlayer &operator=(GamePlayer &&) = default;

    virtual const char *GetClassName() const { return "GamePlayer"; }
};