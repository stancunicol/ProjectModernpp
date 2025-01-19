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
    if (m_players.find(id) == m_players.end()) {
        m_players.emplace(id, Player(playerName, map));
        m_playersBullets[id] = {};
        m_players[id].PlaceCharacter();
        std::cout << "Player " << playerName << " added with ID " << std::to_string(id) << ".\n";
    }
    else {
        std::cout << "[WARNING] Player ID " << std::to_string(id) << " already exists.\n";
    }
}

void EntityManager::AddEnemy(int id, const Enemy& enemy)
{
    m_enemies.emplace(id, enemy);
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

void EntityManager::RemoveEnemy(int id)
{
    auto it = m_enemies.find(id);
    if (it != m_enemies.end()) {
        m_enemies.erase(it);

        auto enemyShootTimerIt = std::find(m_enemyShootTimers.begin(), m_enemyShootTimers.end(), 0.0f); // Verifică logica pentru găsirea corectă a timerului
        if (enemyShootTimerIt != m_enemyShootTimers.end()) {
            m_enemyShootTimers.erase(enemyShootTimerIt);
        }
    }
}


void EntityManager::RemoveBomb(size_t index)
{
    if (index < m_bombs.size()) {
        m_bombs.erase(m_bombs.begin() + index);
    }
    else {
        std::cerr << "Error: Bomb index out of range.\n";
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
    const int radius = 5;

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
                for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it) {
                    if (it->second.GetPosition() == target) {
                        RemoveEnemy(it->first);
                        break;
                    }
                }


                for (auto it = m_players.begin(); it != m_players.end(); ++it) {
                    if (it->second.GetPosition() == target) {
                        RemovePlayer(it->first);
                        break;
                    }
                }
            }
        }
    }
}

