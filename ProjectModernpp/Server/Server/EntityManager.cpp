#include "EntityManager.h"

EntityManager::EntityManager(GameMap& map)
    : m_base{ Point(map.GetHeight() - 1, map.GetWidth() / 2) }, m_database{ "GameData.db" } 
{
    PlaceBase(map);
}

void EntityManager::AddBomb(const Bomb& bomb)
{
    m_bombs.push_back(bomb);
}

void EntityManager::AddPlayer(int id, const std::string& playerName, GameMap& map) 
{
    if (m_players.find(id) == m_players.end()) 
    {
        m_players.emplace(id, Player(playerName, map));
        m_playersBullets[id] = {};
        m_players[id].PlaceCharacter();
    }
}

void EntityManager::AddEnemy(int id, const Enemy& enemy)
{
    m_enemies.emplace(id, enemy);
}

void EntityManager::AddBullet(const Bullet& bullet)
{
    m_bullets.push_back(bullet);
}

void EntityManager::AddPlayerBullet(const Bullet& bullet, int playerId)
{
    auto& bullets = m_playersBullets[playerId];
    bullets.push_back(bullet);
}

void EntityManager::PlaceBase(GameMap& m_map)
{
    const int8_t centerX = m_map.GetHeight() - 1; 
    const int8_t centerY = m_map.GetWidth() / 2; 

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
    if (it != m_enemies.end()) 
    {
        m_enemies.erase(it);
    }
}

void EntityManager::RemoveBomb(size_t index)
{
    if (index < m_bombs.size()) 
    {
        m_bombs.erase(m_bombs.begin() + index);
    }
    else 
    {
        std::cerr << "Error: Bomb index out of range.\n";
    }
}

void EntityManager::ExplodeBomb(const Bomb& bomb, GameMap& map) 
{
    Point center = bomb.GetPosition();
    const int radius = 5;

    for (int dx = -radius; dx <= radius; ++dx) 
    {
        for (int dy = -radius; dy <= radius; ++dy) 
        {
            Point target(center.GetX() + dx, center.GetY() + dy);

            if (target.GetX() >= 0 && target.GetX() < map.GetHeight() &&
                target.GetY() >= 0 && target.GetY() < map.GetWidth()) 
            {
                if (map.GetMap()[target.GetX()][target.GetY()] != CellType::EMPTY) 
                {
                    map.SetStaticCell(target.GetX(), target.GetY(), CellType::EMPTY);
                }

                for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it) 
                {
                    if (it->second.GetPosition() == target) 
                    {
                        RemoveEnemy(it->first);
                        break;
                    }
                }

                for (auto it = m_players.begin(); it != m_players.end(); ++it) 
                {
                    if (it->second.GetPosition() == target) 
                    {
                        RemovePlayer(it->first);
                        break;
                    }
                }
            }
        }
    }
}

