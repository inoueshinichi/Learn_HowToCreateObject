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
#include <mono_game_entity.hpp>

#include <mono_rtti.hpp>

class GameMonoEnemy : public GameMonoEntity
{
    MONO_RTTI_DECL
public:
    virtual ~GameMonoEnemy() {}
    GameMonoEnemy(const GameMonoEnemy &) = delete;
    GameMonoEnemy &operator=(const GameMonoEnemy &) = delete;
    GameMonoEnemy(GameMonoEnemy &&) = default;
    GameMonoEnemy &operator=(GameMonoEnemy &&) = default;

    static const char *GetClassName() { return "GameMonoEnemy"; }
    virtual const char *GetObjectName() const override { return GameMonoEnemy::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoEnemy() = default;
};

MONO_RTTI_IMPL(GameMonoEnemy, GameMonoEntity)