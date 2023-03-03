/**
 * @file game_entity.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <memory>
#include <interface_unknown.hpp>

class GameEntity
{
public:
    virtual ~GameEntity() {}  
    virtual std::shared_ptr<IUnknown> QueryInterface(IID iid) = 0;

    static const char *GetClassName() { return "GameEntity"; }
    virtual const char*GetObjectName() const { return GameEntity::GetClassName(); }
    const char *GetNoStaticClassName() { return "No static GameEntity"; }

    static int sCheckStaticID;
};

int GameEntity::sCheckStaticID = 0;