void EntityManager::UpdateEntities(GameMap& map, float deltaTime)
{
    /*for (auto& [playerId, player] : m_players) {
        if (player.IsActive()) {
            player.MoveCharacter(player.GetDirection(), map);
        }
    }
    for (auto& [id, enemy] : m_enemies) {
        //enemy.MoveRandom(map);

        if (id < m_enemyShootTimers.size()) {
            m_enemyShootTimers[id] += deltaTime;
        }

        if (m_enemyShootTimers[id] >= m_enemyShootInterval) {
            Point shootDir = enemy.GetShootDirection();

            if (shootDir != Point(0, 0)) {
                Point bulletStartPos = enemy.GetPosition() + shootDir;

                if (bulletStartPos.GetX() >= 0 && bulletStartPos.GetX() < map.GetHeight() &&
                    bulletStartPos.GetY() >= 0 && bulletStartPos.GetY() < map.GetWidth() &&
                    map.GetMap()[bulletStartPos.GetX()][bulletStartPos.GetY()] == CellType::EMPTY) {
                    Bullet newBullet(bulletStartPos, shootDir);
                    AddBullet(newBullet);
                }
            }
            m_enemyShootTimers[id] = 0.0f;
        }
    }
    */
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

void EntityManager::HandleCollisions(GameMap& map) {
    for (auto& [playerId, bullets] : m_playersBullets) {
        for (size_t bulletIndex = 0; bulletIndex < bullets.size();) {
            Bullet& bullet = bullets[bulletIndex];

            if (!bullet.IsActive()) {
                ++bulletIndex;
                continue;
            }

            Point currentPosition = bullet.GetPosition();
            Point direction = bullet.GetDirection();

            // Traverse the bullet's path
            while (true) {
                currentPosition = currentPosition + direction;

                // Check if out of bounds
                if (currentPosition.GetX() < 0 || currentPosition.GetX() >= map.GetHeight() ||
                    currentPosition.GetY() < 0 || currentPosition.GetY() >= map.GetWidth()) {
                    bullet.SetActive(false);
                    break;
                }

                // Get cell type at current position
                CellType cell = map.GetMap()[currentPosition.GetX()][currentPosition.GetY()];

                // Handle breakable wall collision
                if (cell == CellType::BREAKABLE_WALL) {
                    bullet.SetActive(false);
                    map.SetStaticCell(currentPosition.GetX(), currentPosition.GetY(), CellType::EMPTY);
                    break;
                }

                // Handle unbreakable wall collision
                if (cell == CellType::UNBREAKABLE_WALL) {
                    bullet.SetActive(false);
                    break;
                }

                // Handle collisions with enemies
                for (size_t enemyIndex = 0; enemyIndex < m_enemies.size(); ++enemyIndex) {
                    if (currentPosition == m_enemies[enemyIndex].GetPosition()) {
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

                // Handle collisions with the base
                if (currentPosition == m_base.GetPosition()) {
                    bullet.SetActive(false);
                    m_base.TakeHit();

                    if (m_base.GetLife() == 0) {
                        std::cout << "BAZA A FOST DISTRUSA\n\n";

                        m_players[playerId].SetPoints(500);
                        m_players[playerId].SetScore();
                        m_database.UpdateGameData(m_players[playerId].GetName(), m_players[playerId].GetScore());
                        m_winner = m_players[playerId].GetName();
                    }
                    break;
                }

                // If no collision occurred, update the bullet's position
                bullet.SetPosition(currentPosition);
            }

            // If the bullet is inactive, remove it
            if (!bullet.IsActive()) {
                bullets.erase(bullets.begin() + bulletIndex);
            }
            else {
                ++bulletIndex;
            }
        }
    }

    // Handle player bullets vs enemy bullets
    for (auto& [playerId, playerBullets] : m_playersBullets) {
        for (size_t i = 0; i < playerBullets.size();) {
            Bullet& playerBullet = playerBullets[i];
            if (!playerBullet.IsActive()) {
                ++i;
                continue;
            }

            for (Bullet& enemyBullet : m_bullets) {
                if (!enemyBullet.IsActive()) continue;

                if (playerBullet.GetPosition() == enemyBullet.GetPosition()) {
                    playerBullet.SetActive(false);
                    enemyBullet.SetActive(false);
                    break;
                }
            }

            if (!playerBullet.IsActive()) {
                playerBullets.erase(playerBullets.begin() + i);
            }
            else {
                ++i;
            }
        }
    }

    // Handle player bullets vs other players
    for (auto& [playerId, bullets] : m_playersBullets) {
        for (Bullet& bullet : bullets) {
            if (!bullet.IsActive()) continue;

            for (auto& [otherPlayerId, otherPlayer] : m_players) {
                if (otherPlayerId == playerId) continue;

                if (bullet.GetPosition() == otherPlayer.GetPosition()) {
                    bullet.SetActive(false);
                    otherPlayer.SetLives(otherPlayer.GetLives() - 1);

                    if (otherPlayer.GetLives() <= 0) {
                        RemovePlayer(otherPlayerId);
                    }
                    break;
                }
            }
        }
    }

    // Handle enemy bullets vs players
    for (size_t bulletIndex = 0; bulletIndex < m_bullets.size();) {
        Bullet& bullet = m_bullets[bulletIndex];
        if (!bullet.IsActive()) {
            ++bulletIndex;
            continue;
        }

        for (auto& [playerId, player] : m_players) {
            if (bullet.GetPosition() == player.GetPosition()) {
                bullet.SetActive(false);
                player.SetLives(player.GetLives() - 1);

                if (player.GetLives() <= 0) {
                    RemovePlayer(playerId);
                }
                break;
            }
        }

        if (!bullet.IsActive()) {
            m_bullets.erase(m_bullets.begin() + bulletIndex);
        }
        else {
            ++bulletIndex;
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

const std::unordered_map<int, Enemy>& EntityManager::GetEnemies() const
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

std::unordered_map<int, Enemy>& EntityManager::GetEnemiesMutable()
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

std::vector<uint8_t> EntityManager::GetPlayerIdsFromRoom(uint8_t playerId)
{
    return m_database.GetPlayersFromRoom(playerId);
}

bool EntityManager::PlayerExists(uint8_t userId) const
{
    return m_players.find(userId) != m_players.end();
}

std::vector<std::tuple<Point, std::string>> EntityManager::GetAllPlayerPositions() const {
    std::vector<std::tuple<Point, std::string>> playerPositions;

    for (const auto& [playerId, player] : m_players) {
        Point position = player.GetPosition();
        std::string name = player.GetName();

        playerPositions.emplace_back(position, name);
    }

    return playerPositions;
}

void EntityManager::ResetPlayers(GameMap& map)
{
    for (auto& [id, player] : m_players) {
        player.ResetPositions(map);
        player.PlaceCharacter();
    }
}

void EntityManager::ResetBombs()
{
    m_bombs.clear();
}

void EntityManager::CloseRoom(const std::string& roomCode) {
    for (auto& [playerId, player] : m_players) {
        player.Deactivate();
    }

    m_enemies.clear();
    m_bullets.clear();
    m_bombs.clear();
    m_playersBullets.clear();
    std::cout << "Room " << roomCode << " closed and resources cleaned up.\n";
}

void EntityManager::UpdateEnemyPositions(GameMap& map) {
    for (auto& enemy : m_enemies) {
        enemy.second.MoveRandom(map);
    }
}
