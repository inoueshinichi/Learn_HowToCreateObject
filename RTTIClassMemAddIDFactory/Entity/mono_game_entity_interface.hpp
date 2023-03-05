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

#include <mono_rtti.hpp>

class GameInterfaceEntity
{
    MONO_RTTI_DECL
public:
    virtual ~GameInterfaceEntity() {}  
    virtual std::shared_ptr<IUnknown> QueryInterface(IID iid) = 0;

    static const char *GetClassName() { return "GameInterfaceEntity"; }
    virtual const char*GetObjectName() const { return GameInterfaceEntity::GetClassName(); }
};

MONO_RTTI_IMPL_NOPARENT(GameInterfaceEntity)

/**
 * @brief Entityの継承図 (GameInterfaceEntity is interface for abstruct virtual func QueryInterface )
 * --------------------------------------------------------------------------------------------
 * GameEntity
 * --------------------------------------------------------------------------------------------
 * GameActor  | GamePlayer       | GameEnemy                                | GameCamera
 * --------------------------------------------------------------------------------------------
 *     -      | GameLegendPlayer | GameBossEnemy                            | GameFpsCamera
 * --------------------------------------------------------------------------------------------
 *     -      |         -        | GameFinalBossEnemy | GameSecretBossEnemy |
 * --------------------------------------------------------------------------------------------
 */