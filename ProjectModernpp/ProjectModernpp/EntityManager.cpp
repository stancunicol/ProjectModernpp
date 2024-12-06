#include "EntityManager.h"

EntityManager::EntityManager() {}

void EntityManager::AddPlayer(const Player& player)
{
    m_players.push_back(player);
    m_players.back().PlaceCharacter();
}

void EntityManager::AddEnemy(const Enemy& enemy)
{
    m_enemies.push_back(enemy);
    m_enemyShootTimers.push_back(0.0f);
}

void EntityManager::AddBullet(const Bullet& bullet)
{
    m_bullets.push_back(bullet);
}