void EntityManager::HandleCollisions(GameMap& map) 
{
    for (auto& [playerId, bullets] : m_playersBullets) 
    {
        for (size_t bulletIndex = 0; bulletIndex < bullets.size();) 
        {
            Bullet& bullet = bullets[bulletIndex];

            if (!bullet.IsActive()) 
            {
                ++bulletIndex;
                continue;
            }

            Point currentPosition = bullet.GetPosition();
            Point direction = bullet.GetDirection();

            while (true) 
            {
                currentPosition = currentPosition + direction;

                if (currentPosition.GetX() < 0 || currentPosition.GetX() >= map.GetHeight() ||
                    currentPosition.GetY() < 0 || currentPosition.GetY() >= map.GetWidth()) 
                {
                    bullet.SetActive(false);
                    break;
                }

                CellType cell = map.GetMap()[currentPosition.GetX()][currentPosition.GetY()];

                if (cell == CellType::BREAKABLE_WALL) 
                {
                    bullet.SetActive(false);
                    map.SetStaticCell(currentPosition.GetX(), currentPosition.GetY(), CellType::EMPTY);
                    break;
                }

                if (cell == CellType::UNBREAKABLE_WALL) 
                {
                    bullet.SetActive(false);
                    break;
                }

                for (size_t enemyIndex = 0; enemyIndex < m_enemies.size(); ++enemyIndex) 
                {
                    if (currentPosition == m_enemies[enemyIndex].GetPosition()) 
                    {
                        bullet.SetActive(false);
                        m_enemies[enemyIndex].SetLives(m_enemies[enemyIndex].GetLives() - 1);

                        if (m_enemies[enemyIndex].GetLives() == 0) 
                        {
                            RemoveEnemy(enemyIndex);
                            m_enemies[enemyIndex].ResetPosition();
                            m_players[playerId].SetPoints(100);
                            m_players[playerId].SetScore(m_players[playerId].GetPoints());
                            m_database.UpdateGameData(m_players[playerId].GetName(), m_players[playerId].GetScore());
                        }
                        break;
                    }
                }

                if (currentPosition == m_base.GetPosition()) 
                {
                    bullet.SetActive(false);
                    m_base.TakeHit();

                    if (m_base.GetLife() == 0) 
                    {
                        m_winner = "";
                    }
                    break;
                }
                bullet.SetPosition(currentPosition);
            }

            if (!bullet.IsActive()) 
            {
                bullets.erase(bullets.begin() + bulletIndex);
            }
            else 
            {
                ++bulletIndex;
            }
        }
    }

    for (auto& [playerId, playerBullets] : m_playersBullets) 
    {
        for (size_t i = 0; i < playerBullets.size();) 
        {
            Bullet& playerBullet = playerBullets[i];

            if (!playerBullet.IsActive()) 
            {
                ++i;
                continue;
            }

            for (Bullet& enemyBullet : m_bullets) 
            {
                if (!enemyBullet.IsActive()) continue;

                if (playerBullet.GetPosition() == enemyBullet.GetPosition()) 
                {
                    playerBullet.SetActive(false);
                    enemyBullet.SetActive(false);
                    break;
                }
            }

            if (!playerBullet.IsActive()) 
            {
                playerBullets.erase(playerBullets.begin() + i);
            }
            else 
            {
                ++i;
            }
        }
    }
    
    for (auto& [playerId, bullets] : m_playersBullets) 
    {
        for (Bullet& bullet : bullets) 
        {
            if (!bullet.IsActive()) continue;

            for (auto& [otherPlayerId, otherPlayer] : m_players) 
            {
                if (otherPlayerId == playerId) continue;

                if (bullet.GetPosition() == otherPlayer.GetPosition()) 
                {
                    bullet.SetActive(false);
                    otherPlayer.SetLives(otherPlayer.GetLives() - 1);
                    otherPlayer.ResetPosition();

                    if (otherPlayer.GetLives() <= 0) 
                    {
                        RemovePlayer(otherPlayerId);
                    }
                    break;
                }
            }
        }
    }

    for (size_t bulletIndex = 0; bulletIndex < m_bullets.size();) 
    {
        Bullet& bullet = m_bullets[bulletIndex];
        if (!bullet.IsActive()) 
        {
            ++bulletIndex;
            continue;
        }

        for (auto& [playerId, player] : m_players) 
        {
            if (bullet.GetPosition() == player.GetPosition()) 
            {
                bullet.SetActive(false);
                player.SetLives(player.GetLives() - 1);
                player.ResetPosition();

                if (player.GetLives() <= 0) 
                {
                    RemovePlayer(playerId);
                }
                break;
            }
        }

        if (!bullet.IsActive()) 
        {
            m_bullets.erase(m_bullets.begin() + bulletIndex);
        }
        else 
        {
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

std::unordered_map<uint8_t, std::vector<Bullet>>& EntityManager::GetPlayersBullets()
{
    return m_playersBullets;
}

const std::unordered_map<uint8_t, Player>& EntityManager::GetPlayers() const 
{
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

const Base& EntityManager::GetBase() const
{
    return m_base;
}

std::string EntityManager::GetWinner() const
{
    return m_winner;
}

std::vector<uint8_t> EntityManager::GetPlayerIdsFromRoom(uint8_t playerId)
{
    return m_database.GetPlayersFromRoom(playerId);
}

std::vector<std::tuple<Point, std::string>> EntityManager::GetAllPlayerPositions() const 
{
    std::vector<std::tuple<Point, std::string>> playerPositions;

    for (const auto& [playerId, player] : m_players) 
    {
        Point position = player.GetPosition();
        std::string name = player.GetName();

        playerPositions.emplace_back(position, name);
    }

    return playerPositions;
}

void EntityManager::ResetPlayers(GameMap& map)
{
    for (auto& [id, player] : m_players)
    {
        player.ResetPositions(map);
        player.PlaceCharacter();
    }
}

void EntityManager::ResetBombs()
{
    m_bombs.clear();
}

void EntityManager::ResetBullets()
{
    m_bullets.clear();
}

void EntityManager::CloseRoom(const std::string& roomCode) 
{
    for (auto& [playerId, player] : m_players) 
    {
        player.Deactivate();
    }

    m_enemies.clear();
    m_bullets.clear();
    m_bombs.clear();
    m_playersBullets.clear();
    std::cout << "Room " << roomCode << " closed and resources cleaned up.\n";
}

void EntityManager::UpdateEnemyPositions(GameMap& map) 
{
    for (auto& enemy : m_enemies) 
    {
        enemy.second.MoveRandom(map);
        if (rand() % 5 == 0) 
        {
            Point shootDirection = enemy.second.GetShootDirection();
            if (shootDirection != Point(0, 0)) 
            {
                Bullet newBullet(enemy.second.GetPosition(), shootDirection);
                newBullet.SetActive(true);
                AddBullet(newBullet);
            }
        }
    }
}
