#pragma once
import Base;

#include <mutex>
#include <crow.h>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <conio.h>
#include "DataBase.h"
#include "Player.h"
#include "Bullet.h"
#include "EntityManager.h"
#include "Room.h"

class  Game {
private:
    GameMap m_map;
    EntityManager m_entityManager;
    DataBase m_database;

    std::unordered_map<std::string, Room> m_rooms;

    std::string GenerateRoomCode();

    std::mutex roomMutex;

    std::mutex gameMutex;


public:

    Game();

    ~Game();

    void InitializeGame(); //the game is initialed

    void Run(); //runs the game  

    void EndGame(const std::string& winner);

    void SetLevel(int level);


    GameMap& GetMap() { return m_map; };
    DataBase& GetDatabase() { return m_database; };
    EntityManager& GetEntityManager() { return m_entityManager; };

    crow::json::wvalue TranformInJson();

    crow::json::wvalue GetGameStateAsJson();

    std::string CreateRoom();

    std::optional<std::string> JoinRoom(const std::string& code, const std::string& playerName);

    bool LeaveRoom(const std::string& code, const std::string& playerName);

    std::optional<Room> GetRoom(const std::string& code);

    void UpdatePlayerMovements();

    std::mutex& GetGameMutex();
};
