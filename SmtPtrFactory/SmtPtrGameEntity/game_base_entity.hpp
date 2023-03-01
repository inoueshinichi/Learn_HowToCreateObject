/**
 * @file game_base_entity.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <game_entity.hpp>

class GameBaseEntity : public std::enable_shared_from_this<GameBaseEntity>, public GameEntity
{
public:
    virtual ~GameBaseEntity() {}
    GameBaseEntity(const GameBaseEntity &) = delete;
    GameBaseEntity &operator=(const GameBaseEntity &) = delete;
    GameBaseEntity(GameBaseEntity &&) = default;
    GameBaseEntity &operator=(GameBaseEntity &&) = default;

    static const char *GetClassName() { return "GameBaseEntity"; }
    virtual const char*GetObjectName() const override { return GameBaseEntity::GetClassName(); }

    QUERYINTERFACE_BEGIN
    QUERYINTERFACE_END

protected:
    friend class GameEntityFactory;
    GameBaseEntity() = default;
};