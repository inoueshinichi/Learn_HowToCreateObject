/**
 * @file game_boss_enemy.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <mono_game_enemy.hpp>

#include <mono_rtti.hpp>

class GameMonoBossEnemy : public GameMonoEnemy
{
    MONO_RTTI_DECL
public:
    virtual ~GameMonoBossEnemy() {}
    GameMonoBossEnemy(const GameMonoBossEnemy &) = delete;
    GameMonoBossEnemy &operator=(const GameMonoBossEnemy &) = delete;
    GameMonoBossEnemy(GameMonoBossEnemy &&) = default;
    GameMonoBossEnemy &operator=(GameMonoBossEnemy &&) = default;

    static const char *GetClassName() { return "GameMonoBossEnemy"; }
    virtual const char *GetObjectName() const override { return GameMonoBossEnemy::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoBossEnemy() = default;
};

MONO_RTTI_IMPL(GameMonoBossEnemy, GameMonoEnemy)