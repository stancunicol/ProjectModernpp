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

class EntityManager {
private:
    std::unordered_map<uint8_t, Player> m_players;
    std::unordered_map<int, Enemy> m_enemies;
    std::vector<Bullet> m_bullets;
    std::vector<Bomb> m_bombs;
    std::unordered_map<uint8_t, std::vector<Bullet>> m_playersBullets;

    std::vector<float> m_enemyShootTimers;
    const float m_enemyShootInterval = 0.3f;

    Base m_base;
    std::string m_winner;

    DataBase m_database;

    void HandleCollisions(GameMap& map);
public:
    EntityManager(GameMap& gameMap);

    void AddBomb(const Bomb& bomb);
    void AddPlayer(int id, const std::string& playerName, GameMap& map);
    void AddEnemy(int id, const Enemy& enemy);
    void AddBullet(const Bullet& bullet);
    void AddPlayerBullet(const Bullet& bullet, int playerId);
    void PlaceBase(GameMap& map);

    void RemoveBullet(size_t index);
    void RemovePlayer(size_t playerId);
    void RemoveEnemy(int id);
    void RemoveBomb(size_t index);


    void EnemyShoots(const Point& direction, const Point& position);
    void PlayerShoot(GameMap& map, int playerId);

    void ExplodeBomb(const Bomb& bomb, GameMap& map);

    void UpdateEntities(GameMap& map, float deltaTime);   // Updates the positions and states of all entities

    const std::unordered_map<uint8_t, Player>& GetPlayers() const;
    const std::unordered_map<int, Enemy>& GetEnemies() const;
    const std::vector<Bullet>& GetBullets() const;
    const std::vector<Bomb>& GetBombs() const;

    std::unordered_map<uint8_t, Player>& GetPlayersMutable();
    std::unordered_map<int, Enemy>& GetEnemiesMutable();
    std::vector<Bullet>& GetBulletsMutable();
    const Base& GetBase() const;
    std::string GetWinner() const;

    Player* GetPlayerById(int playerId);

    std::vector<uint8_t> GetPlayerIdsFromRoom(uint8_t playerId);

    bool PlayerExists(uint8_t userId) const;

    std::vector<std::tuple<Point, std::string>> GetAllPlayerPositions() const;

    void ResetPlayers(GameMap& map);

    void CloseRoom(const std::string& roomCode);

    void UpdateEnemyPositions(GameMap& map);
};

