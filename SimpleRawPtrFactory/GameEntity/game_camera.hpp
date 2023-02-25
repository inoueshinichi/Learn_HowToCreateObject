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
#include <game_entity.hpp>

class GameCamera : public GameEntity
{
public:
    virtual ~GameCamera() {}
    GameCamera() = default;
    GameCamera(const GameCamera &) = delete;
    GameCamera &operator=(const GameCamera &) = delete;
    GameCamera(GameCamera &&) = default;
    GameCamera &operator=(GameCamera &&) = default;

    virtual const char *GetClassName() const { return "GameCamera"; }
};
