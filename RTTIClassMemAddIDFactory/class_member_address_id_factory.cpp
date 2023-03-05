/**
 * @file class_member_address_id_factory.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>

// Mono inhereted entities
#include <mono_game_entity.hpp>
#include <mono_game_actor.hpp>
#include <mono_game_player.hpp>
#include <mono_game_legend_player.hpp>
#include <mono_game_enemy.hpp>
#include <mono_game_boss_enemy.hpp>
#include <mono_game_final_boss_enemy.hpp>
#include <mono_game_secret_boss_enemy.hpp>
#include <mono_game_camera.hpp>
#include <mono_game_fps_camera.hpp>

// Class mem address base factory
#include <entity_factory_with_class_mem_address.hpp>

int main(int, char**)
{
    try
    {
        GameMonoEntityFactoryWithClassMemAdd factory;

        // Register
        factory.Register<GameMonoEntity>(reinterpret_cast<std::intptr_t>(&GameMonoEntity::sMonoRtti));
        factory.Register<GameMonoActor>(reinterpret_cast<std::intptr_t>(&GameMonoActor::sMonoRtti));
        factory.Register<GameMonoPlayer>(reinterpret_cast<std::intptr_t>(&GameMonoPlayer::sMonoRtti));
        factory.Register<GameMonoLegendPlayer>(reinterpret_cast<std::intptr_t>(&GameMonoLegendPlayer::sMonoRtti));
        factory.Register<GameMonoEnemy>(reinterpret_cast<std::intptr_t>(&GameMonoEnemy::sMonoRtti));
        factory.Register<GameMonoBossEnemy>(reinterpret_cast<std::intptr_t>(&GameMonoBossEnemy::sMonoRtti));
        factory.Register<GameMonoFinalBossEnemy>(reinterpret_cast<std::intptr_t>(&GameMonoFinalBossEnemy::sMonoRtti));
        factory.Register<GameMonoSecretBossEnemy>(reinterpret_cast<std::intptr_t>(&GameMonoSecretBossEnemy::sMonoRtti));
        factory.Register<GameMonoCamera>(reinterpret_cast<std::intptr_t>(&GameMonoCamera::sMonoRtti));
        factory.Register<GameMonoFpsCamera>(reinterpret_cast<std::intptr_t>(&GameMonoFpsCamera::sMonoRtti));

        // Create
        std::cout << "Create" << std::endl;
        std::vector<std::shared_ptr<GameMonoEntity>> entities;
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoEntity::sMonoRtti)));
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoActor::sMonoRtti)));
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoPlayer::sMonoRtti)));
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoLegendPlayer::sMonoRtti)));
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoEnemy::sMonoRtti)));
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoBossEnemy::sMonoRtti)));
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoFinalBossEnemy::sMonoRtti)));
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoSecretBossEnemy::sMonoRtti)));
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoCamera::sMonoRtti)));
        entities.emplace_back(factory.CreateEntity(reinterpret_cast<std::intptr_t>(&GameMonoFpsCamera::sMonoRtti)));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}