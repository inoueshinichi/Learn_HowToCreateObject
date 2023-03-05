/**
 * @file game_final_boss_enemy.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <mono_game_boss_enemy.hpp>

#include <mono_rtti.hpp>

class GameMonoFinalBossEnemy final : public GameMonoBossEnemy
{
    MONO_RTTI_DECL
public:
    virtual ~GameMonoFinalBossEnemy() {}
    GameMonoFinalBossEnemy(const GameMonoFinalBossEnemy &) = delete;
    GameMonoFinalBossEnemy &operator=(const GameMonoFinalBossEnemy &) = delete;
    GameMonoFinalBossEnemy(GameMonoFinalBossEnemy &&) = default;
    GameMonoFinalBossEnemy &operator=(GameMonoFinalBossEnemy &&) = default;

    static const char *GetClassName() { return "GameMonoFinalBossEnemy"; }
    virtual const char *GetObjectName() const override { return GameMonoFinalBossEnemy::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoFinalBossEnemy() = default;
};

MONO_RTTI_IMPL(GameMonoFinalBossEnemy, GameMonoBossEnemy)