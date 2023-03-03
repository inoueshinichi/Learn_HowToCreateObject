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
#include <game_camera.hpp>
#include <interface_fps_view.hpp>

class GameFpsCamera final : public GameCamera, public IFpsView
{
public:
    virtual ~GameFpsCamera() {}
    GameFpsCamera(const GameFpsCamera &) = delete;
    GameFpsCamera &operator=(const GameFpsCamera &) = delete;
    GameFpsCamera(GameFpsCamera &&) = default;
    GameFpsCamera &operator=(GameFpsCamera &&) = default;

    static const char *GetClassName() { return "GameFpsCamera"; }
    virtual const char *GetObjectName() const override { return GameFpsCamera::GetClassName(); }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(ID_View, IView, GameFpsCamera)
    QUERYINTERFACE_ADD(ID_FpsView, IFpsView, GameFpsCamera)
    QUERYINTERFACE_END

    void View() override final
    {
        std::cout << GetObjectName() << " View" << std::endl;
    }

    void FpsView() override final
    {
        std::cout << GetObjectName() << " FpsView" << std::endl;
    }

    static int sCheckStaticID;

protected:
    friend class GameEntityFactory;
    GameFpsCamera() = default;
};

int GameFpsCamera::sCheckStaticID = 9;