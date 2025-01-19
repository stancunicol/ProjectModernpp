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

class  Game 
{
private:
    GameMap m_map;

    EntityManager m_entityManager;

    DataBase m_database;

    RoomManager m_roomManager;
    
    std::unordered_map<std::string, Room> m_rooms;

    std::mutex m_roomMutex;

    std::mutex m_gameMutex;

public:

    Game();

    ~Game();

    void InitializeGame();

    void EndGame(const std::string& winner);

    void SetLevel(int level);


    bool CheckBulletCollision(const Bullet& bullet, Point& hitObjectPos, bool& isPlayerHit, bool& isEnemyHit);
    
    void DestroyWall(const Point& hitPosition);
    
    void UpdatePlayerMovements();
    
    std::vector<Point> GetUpdatedBombs();

    std::vector<Enemy> GetUpdateEnemies();


    std::string GenerateRoomCode();

    std::string CreateRoom();

    std::optional<std::string> JoinRoom(const std::string& code, int playerId);

    bool LeaveRoom(const std::string& code, const int& playerId);

    void CloseRoom(const std::string& code);

    RoomManager GetRoomManager();


    Room* GetRoom(const std::string& code);
    
    GameMap& GetMap();
    
    DataBase& GetDatabase();
    
    EntityManager& GetEntityManager();


    crow::json::wvalue TranformInJson();
    
    crow::json::wvalue GetGameStateAsJson();

    std::mutex& GetGameMutex();
};
