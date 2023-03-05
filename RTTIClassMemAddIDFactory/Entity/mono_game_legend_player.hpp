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
#include <mono_game_player.hpp>

#include <mono_rtti.hpp>

class GameMonoLegendPlayer final : public GameMonoPlayer
{
    MONO_RTTI_DECL
public:
    virtual ~GameMonoLegendPlayer() {}
    GameMonoLegendPlayer(const GameMonoLegendPlayer &) = delete;
    GameMonoLegendPlayer &operator=(const GameMonoLegendPlayer &) = delete;
    GameMonoLegendPlayer(GameMonoLegendPlayer &&) = default;
    GameMonoLegendPlayer &operator=(GameMonoLegendPlayer &&) = default;

    static const char *GetClassName() { return "GameMonoLegendPlayer"; }
    virtual const char *GetObjectName() const override { return GameMonoLegendPlayer::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoLegendPlayer() = default;
};

MONO_RTTI_IMPL(GameMonoLegendPlayer, GameMonoPlayer)