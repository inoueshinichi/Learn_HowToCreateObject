/**
 * @file game_actor.hpp
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

class GameMonoActor : public GameMonoEntity
{
    MONO_RTTI_DECL
public:
    virtual ~GameMonoActor() {}
    GameMonoActor(const GameMonoActor &) = delete;
    GameMonoActor &operator=(const GameMonoActor &) = delete;
    GameMonoActor(GameMonoActor &&) = default;
    GameMonoActor &operator=(GameMonoActor &&) = default;

    static const char *GetClassName() { return "GameMonoActor"; }
    virtual const char *GetObjectName() const override { return GameMonoActor::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoActor() = default;
};

MONO_RTTI_IMPL(GameMonoActor, GameMonoEntity)