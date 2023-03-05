/**
 * @file game_fps_camera.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <mono_game_camera.hpp>

#include <mono_rtti.hpp>

class GameMonoFpsCamera final : public GameMonoCamera
{
    MONO_RTTI_DECL
public:
    virtual ~GameMonoFpsCamera() {}
    GameMonoFpsCamera(const GameMonoFpsCamera &) = delete;
    GameMonoFpsCamera &operator=(const GameMonoFpsCamera &) = delete;
    GameMonoFpsCamera(GameMonoFpsCamera &&) = default;
    GameMonoFpsCamera &operator=(GameMonoFpsCamera &&) = default;

    static const char *GetClassName() { return "GameMonoFpsCamera"; }
    virtual const char *GetObjectName() const override { return GameMonoFpsCamera::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoFpsCamera() = default;
};

MONO_RTTI_IMPL(GameMonoFpsCamera, GameMonoCamera)