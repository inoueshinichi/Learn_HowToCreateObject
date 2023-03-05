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
#include <mono_game_entity.hpp>

#include <mono_rtti.hpp>

class GameMonoPlayer : public GameMonoEntity
{
    MONO_RTTI_DECL
public:
    virtual ~GameMonoPlayer() {}
    GameMonoPlayer(const GameMonoPlayer &) = delete;
    GameMonoPlayer &operator=(const GameMonoPlayer &) = delete;
    GameMonoPlayer(GameMonoPlayer &&) = default;
    GameMonoPlayer &operator=(GameMonoPlayer &&) = default;

    static const char *GetClassName() { return "GameMonoPlayer"; }
    virtual const char *GetObjectName() const override { return GameMonoPlayer::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoPlayer() = default;
};

MONO_RTTI_IMPL(GameMonoPlayer, GameMonoEntity)