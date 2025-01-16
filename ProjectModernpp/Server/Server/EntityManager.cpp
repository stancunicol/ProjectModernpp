#include "EntityManager.h"

EntityManager::EntityManager(GameMap& map)
    : m_base{ Point(map.GetHeight() - 1, map.GetWidth() / 2) }, m_database{ "GameData.db" } {
    PlaceBase(map);
}

void EntityManager::AddBomb(const Bomb& bomb)
{
    m_bombs.push_back(bomb);
}

void EntityManager::AddPlayer(int id, const std::string& playerName, GameMap& map) {
    m_players.emplace(id, Player(playerName, map));
    m_playersBullets[id] = {};
    m_players[id].PlaceCharacter();
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

void EntityManager::AddPlayerBullet(const Bullet& bullet, int playerId) {
    auto& bullets = m_playersBullets[playerId];
    bullets.push_back(bullet);
}

void EntityManager::PlaceBase(GameMap& m_map)
{
    const int8_t centerX = m_map.GetHeight() - 1; // Bottom row
    const int8_t centerY = m_map.GetWidth() / 2;  // Middle column

    m_base.SetPosition({ centerX, centerY });

    auto& map = m_map.GetMap();

    map[centerX][centerY - 1] = CellType::BREAKABLE_WALL;
    map[centerX][centerY + 1] = CellType::BREAKABLE_WALL;
    map[centerX - 1][centerY - 1] = CellType::BREAKABLE_WALL;
    map[centerX - 1][centerY] = CellType::BREAKABLE_WALL;
    map[centerX - 1][centerY + 1] = CellType::BREAKABLE_WALL;
}

void EntityManager::RemovePlayer(size_t playerId)
{
    auto it = m_players.find(playerId);
    if (it != m_players.end()) {
        it->second.Deactivate();

        std::this_thread::sleep_for(std::chrono::seconds(5));
        it->second.Activate();
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

void EntityManager::PlayerShoot(GameMap& map, int playerId) {
    auto it = m_players.find(playerId);
    if (it == m_players.end()) {
        std::cerr << "Error: Player not found.\n";
        return;
    }

    Player& player = it->second;
    Point shootDirection = player.GetShootDirection();

    if (shootDirection != Point(0, 0)) {
        Point bulletStartPos = player.GetPosition();

        if (bulletStartPos.GetX() >= 0 && bulletStartPos.GetX() < map.GetHeight() &&
            bulletStartPos.GetY() >= 0 && bulletStartPos.GetY() < map.GetWidth() &&
            map.GetMap()[bulletStartPos.GetX()][bulletStartPos.GetY()] == CellType::EMPTY) {

            Bullet bullet(bulletStartPos, shootDirection);
            AddPlayerBullet(bullet, playerId);
        }
        else {
            std::cerr << "Error: Invalid bullet start position.\n";
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

    for (auto& [playerId, player] : m_players) {
        if (player.IsActive()) {
            player.MoveCharacter(player.GetDirection(), map);
        }
    }

    for (size_t i = 0; i < m_enemies.size(); ++i) {
        m_enemies[i].MoveRandom(map);
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

    // Actualizăm gloanțele fiecărui jucător
    for (auto& [playerId, bullets] : m_playersBullets) {
        for (Bullet& bullet : bullets) {
            bullet.Move(map);
        }
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

    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
        [](const Bullet& bullet) { return !bullet.IsActive(); }),
        m_bullets.end());

    for (auto& playerBullets : m_playersBullets) {
        playerBullets.second.erase(std::remove_if(playerBullets.second.begin(), playerBullets.second.end(),
            [](const Bullet& bullet) { return !bullet.IsActive(); }),
            playerBullets.second.end());
    }
}

void EntityManager::HandleCollisions(GameMap& map)
{
    // Coliziuni gloanțe player - inamici
    for (auto& [playerId, bullets] : m_playersBullets) {
        for (size_t bulletIndex = 0; bulletIndex < bullets.size(); ++bulletIndex) {
            Bullet& bullet = bullets[bulletIndex];
            if (!bullet.IsActive()) continue;

            // Coliziuni cu inamici
            for (size_t enemyIndex = 0; enemyIndex < m_enemies.size(); ++enemyIndex) {
                if (bullet.GetPosition() == m_enemies[enemyIndex].GetPosition()) {
                    bullet.SetActive(false);
                    m_enemies[enemyIndex].SetLives(m_enemies[enemyIndex].GetLives() - 1);

                    if (m_enemies[enemyIndex].GetLives() == 0) {
                        RemoveEnemy(enemyIndex);
                        m_players[playerId].SetPoints(100);
                        m_players[playerId].SetScore();
                        m_database.UpdateGameData(m_players[playerId].GetName(), m_players[playerId].GetScore());
                    }
                    break;
                }
            }

            // Coliziuni cu baza
            if ((bullet.GetPosition() + bullet.GetDirection()) == m_base.GetPosition()) {
                bullet.SetActive(false);
                m_base.TakeHit();
                if (m_base.GetLife() == 0) {
                    m_players[playerId].SetPoints(500);
                    m_players[playerId].SetScore();
                    m_database.UpdateGameData(m_players[playerId].GetName(), m_players[playerId].GetScore());
                    m_winner = m_players[playerId].GetName();
                }
                break;
            }
        }
    }

    // Coliziuni gloanțe inamici - jucători
    for (size_t bulletIndex = 0; bulletIndex < m_bullets.size(); ++bulletIndex) {
        Bullet& bullet = m_bullets[bulletIndex];
        if (!bullet.IsActive()) continue;

        // Coliziuni cu jucători
        for (size_t playerIndex = 0; playerIndex < m_players.size(); ++playerIndex) {
            if (bullet.GetPosition() == m_players[playerIndex].GetPosition()) {
                bullet.SetActive(false);
                m_players[playerIndex].SetLives(m_players[playerIndex].GetLives() - 1);

                if (m_players[playerIndex].GetLives() == 0) {
                    RemovePlayer(playerIndex);
                }
                break;
            }
        }

        // Coliziuni cu baza
        if ((bullet.GetPosition() + bullet.GetDirection()) == m_base.GetPosition()) {
            bullet.SetActive(false);
            m_base.TakeHit();
            if (m_base.GetLife() == 0) {
                m_winner = "Enemy";
            }
            break;
        }
    }
}

void EntityManager::RemoveBullet(size_t index)
{
    if (index < m_bullets.size()) {
        m_bullets.erase(m_bullets.begin() + index);
    }
}

const std::unordered_map<uint8_t, Player>& EntityManager::GetPlayers() const {
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

const std::vector<Bomb>& EntityManager::GetBombs() const
{
    return m_bombs;
}

std::unordered_map<uint8_t, Player>& EntityManager::GetPlayersMutable()
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

Player* EntityManager::GetPlayerById(int playerId) {
    auto it = m_players.find(playerId);
    return (it != m_players.end()) ? &it->second : nullptr;
}

bool EntityManager::PlayerExists(uint8_t userId) const
{
    return m_players.find(userId) != m_players.end();
}
