/**
 * @file game_actor.hpp
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

class GameActor : public GameEntity
{
public:
    virtual ~GameActor() {}
    GameActor() = default;
    GameActor(const GameActor &) = delete;
    GameActor &operator=(const GameActor &) = delete;
    GameActor(GameActor &&) = default;
    GameActor &operator=(GameActor &&) = default;

    virtual const char *GetClassName() const { return "GameActor"; }
};