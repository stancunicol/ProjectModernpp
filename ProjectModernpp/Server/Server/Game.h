#pragma once
import Base;

#include <mutex>
#include <crow.h>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <cstdint>
#include <conio.h>
#include "DataBase.h"
#include "Player.h"
#include "Bullet.h"
#include "EntityManager.h"
#include "Room.h"
#include "RoomManager.h"

class  Game {
private:
    GameMap m_map;
    EntityManager m_entityManager;
    DataBase m_database;

    std::unordered_map<std::string, Room> m_rooms;

    std::mutex m_roomMutex;

    std::mutex m_gameMutex;

    RoomManager m_roomManager;

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

    std::optional<std::string> JoinRoom(const std::string& code, int playerId);

    bool LeaveRoom(const std::string& code, const int& playerId);

    Room* GetRoom(const std::string& code);

    void UpdatePlayerMovements();

    std::mutex& GetGameMutex();

    std::string GenerateRoomCode();

    RoomManager GetRoomManager();

    void CloseRoom(const std::string& code);

    void UpdateEnemyPositions();
};
