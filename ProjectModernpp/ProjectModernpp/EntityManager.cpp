﻿#include "EntityManager.h"

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

void EntityManager::AddPlayerBullet(const Bullet& bullet)
{
    m_playerBullets.push_back(bullet);
}


void EntityManager::RemovePlayer(size_t index)
{
    if (index < m_players.size()) {
        m_players.erase(m_players.begin() + index);
    }
}

void EntityManager::RemoveEnemy(size_t index)
{
    if (index < m_enemies.size()) {
        m_enemies.erase(m_enemies.begin() + index);
        m_enemyShootTimers.erase(m_enemyShootTimers.begin() + index);
    }
}

void EntityManager::EnemyShoots(const Point& direction, const Point& position)
{
    Bullet newBullet(position, direction);
    newBullet.SetActive(true);
    AddBullet(newBullet);
}

void EntityManager::PlayerShoot(GameMap& map)
{
    Player& player = m_players[0];
    Point shootDirection = player.GetShootDirection();

    if (shootDirection != Point(0, 0)) {
        Point bulletStartPos = player.GetPosition();

        if (bulletStartPos.GetX() >= 0 && bulletStartPos.GetX() < map.GetHeight() &&
            bulletStartPos.GetY() >= 0 && bulletStartPos.GetY() < map.GetWidth() &&
            map.GetMap()[bulletStartPos.GetX()][bulletStartPos.GetY()] == CellType::EMPTY) {
            Bullet bullet(bulletStartPos, shootDirection);
            AddPlayerBullet(bullet);
        }
    }
    player.SetShootDirection(shootDirection);
}

void EntityManager::UpdateEntities(GameMap& map, float deltaTime)
{
    map.ClearDynamicEntities();

    for (size_t i = 0; i < m_players.size(); ++i) {
        m_players[i].MoveCharacter(m_players[i].GetDirection(), map);
    }

    for (size_t i = 0; i < m_enemies.size(); ++i) {
        m_enemies[i].MoveRandom(map);
        // Actualizăm timerul pentru tragere
        m_enemyShootTimers[i] += deltaTime;

        if (m_enemyShootTimers[i] >= m_enemyShootInterval) {
            Point shootDir = m_enemies[i].GetShootDirection();
            if (shootDir != Point(0, 0)) {
                Point bulletStartPos = m_enemies[i].GetPosition() + shootDir;
                if (bulletStartPos.GetX() >= 0 && bulletStartPos.GetX() < map.GetHeight() &&
                    bulletStartPos.GetY() >= 0 && bulletStartPos.GetY() < map.GetWidth() &&
                    map.GetMap()[bulletStartPos.GetX()][bulletStartPos.GetY()] == CellType::EMPTY) {
                    Bullet newBullet(bulletStartPos, shootDir);
                    AddBullet(newBullet);
                }
            }
            m_enemyShootTimers[i] = 0.0f;
        }
    }

    for (size_t i = 0; i < m_playerBullets.size(); ++i) {
        m_playerBullets[i].Move(map);
    }

    for (size_t i = 0; i < m_bullets.size(); ++i) {
        m_bullets[i].Move(map); // Mutăm fiecare glonț în direcția sa
    }

    HandleCollisions();

    for (const Player& player : m_players) {
        map.DrawEntity(player.GetPosition(), 'P');
    }

    for (const Enemy& enemy : m_enemies) {
        map.DrawEntity(enemy.GetPosition(), 'E');
    }

    for (const Bullet& bullet : m_playerBullets) {
        if (bullet.IsActive()) {
            map.DrawEntity(bullet.GetPosition(), 'o'); // Gloanțele playerului
        }
    }

    for (const Bullet& bullet : m_bullets) {
        if (bullet.IsActive()) {
            map.DrawEntity(bullet.GetPosition(), 'o');
        }
    }
    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
        [](const Bullet& bullet) { return !bullet.IsActive(); }),
        m_bullets.end());

    m_playerBullets.erase(std::remove_if(m_playerBullets.begin(), m_playerBullets.end(),
        [](const Bullet& bullet) { return !bullet.IsActive(); }),
        m_playerBullets.end());

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

void EntityManager::HandleCollisions() {
    // Coliziuni gloanțe player - inamici
    for (size_t i = 0; i < m_playerBullets.size(); ++i) {
        if (!m_playerBullets[i].IsActive()) continue;

        for (size_t j = 0; j < m_enemies.size(); ++j) {
            if (m_playerBullets[i].GetPosition() == m_enemies[j].GetPosition()) {
                m_playerBullets[i].SetActive(false);
                m_enemies[j].SetLives(m_enemies[j].GetLives() - 1);

                if (m_enemies[j].GetLives() == 0) {
                    RemoveEnemy(j);
                }
                break;
            }
        }
    }

    // Coliziuni gloanțe inamici - player
    for (size_t i = 0; i < m_bullets.size(); ++i) {
        if (!m_bullets[i].IsActive()) continue;

        for (size_t k = 0; k < m_players.size(); ++k) {
            if (m_bullets[i].GetPosition() == m_players[k].GetPosition()) {
                m_bullets[i].SetActive(false);
                m_players[k].SetLives(m_players[k].GetLives() - 1);

                if (m_players[k].GetLives() == 0) {
                    //RemovePlayer(k);
                }
                break;
            }
        }
    }
}
