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
#include <game_base_entity.hpp>
#include <interface_view.hpp>

class GameCamera : public GameBaseEntity, public IView
{
public:
    virtual ~GameCamera() {}
    GameCamera(const GameCamera &) = delete;
    GameCamera &operator=(const GameCamera &) = delete;
    GameCamera(GameCamera &&) = default;
    GameCamera &operator=(GameCamera &&) = default;

    static const char *GetClassName() { return "GameCamera"; }
    virtual const char *GetObjectName() const override { return GameCamera::GetClassName(); }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(ID_View, IView, GameCamera)
    QUERYINTERFACE_END

    virtual void View() override
    {
        std::cout << GetObjectName() << " View" << std::endl;
    }

    static int sCheckStaticID;

protected:
    friend class GameEntityFactory;
    GameCamera() = default;
};

int GameCamera::sCheckStaticID = 8;
