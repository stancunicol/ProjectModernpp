#pragma once
#include <mutex>
#include <condition_variable>
#include <vector>
#include <conio.h>
#include "Player.h"
#include "Base.h"
#include "Bullet.h"
#include "EntityManager.h"
#include "DataBase.h"
#include <crow.h>
#include <unordered_map>
#include "Room.h"


class  Game {
private:
    GameMap m_map;
    EntityManager m_entityManager;

    DataBase m_database;

    std::unordered_map<std::string, Room> m_rooms;

    std::string GenerateRoomCode();

    std::mutex roomMutex;

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

    DataBase& GetDatabase() { return m_database; };

    crow::json::wvalue TranformInJson();

    crow::json::wvalue GetGameStateAsJson();

    std::string CreateRoom();

    std::optional<std::string> JoinRoom(const std::string& code, const std::string& playerName);

    bool LeaveRoom(const std::string& code, const std::string& playerName);

    std::optional<Room> GetRoom(const std::string& code);
};
