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
#include <game_base_entity.hpp>
#include <interface_escape.hpp>

class GameActor : public GameBaseEntity, public IEscape
{
public:
    virtual ~GameActor() {}
    GameActor(const GameActor &) = delete;
    GameActor &operator=(const GameActor &) = delete;
    GameActor(GameActor &&) = default;
    GameActor &operator=(GameActor &&) = default;

    static const char *GetClassName() { return "GameActor"; }
    virtual const char *GetObjectName() const override { return GameActor::GetClassName(); }
    const char* GetNoStaticClassName() { return "No static GameActor"; }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_ADD(ID_Escape, IEscape, GameActor)
    QUERYINTERFACE_END

    void Escape() override final
    {
        std::cout << GetObjectName() << " Escape" << std::endl;
    }

    static int sCheckStaticID;

protected:
    friend class GameEntityFactory;
    GameActor() = default;
};

int GameActor::sCheckStaticID = 2;