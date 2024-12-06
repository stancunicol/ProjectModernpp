#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "GameMap.h"

class EntityManager {
public:
    EntityManager();

    void AddPlayer(const Player& player);
    void AddEnemy(const Enemy& enemy);
    void AddBullet(const Bullet& bullet);

    void RemoveBullet(size_t index);
    void RemovePlayer(size_t index);
    void RemoveEnemy(size_t index);


    void EnemyShoots(const Point& direction, const Point& position);
    void PlayerShoot(GameMap& map);
    void UpdateEntities(GameMap& map, float deltaTime);   // Updates the positions and states of all entities


    const std::vector<Player>& GetPlayers() const;
    const std::vector<Enemy>& GetEnemies() const;
    const std::vector<Bullet>& GetBullets() const;

    std::vector<Player>& GetPlayersMutable();

private:
    std::vector<Player> m_players;      
    std::vector<Enemy> m_enemies;       
    std::vector<Bullet> m_bullets; 

private:
    std::vector<float> m_enemyShootTimers; // Timpurile scurse pentru fiecare inamic
    const float m_enemyShootInterval = 0.3f; // Intervalul între trageri

    void HandleCollisions();

};

