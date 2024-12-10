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
    bool m_setLevel;
    std::mutex mutex;
    std::condition_variable condition;

    Game();
    ~Game();

    void InitializeGame(); //the game is initialed

    void Run(); //runs the game  

    void EndGame(const std::string& winner);

    void SetLevel(int level);

    crow::json::wvalue TranformInJson();

    crow::json::wvalue GetGameStateAsJson();
};
