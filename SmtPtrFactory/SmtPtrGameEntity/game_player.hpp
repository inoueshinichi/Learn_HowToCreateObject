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
#include <game_base_entity.hpp>
#include <interface_attack.hpp>
#include <interface_defence.hpp>
#include <interface_escape.hpp>

class GamePlayer : public GameBaseEntity, public IAttack, public IDefence, public IEscape
{
public:
    virtual ~GamePlayer() {}
    GamePlayer(const GamePlayer &) = delete;
    GamePlayer &operator=(const GamePlayer &) = delete;
    GamePlayer(GamePlayer &&) = default;
    GamePlayer &operator=(GamePlayer &&) = default;

    static const char *GetClassName() { return "GamePlayer"; }
    virtual const char *GetObjectName() const override { return GamePlayer::GetClassName(); }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(ID_Attack, IAttack, GamePlayer)
    QUERYINTERFACE_ADD(ID_Defence, IDefence, GamePlayer)
    QUERYINTERFACE_ADD(ID_Escape, IEscape, GamePlayer)
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

protected:
    friend class GameEntityFactory;
    GamePlayer() = default;
};