/**
 * @file game_legend_player.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <game_player.hpp>
// #include <interface_attack.hpp>
#include <interface_defence.hpp>
#include <interface_extra_attack.hpp>

class GameLegendPlayer final : public GamePlayer, /*public IAttack, */ /*public IDefence,*/ public IExtraAttack
{
public:
    virtual ~GameLegendPlayer() {}
    GameLegendPlayer(const GameLegendPlayer &) = delete;
    GameLegendPlayer &operator=(const GameLegendPlayer &) = delete;
    GameLegendPlayer(GameLegendPlayer &&) = default;
    GameLegendPlayer &operator=(GameLegendPlayer &&) = default;

    static const char *GetClassName() { return "GameLegendPlayer"; }
    virtual const char *GetObjectName() const override { return GameLegendPlayer::GetClassName(); }
    const char* GetNoStaticClassName() { return "No static GameLegendPlayer"; }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(ID_Attack, IAttack, GameLegendPlayer)
    QUERYINTERFACE_ADD(ID_Defence, IDefence, GameLegendPlayer)
    QUERYINTERFACE_ADD(ID_ExtraAttack, IExtraAttack, GameLegendPlayer)
    QUERYINTERFACE_END

    void ExtraAttack() override final
    {
        std::cout << GetObjectName() << " ExtraAttack" << std::endl;
    }

    void Attack() override final
    {
        std::cout << GetObjectName() << " Attack" << std::endl;
    }

    void Defence() override final
    {
        std::cout << GetObjectName() << " Defence" << std::endl;
    }

    static int sCheckStaticID;

protected:
    friend class GameEntityFactory;
    GameLegendPlayer() = default;
};

int GameLegendPlayer::sCheckStaticID = 4;