/**
 * @file game_entity.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-03-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <iostream>

#include <mono_rtti.hpp>

class GameMonoEntity : public std::enable_shared_from_this<GameMonoEntity>
{
    MONO_RTTI_NO_PARENT_DECL
public:
    virtual ~GameMonoEntity() {}
    GameMonoEntity(const GameMonoEntity &) = delete;
    GameMonoEntity &operator=(const GameMonoEntity &) = delete;
    GameMonoEntity(GameMonoEntity &&) = default;
    GameMonoEntity &operator=(GameMonoEntity &&) = default;

    static const char *GetClassName() { return "GameMonoEntity"; }
    virtual const char*GetObjectName() const { return GameMonoEntity::GetClassName(); }

protected:
    friend class GameMonoEntityFactoryWithClassMemAdd;
    GameMonoEntity() = default;
};

MONO_RTTI_IMPL_NOPARENT(GameMonoEntity)
