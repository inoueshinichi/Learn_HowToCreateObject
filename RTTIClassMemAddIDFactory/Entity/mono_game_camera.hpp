/**
 * @file game_camera.hpp
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

class GameMonoCamera : public GameMonoEntity
{
    MONO_RTTI_DECL
public:
    virtual ~GameMonoCamera() {}
    GameMonoCamera(const GameMonoCamera &) = delete;
    GameMonoCamera &operator=(const GameMonoCamera &) = delete;
    GameMonoCamera(GameMonoCamera &&) = default;
    GameMonoCamera &operator=(GameMonoCamera &&) = default;

    static const char *GetClassName() { return "GameMonoCamera"; }
    virtual const char *GetObjectName() const override { return GameMonoCamera::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoCamera() = default;
};

MONO_RTTI_IMPL(GameMonoCamera, GameMonoEntity)