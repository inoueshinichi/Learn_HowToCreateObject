/**
 * @file factory_key_string_value_fanctor_and_smt_ptr_with_interface.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
// Factory
#include <game_entity_factory.hpp>

// GameEntity
#include <game_entity.hpp>
#include <game_base_entity.hpp>
#include <game_camera.hpp>
#include <game_fps_camera.hpp>
#include <game_player.hpp>
#include <game_legend_player.hpp>
#include <game_actor.hpp>
#include <game_enemy.hpp>
#include <game_boss_enemy.hpp>
#include <game_final_boss_enemy.hpp>

// Interface
#include <interface_unknown.hpp>

#include <string>
#include <sstream>
#include <memory>
#include <vector>

int main(int, char**)
{
    try
    {
        GameEntityFactory factory;

        // Register
        std::cout << "Register" << std::endl;
        factory.Register<GameBaseEntity>(GameBaseEntity::GetClassName());
        factory.Register<GameCamera>(GameCamera::GetClassName());
        factory.Register<GameFpsCamera>(GameFpsCamera::GetClassName());
        factory.Register<GamePlayer>(GamePlayer::GetClassName());
        factory.Register<GameLegendPlayer>(GameLegendPlayer::GetClassName());
        factory.Register<GameActor>(GameActor::GetClassName());
        factory.Register<GameEnemy>(GameEnemy::GetClassName());
        factory.Register<GameBossEnemy>(GameBossEnemy::GetClassName());
        factory.Register<GameFinalBossEnemy>(GameFinalBossEnemy::GetClassName());
        std::cout << std::endl;

        // Create
        std::cout << "Create" << std::endl;
        std::vector<std::shared_ptr<GameEntity>> entities;
        entities.emplace_back(factory.CreateEntity("GameBaseEntity"));
        entities.emplace_back(factory.CreateEntity("GameCamera"));
        entities.emplace_back(factory.CreateEntity("GameFpsCamera"));
        entities.emplace_back(factory.CreateEntity("GamePlayer"));
        entities.emplace_back(factory.CreateEntity("GameLegendPlayer"));
        entities.emplace_back(factory.CreateEntity("GameActor"));
        entities.emplace_back(factory.CreateEntity("GameEnemy"));
        entities.emplace_back(factory.CreateEntity("GameBossEnemy"));
        entities.emplace_back(factory.CreateEntity("GameFinalBossEnemy"));
        std::cout << std::endl;

        // Check virtual function
        std::cout << "[Check virtual GetObjectName()]" << std::endl;
        for (auto& sp : entities)
        {
            std::cout << sp->GetObjectName() << std::endl;
        }
        std::cout << std::endl;

        // Check static function
        std::cout << "[Check static GetClassName()]" << std::endl;
        for (auto &sp : entities)
        {
            std::cout << sp->GetObjectName() << ": " << sp->GetClassName() << std::endl;
        }
        std::cout << std::endl;

        // Chech no static function
        std::cout << "[Check no static GetNoStaticClassName()]" << std::endl;
        for (auto &sp : entities)
        {
            std::cout << sp->GetObjectName() << ": " << sp->GetNoStaticClassName() << std::endl;
        }
        std::cout << std::endl;

        // Check static function address with polymorphism
        std::cout << "[Check static function address by polymorphism]" << std::endl;
        for (auto &sp : entities)
        {
            std::printf("%s: %p\n", sp->GetObjectName(), (void *)(sp->GetClassName));
        }

        // Check static variable with polymorphism
        std::cout << "[Check static sCheckStaticID with polymorphism]" << std::endl;
        for (auto& sp : entities)
        {
            std::cout << sp->GetObjectName() << ": " << sp->sCheckStaticID << std::endl;
        }

        // Check static variable by class name symbol
        std::cout << "[Check static sCheckStaticID by ClassName Symbol]" << std::endl;
        std::cout << "GameBaseEntity: " << GameBaseEntity::sCheckStaticID << std::endl;
        std::cout << "GameCamera: " << GameCamera::sCheckStaticID << std::endl;
        std::cout << "GameFpsCamera: " << GameFpsCamera::sCheckStaticID << std::endl;
        std::cout << "GamePlayer: " << GamePlayer::sCheckStaticID << std::endl;
        std::cout << "GameLegendPlayer: " << GameLegendPlayer::sCheckStaticID << std::endl;
        std::cout << "GameEnemy: " << GameEnemy::sCheckStaticID << std::endl;
        std::cout << "GameBossEnemy: " << GameBossEnemy::sCheckStaticID << std::endl;
        std::cout << "GameFinalBossEnemy: " << GameFinalBossEnemy::sCheckStaticID << std::endl;

        // Check static variable address with polymorphism
        std::cout << "[Check static sCheckStaticID address by polymorphism]" << std::endl;
        for (auto& sp : entities)
        {
            std::printf("%s: %p\n", sp->GetObjectName(), (void*)&(sp->sCheckStaticID));
        }

        /**
         * @brief Interface
         * @note Interface???????????????, ??????????????????????????????????????????????????????????????????????????????????????????????????????????????????.
         * @warning Interface?????????IUnknonw???????????????????????????????????????. (Interface???????????????????????????????????????????????????)
         * @warning Entity???????????????????????????????????????Interface?????????????????????????????????????????????, 
         * ??????????????????????????????Interface??????????????????????????????. ??????, ???????????????????????????????????????????????????????????????.
         * @note ????????????????????????????????????????????????(virtual func)????????????????????????????????????????????????, 
         * ????????????????????????virtual??????????????????final????????????.?????????, ????????????????????????????????????????????????????????????????????????.
         * 
         * @note Interface????????????????????????, Component???????????????????????????????????????????????????????????????.
         */
        /* Interface */
        std::shared_ptr<IUnknown> interface;

        std::ostringstream oss;
        oss << "Interface table\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "|                     | Attack | ExtraAttack | Defence | ExtraDefence | Escape | View | FpsView |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "| GameBase            |   x    |      x      |    x    |       x      |    x   |   x  |    x    |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "| GamePlayer          |   O    |      x      |    O    |       x      |    O   |   x  |    x    |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "| GameLegenedPlayer   |   O    |      O      |    O    |       x      |    x(O)|   x  |    x    |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "| GameActor           |   x    |      x      |    x    |       x      |    O   |   x  |    x    |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "| GameEnemy           |   x    |      x      |    O    |       x      |    O   |   x  |    x    |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "| GameBossEnemy       |   O    |      x      |    O    |       x      |    O   |   x  |    x    |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "| GameFinalBossEnemy  |   O    |      x      |    O    |       O      |    x(O)|   x  |    x    |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "| GameCamera          |   x    |      x      |    x    |       x      |    x   |   O  |    x    |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        oss << "| GameFpsCamera       |   x    |      x      |    x    |       x      |    x   |   O  |    O    |\n";
        oss << "-------------------------------------------------------------------------------------------------\n";
        std::cout << oss.str() << std::endl;


        // Check interface attack
        std::cout << "[Check interface attack]" << std::endl;
        for (auto &sp : entities)
        {
            if ((interface = sp->QueryInterface(IID::ID_Attack)) != nullptr)
            {
                std::static_pointer_cast<IAttack>(interface)->Attack();
            }
        }
        std::cout << std::endl;

        // Check interface defence
        std::cout << "[Check interface defence]" << std::endl;
        for (auto &sp : entities)
        {
            if ((interface = sp->QueryInterface(IID::ID_Defence)) != nullptr)
            {
                std::static_pointer_cast<IDefence>(interface)->Defence();
            }
        }
        std::cout << std::endl;

        // Check interface escape
        std::cout << "[Check interface escape]" << std::endl;
        for (auto &sp : entities)
        {
            if ((interface = sp->QueryInterface(IID::ID_Escape)) != nullptr)
            {
                std::static_pointer_cast<IEscape>(interface)->Escape();
            }
        }
        std::cout << std::endl;

        // Check interface view
        std::cout << "[Check interface view]" << std::endl;
        for (auto &sp : entities)
        {
            if ((interface = sp->QueryInterface(IID::ID_View)) != nullptr)
            {
                std::static_pointer_cast<IView>(interface)->View();
            }
        }
        std::cout << std::endl;

        // Check interface extra-attack
        std::cout << "[Check interface extra-attack]" << std::endl;
        for (auto &sp : entities)
        {
            if ((interface = sp->QueryInterface(IID::ID_ExtraAttack)) != nullptr)
            {
                std::static_pointer_cast<IExtraAttack>(interface)->ExtraAttack();
            }
        }
        std::cout << std::endl;

        // Check interface extra-defence
        std::cout << "[Check interface extra-defence]" << std::endl;
        for (auto &sp : entities)
        {
            if ((interface = sp->QueryInterface(IID::ID_ExtraDefence)) != nullptr)
            {
                std::static_pointer_cast<IExtraDefence>(interface)->ExtraDefence();
            }
        }
        std::cout << std::endl;

        // Check interface fps-vew
        std::cout << "[Check interface fps-vew]" << std::endl;
        for (auto &sp : entities)
        {
            if ((interface = sp->QueryInterface(IID::ID_FpsView)) != nullptr)
            {
                std::static_pointer_cast<IFpsView>(interface)->FpsView();
            }
        }
        std::cout << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
