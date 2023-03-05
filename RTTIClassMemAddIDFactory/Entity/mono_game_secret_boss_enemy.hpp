/**
 * @file game_secret_boss_enemy.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <mono_game_boss_enemy.hpp>

#include <mono_rtti.hpp>

class GameMonoSecretBossEnemy : public GameMonoBossEnemy
{
    MONO_RTTI_DECL
public:
    virtual ~GameMonoSecretBossEnemy() {}
    GameMonoSecretBossEnemy(const GameMonoSecretBossEnemy &) = delete;
    GameMonoSecretBossEnemy &operator=(const GameMonoSecretBossEnemy &) = delete;
    GameMonoSecretBossEnemy(GameMonoSecretBossEnemy &&) = default;
    GameMonoSecretBossEnemy &operator=(GameMonoSecretBossEnemy &&) = default;

    static const char *GetClassName() { return "GameMonoSecretBossEnemy"; }
    virtual const char *GetObjectName() const override { return GameMonoSecretBossEnemy::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoSecretBossEnemy() = default;
};

MONO_RTTI_IMPL(GameMonoSecretBossEnemy, GameMonoBossEnemy)