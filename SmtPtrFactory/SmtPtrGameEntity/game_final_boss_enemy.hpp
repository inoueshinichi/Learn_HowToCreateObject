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
#include <game_boss_enemy.hpp>
#include <interface_attack.hpp>
// #include <interface_defence.hpp>
#include <interface_extra_defence.hpp>

class GameFinalBossEnemy final : public GameBossEnemy, /*public IAttack,*/ /*public IDefence,*/ public IExtraDefence
{
public:
    virtual ~GameFinalBossEnemy() {}
    GameFinalBossEnemy(const GameFinalBossEnemy &) = delete;
    GameFinalBossEnemy &operator=(const GameFinalBossEnemy &) = delete;
    GameFinalBossEnemy(GameFinalBossEnemy &&) = default;
    GameFinalBossEnemy &operator=(GameFinalBossEnemy &&) = default;

    static const char *GetClassName() { return "GameFinalBossEnemy"; }
    virtual const char *GetObjectName() const override { return GameFinalBossEnemy::GetClassName(); }
    const char* GetNoStaticClassName() { return "No static FinalBossEnemy"; }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(ID_Attack, IAttack, GameFinalBossEnemy)
    QUERYINTERFACE_ADD(ID_Defence, IDefence, GameFinalBossEnemy)
    QUERYINTERFACE_ADD(ID_ExtraDefence, IExtraDefence, GameFinalBossEnemy)
    QUERYINTERFACE_END

    void Attack() override final
    {
        std::cout << GetObjectName() << " Attack" << std::endl;
    }

    void Defence() override final
    {
        std::cout << GetObjectName() << " Defence" << std::endl;
    }

    void ExtraDefence() override final
    {
        std::cout << GetObjectName() << " ExtraDefence" << std::endl;
    }

    static int sCheckStaticID;

protected:
    friend class GameEntityFactory;
    GameFinalBossEnemy() = default;
};

int GameFinalBossEnemy::sCheckStaticID = 7;