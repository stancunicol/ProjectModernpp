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

private:
    std::vector<Player> m_players;      
    std::vector<Enemy> m_enemies;       
    std::vector<Bullet> m_bullets; 

private:
    std::vector<float> m_enemyShootTimers; // Timpurile scurse pentru fiecare inamic
    const float m_enemyShootInterval = 0.3f; // Intervalul �ntre trageri

};

