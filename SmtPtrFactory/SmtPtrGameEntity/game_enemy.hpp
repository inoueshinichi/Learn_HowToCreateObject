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
#include <game_base_entity.hpp>
#include <interface_escape.hpp>
#include <interface_defence.hpp>

class GameEnemy : public GameBaseEntity, public IEscape, public IDefence
{
public:
    virtual ~GameEnemy() {}
    GameEnemy(const GameEnemy &) = delete;
    GameEnemy &operator=(const GameEnemy &) = delete;
    GameEnemy(GameEnemy &&) = default;
    GameEnemy &operator=(GameEnemy &&) = default;

    static const char *GetClassName() { return "GameEnemy"; }
    virtual const char *GetObjectName() const override { return GameEnemy::GetClassName(); }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(ID_Escape, IEscape, GameEnemy)
    QUERYINTERFACE_ADD(ID_Defence, IDefence, GameEnemy)
    QUERYINTERFACE_END

    /*virutal*/ void Escape() override /*final*/
    {
        std::cout << GetObjectName() << " Escape" << std::endl;
    }

    /*virtual*/ void Defence() override
    {
        std::cout << GetObjectName() << " Defence" << std::endl;
    }

    static int sCheckStaticID;

protected:
    friend class GameEntityFactory;
    GameEnemy() = default;
};

int GameEnemy::sCheckStaticID = 5;