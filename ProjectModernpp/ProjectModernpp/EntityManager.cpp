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

void EntityManager::RemovePlayer(size_t index) 
{
    if (index < m_players.size()) {
        //m_players.erase(m_players.begin() + index);
    }
}

void EntityManager::RemoveEnemy(size_t index) 
{
    if (index < m_enemies.size()) {
        //m_enemies.erase(m_enemies.begin() + index);
        m_enemyShootTimers.erase(m_enemyShootTimers.begin() + index);
    }
}

void EntityManager::RemoveBullet(size_t index) 
{
    if (index < m_bullets.size()) {
        m_bullets.erase(m_bullets.begin() + index);
    }
}

const std::vector<Player>& EntityManager::GetPlayers() const
{
    return m_players;
}

const std::vector<Enemy>& EntityManager::GetEnemies() const
{
    return m_enemies;
}

const std::vector<Bullet>& EntityManager::GetBullets() const
{
    return m_bullets;
}

std::vector<Player>& EntityManager::GetPlayersMutable()
{
    return m_players;
}

void EntityManager::HandleCollisions()
{
    for (size_t i = 0; i < m_bullets.size(); ++i) {
        if (!m_bullets[i].IsActive()) continue;

        for (size_t j = 0; j < m_enemies.size(); ++j) {
            if (m_bullets[i].GetPosition() == m_enemies[j].GetPosition()) {
                m_bullets[i].SetActive(false);
                //RemoveEnemy(j);
                break;
            }
        }

        for (size_t k = 0; k < m_players.size(); ++k) {
            if (m_bullets[i].GetPosition() == m_players[k].GetPosition()) {
                m_bullets[i].SetActive(false);
                //RemovePlayer(k);
                break;
            }
        }
    }
}