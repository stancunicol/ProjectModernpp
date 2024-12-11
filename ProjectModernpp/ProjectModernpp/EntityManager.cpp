#include "EntityManager.h"

EntityManager::EntityManager(GameMap& map)
    : m_base{ Point(map.GetHeight() - 1, map.GetWidth() / 2) }, m_database{ "GameData.db" } {
    PlaceBase(map);
}

void EntityManager::AddBomb(const Bomb& bomb)
{
    m_bombs.push_back(bomb);
}

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

void EntityManager::PlaceBase(GameMap& m_map)
{
    const int centerX = m_map.GetHeight() - 1; // Bottom row
    const int centerY = m_map.GetWidth() / 2;  // Middle column

    m_base.SetPosition({ centerX, centerY });

    auto& map = m_map.GetMap();

    map[centerX][centerY - 1] = CellType::BREAKABLE_WALL;
    map[centerX][centerY + 1] = CellType::BREAKABLE_WALL;
    map[centerX - 1][centerY - 1] = CellType::BREAKABLE_WALL;
    map[centerX - 1][centerY] = CellType::BREAKABLE_WALL;
    map[centerX - 1][centerY + 1] = CellType::BREAKABLE_WALL;
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

void EntityManager::RemoveBomb(size_t index)
{
    if (index < m_bombs.size()) {
        m_bombs.erase(m_bombs.begin() + index);
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

void EntityManager::ExplodeBomb(const Bomb& bomb, GameMap& map) {
    Point center = bomb.GetPosition();
    const int radius = 3;

    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            Point target(center.GetX() + dx, center.GetY() + dy);

            if (target.GetX() >= 0 && target.GetX() < map.GetHeight() &&
                target.GetY() >= 0 && target.GetY() < map.GetWidth()) {

                // Ștergem blocurile
                if (map.GetMap()[target.GetX()][target.GetY()] != CellType::EMPTY) {
                    map.SetStaticCell(target.GetX(), target.GetY(), CellType::EMPTY);
                }

                // Eliminăm inamici și jucători
                for (size_t i = 0; i < m_enemies.size(); ++i) {
                    if (m_enemies[i].GetPosition() == target) {
                        RemoveEnemy(i);
                        break;
                    }
                }

                for (size_t i = 0; i < m_players.size(); ++i) {
                    if (m_players[i].GetPosition() == target) {
                        RemovePlayer(i);
                        m_players[i].SetScore();
                        break;
                    }
                }
            }
        }
    }
}

void EntityManager::UpdateEntities(GameMap& map, float deltaTime)
{
    map.ClearDynamicEntities();

    for (size_t i = 0; i < m_players.size(); ++i) 
    {
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

    for (size_t i = 0; i < m_playerBullets.size(); ++i) 
    {
        m_playerBullets[i].Move(map);
    }

    for (size_t i = 0; i < m_bullets.size(); ++i) 
    {
        m_bullets[i].Move(map);
    }

    for (size_t i = 0; i < m_bombs.size(); ++i) {
        m_bombs[i].Update(deltaTime);
        if (m_bombs[i].IsActive()) {
            ExplodeBomb(m_bombs[i], map);
            std::cout << "!!! BOOM !!! Bomb Exploded!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            RemoveBomb(i);
        }
    }
    
    HandleCollisions(map);

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

    if (!m_base.IsDestroyed()) {
        map.DrawEntity(m_base.GetPosition(), 'B');
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

std::vector<Enemy>& EntityManager::GetEnemiesMutable()
{
    return m_enemies;
}

std::vector<Bullet>& EntityManager::GetBulletsMutable()
{
    return m_bullets;
}

const Base& EntityManager::GetBase() const
{
    return m_base;
}

std::string EntityManager::GetWinner() const
{
    return m_winner;
}

void EntityManager::HandleCollisions(GameMap& map) 
{
    // Coliziuni gloanțe player - inamici
    for (size_t i = 0; i < m_playerBullets.size(); ++i) {
        if (!m_playerBullets[i].IsActive()) continue;

        for (size_t j = 0; j < m_enemies.size(); ++j) {
            if (m_playerBullets[i].GetPosition() == m_enemies[j].GetPosition()) {
                m_playerBullets[i].SetActive(false);
                m_enemies[j].SetLives(m_enemies[j].GetLives() - 1);

                if (m_enemies[j].GetLives() == 0) {
                    RemoveEnemy(j);
                    m_players[0].SetPoints(100);
                    m_players[0].SetScore();
                    m_database.UpdateGameData(m_players[0].GetName(), m_players[0].GetScore());
                }
                break;
            }
        }

        if ((m_playerBullets[i].GetPosition() + m_playerBullets[i].GetDirection()) == m_base.GetPosition()) {
            m_playerBullets[i].SetActive(false);
            m_base.TakeHit();
            if (m_base.GetLife() == 0) {
                m_players[0].SetPoints(500);
                m_players[0].SetScore();
                m_database.UpdateGameData(m_players[0].GetName(), m_players[0].GetScore());
                m_winner = m_players[0].GetName();
            }
            break;
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

        if ((m_bullets[i].GetPosition() + m_bullets[i].GetDirection()) == m_base.GetPosition()) {
            m_bullets[i].SetActive(false);
            m_base.TakeHit();
            if (m_base.GetLife() == 0) {
                m_winner = "Enemy";
            }
            break;
        }
    }
}

