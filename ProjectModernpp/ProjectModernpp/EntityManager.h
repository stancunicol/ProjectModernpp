#pragma once
#include <thread>
#include <chrono>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "GameMap.h"
#include "Bomb.h"
#include "Base.h"
#include "DataBase.h"

class EntityManager {
private:
    std::vector<Player> m_players;
    std::vector<Enemy> m_enemies;
    std::vector<Bullet> m_bullets;
    std::vector<Bullet> m_playerBullets;
    std::vector<Bomb> m_bombs;

    std::vector<float> m_enemyShootTimers; // Timpurile scurse pentru fiecare inamic
    const float m_enemyShootInterval = 0.3f; // Intervalul între trageri

    Base m_base;
    std::string m_winner;

    DataBase m_database;

    void HandleCollisions(GameMap& map);
public:
    EntityManager(GameMap& gameMap);

    void AddBomb(const Bomb& bomb);
    void AddPlayer(const Player& player);
    void AddEnemy(const Enemy& enemy);
    void AddBullet(const Bullet& bullet);
    void AddPlayerBullet(const Bullet& bullet);
    void PlaceBase(GameMap& map);

    void RemoveBullet(size_t index);
    void RemovePlayer(size_t index);
    void RemoveEnemy(size_t index);
    void RemoveBomb(size_t index);


    void EnemyShoots(const Point& direction, const Point& position);
    void PlayerShoot(GameMap& map);

    void ExplodeBomb(const Bomb& bomb, GameMap& map);

    void UpdateEntities(GameMap& map, float deltaTime);   // Updates the positions and states of all entities

    const std::vector<Player>& GetPlayers() const;
    const std::vector<Enemy>& GetEnemies() const;
    const std::vector<Bullet>& GetBullets() const;

    std::vector<Player>& GetPlayersMutable();
    std::vector<Enemy>& GetEnemiesMutable();
    std::vector<Bullet>& GetBulletsMutable();
    const Base& GetBase() const;
    std::string GetWinner() const;

};

