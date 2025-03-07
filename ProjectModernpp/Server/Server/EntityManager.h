#pragma once
import Base;
import Bomb;

#include <thread>
#include <chrono>
#include <unordered_map>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "GameMap.h"
#include "DataBase.h"

class EntityManager 
{
private:
    std::unordered_map<uint8_t, Player> m_players;

    std::unordered_map<int, Enemy> m_enemies;

    std::unordered_map<uint8_t, std::vector<Bullet>> m_playersBullets;

    std::vector<Bullet> m_bullets;
    
    std::vector<Bomb> m_bombs;

    std::string m_winner;
    
    Base m_base;

    DataBase m_database;

public:
    EntityManager(GameMap& gameMap);

    void AddBomb(const Bomb& bomb);
    
    void AddPlayer(int id, const std::string& playerName, GameMap& map);
    
    void AddEnemy(int id, const Enemy& enemy);
    
    void AddBullet(const Bullet& bullet);
    
    void AddPlayerBullet(const Bullet& bullet, int playerId);
    
    void PlaceBase(GameMap& map);
    
    void RemovePlayer(size_t playerId);
    
    void RemoveEnemy(int id);
    
    void RemoveBomb(size_t index);

    void RemoveBullet(size_t index);

    void ExplodeBomb(const Bomb& bomb, GameMap& map);

   
    void HandleCollisions(GameMap& map);


    void ResetPlayers(GameMap& map);

    void ResetBombs();

    void ResetBullets();

    void CloseRoom(const std::string& roomCode);

    void UpdateEnemyPositions(GameMap& map);


    std::unordered_map<uint8_t, std::vector<Bullet>>& GetPlayersBullets();

    const std::unordered_map<uint8_t, Player>& GetPlayers() const;
    
    const std::unordered_map<int, Enemy>& GetEnemies() const;
    
    const std::vector<Bullet>& GetBullets() const;
    
    const std::vector<Bomb>& GetBombs() const;

    std::unordered_map<uint8_t, Player>& GetPlayersMutable();
    
    const Base& GetBase() const;
    
    std::string GetWinner() const;

    std::vector<uint8_t> GetPlayerIdsFromRoom(uint8_t playerId);

    std::vector<std::tuple<Point, std::string>> GetAllPlayerPositions() const;
};

