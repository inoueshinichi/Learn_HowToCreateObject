/**
 * @file factory_with_enum.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <exception>
#include <vector>
#include <chrono>

#include <Factory/factory_enum.hpp>
#include <GameEntity/game_entity.hpp>


/**
 * @brief Enumによるシンプルなファクトリ
 * 
 * @return int 
 */

int main(int, char**)
{
    try
    {
        std::vector<GameEntity*> entities;
        GameEntity* entity;

        auto start = std::chrono::high_resolution_clock::now();

        entity = EntityFactoryEnum(EntityType::ID_GameEntity);
        entities.push_back(entity);
        entity = EntityFactoryEnum(EntityType::ID_GameCamera);
        entities.push_back(entity);
        entity = EntityFactoryEnum(EntityType::ID_GamePlayer);
        entities.push_back(entity);
        entity = EntityFactoryEnum(EntityType::ID_GameActor);
        entities.push_back(entity);
        entity = EntityFactoryEnum(EntityType::ID_GameEnemy);
        entities.push_back(entity);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = end - start;
        double duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        std::cout << __func__ << ": " << duration_ns << "[ns]" << std::endl;

        for (auto ent : entities)
        {
            std::cout << ent->GetClassName() << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}