#pragma once
#include <vector>
#include <conio.h>
#include "Player.h"
#include "Base.h"
#include "Bullet.h"
#include "EntityManager.h"
#include <crow.h>


class  Game {
private:
    GameMap m_map;
    EntityManager m_entityManager;

public:
    Game(uint32_t width, uint32_t height, uint8_t level);

    ~Game(); 

    void InitializeGame(); //the game is initialed

    void Run();//runs the game  

    crow::json::wvalue TranformInJson();

    crow::json::wvalue GetGameStateAsJson();
};
