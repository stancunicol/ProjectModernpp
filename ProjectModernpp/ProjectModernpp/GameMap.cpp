#include "GameMap.h"
#include <cstdlib>
#include <iostream>
#include "Bullet.h"

GameMap::GameMap(uint32_t width, uint32_t height, uint8_t level)
    : m_width(width), m_height(height), m_level(level) {
    m_grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    Initialize();
}

void GameMap::Initialize() {
    int breakableWallChance, unbreakableWallChance, safeZoneSize = 3;

    switch (m_level) {
    case 1: breakableWallChance = 15; unbreakableWallChance = 5; break;
    case 2: breakableWallChance = 20; unbreakableWallChance = 10; break;
    case 3: breakableWallChance = 30; unbreakableWallChance = 15; break;
    default: break;
    }

    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (IsInSafeZone(i, j, safeZoneSize)) continue;

            int randVal = rand() % 100;

            if (m_grid[i][j] == CellType::EMPTY && HasEmptyNeighbors(i, j)) {
                if (randVal < breakableWallChance) {
                    m_grid[i][j] = CellType::BREAKABLE_WALL;
                }
                else if (randVal < breakableWallChance + unbreakableWallChance) {
                    m_grid[i][j] = CellType::UNBREAKABLE_WALL;
                }
            }
        }
    }
}

bool GameMap::IsInSafeZone(int x, int y, int safeZoneSize) {
    return (x < safeZoneSize && y < safeZoneSize) ||
        (x < safeZoneSize && y >= m_width - safeZoneSize) ||
        (x >= m_height - safeZoneSize && y < safeZoneSize) ||
        (x >= m_height - safeZoneSize && y >= m_width - safeZoneSize);
}

bool GameMap::HasEmptyNeighbors(int x, int y) {
    int emptyCount = 0;
    const int dx[] = { -1, 0, 1, 0 };
    const int dy[] = { 0, -1, 0, 1 };

    for (int k = 0; k < 4; ++k) {
        int nx = x + dx[k], ny = y + dy[k];
        if (nx >= 0 && nx < m_height && ny >= 0 && ny < m_width &&
            m_grid[nx][ny] == CellType::EMPTY) {
            emptyCount++;
        }
    }
    return emptyCount >= 2;
}

void GameMap::Display() const {
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            switch (m_grid[i][j]) {
            case CellType::PLAYER: std::cout << "\033[32m P \033[0m"; break;
            case CellType::EMPTY: std::cout << "\033[36m . \033[0m"; break;
            case CellType::BREAKABLE_WALL: std::cout << "\033[35m * \033[0m"; break;
            case CellType::UNBREAKABLE_WALL: std::cout << "\033[33m # \033[0m"; break;
            case CellType::BULLET: std::cout << "\033[34m o \033[0m"; break;
            case CellType::ENEMY: std::cout << "\033[31m E \033[0m"; break;
            case CellType::BASE: std::cout << " B "; break;
            default: std::cout << " ? "; break;
            }
        }
        std::cout << '\n';
    }
}

std::vector<std::vector<CellType>>& GameMap::GetMap() {
    return m_grid;
}

uint32_t GameMap::GetWidth() const
{
    return m_grid[0].size();
}

uint32_t GameMap::GetHeight() const
{
    return m_grid.size();
}

void GameMap::AddBullet(const Bullet& bullet) {
    m_bullets.push_back(bullet);
}

void GameMap::UpdateBullets() {
    for (auto& bullet : m_bullets) {
        if (!bullet.IsActive()) continue;

        // Calculăm noua poziție
        Point newPos = bullet.GetPosition() + bullet.GetDirection();

        // Verificăm limitele hărții
        if (newPos.GetX() < 0 || newPos.GetX() >= m_height ||
            newPos.GetY() < 0 || newPos.GetY() >= m_width) {
            // Curățăm poziția anterioară a glonțului
            m_grid[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;

            bullet.SetActive(false); // Glonțul devine inactiv
            continue;
        }

        // Verificăm coliziunea cu jucătorul
        if (m_grid[newPos.GetX()][newPos.GetY()] == CellType::PLAYER) {
            // Glonțul devine inactiv, dar nu curățăm poziția jucătorului
            bullet.SetActive(false);
            m_grid[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;
            continue;
        }

        // Alte coliziuni (ziduri)
        if (m_grid[newPos.GetX()][newPos.GetY()] == CellType::BREAKABLE_WALL) {
            m_grid[newPos.GetX()][newPos.GetY()] = CellType::EMPTY;
            m_grid[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;
            bullet.SetActive(false);
            continue;
        }
        if (m_grid[newPos.GetX()][newPos.GetY()] == CellType::UNBREAKABLE_WALL) {
            m_grid[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;
            bullet.SetActive(false);
            continue;
        }

        // Mutăm glonțul pe noua poziție
        m_grid[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;
        bullet.SetPosition(newPos);
        m_grid[newPos.GetX()][newPos.GetY()] = CellType::BULLET;
    }

    // Eliminăm gloanțele inactive
    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
        [](const Bullet& b) { return !b.IsActive(); }), m_bullets.end());
}

