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
#include <game_enemy.hpp>
#include <interface_attack.hpp>
#include <interface_escape.hpp>

class GameBossEnemy : public GameEnemy, public IAttack/*, public IDefence*/ /*, public IEscape*/
{
public:
    virtual ~GameBossEnemy() {}
    GameBossEnemy(const GameBossEnemy &) = delete;
    GameBossEnemy &operator=(const GameBossEnemy &) = delete;
    GameBossEnemy(GameBossEnemy &&) = default;
    GameBossEnemy &operator=(GameBossEnemy &&) = default;

    static const char *GetClassName() { return "GameBossEnemy"; }
    virtual const char *GetObjectName() const override { return GameBossEnemy::GetClassName(); }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(ID_Attack, IAttack, GameBossEnemy)
    QUERYINTERFACE_ADD(ID_Defence, IDefence, GameBossEnemy)
    QUERYINTERFACE_ADD(ID_Escape, IEscape, GameBossEnemy)
    QUERYINTERFACE_END

    virtual void Attack() override
    {
        std::cout << GetObjectName() << " Attack" << std::endl;
    }

    virtual void Defence() override
    {
        std::cout << GetObjectName() << " Defence" << std::endl;
    }

    /*virtual*/ void Escape() override final
    {
        std::cout << GetObjectName() << " Escape" << std::endl;
    }

    static int sCheckStaticID;

protected:
    friend class GameEntityFactory;
    GameBossEnemy() = default;
};

int GameBossEnemy::sCheckStaticID = 6;