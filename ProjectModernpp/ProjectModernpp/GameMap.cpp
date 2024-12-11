#include "GameMap.h"
#include <cstdlib>
#include <iostream>
#include "Bullet.h"
#include <cassert>

void GameMap::MatrixSizeGenerator()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    do {
        m_height = 19 + (std::rand() % ((27 - 19) + 1));
    } while (m_height % 2 == 0);

    do {
        m_width = 19 + (std::rand() % ((27 - 19) + 1));
    } while (m_width % 2 == 0 || m_width == m_height);
}

GameMap::GameMap(uint8_t level)
    : m_level(level)
{
    MatrixSizeGenerator();
    m_dynamicGrid.resize(m_height, std::vector<CellType>(m_width, CellType::EMPTY));
    m_grid.resize(m_height, std::vector<CellType>(m_width, CellType::EMPTY));
    Initialize();
}

void GameMap::Initialize()
{
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

            if (m_grid[i][j] == CellType::EMPTY && HasEmptyNeighbors(i, j))
            {
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

bool GameMap::IsInSafeZone(int x, int y, int safeZoneSize)
{
    return (x < safeZoneSize && y < safeZoneSize) ||
        (x < safeZoneSize && y >= m_width - safeZoneSize) ||
        (x >= m_height - safeZoneSize && y < safeZoneSize) ||
        (x >= m_height - safeZoneSize && y >= m_width - safeZoneSize);
}

bool GameMap::HasEmptyNeighbors(int x, int y)
{
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

void GameMap::Display() const
{
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {

            if (m_dynamicGrid[i][j] != CellType::EMPTY && m_dynamicGrid[i][j] != CellType::BREAKABLE_WALL
                && m_dynamicGrid[i][j] != CellType::UNBREAKABLE_WALL) {
                std::cout << ' ' << static_cast<char>(m_dynamicGrid[i][j]) << ' ';
            }
            else {
                std::cout << ' ' << static_cast<char>(m_grid[i][j]) << ' ';;
            }
        }
        std::cout << std::endl;
    }
}

void GameMap::ClearDynamicEntities()
{
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            m_dynamicGrid[i][j] = CellType::EMPTY;
        }
    }
}

void GameMap::DrawEntity(const Point& position, char symbol)
{
    if (position.GetX() >= 0 && position.GetX() < m_height &&
        position.GetY() >= 0 && position.GetY() < m_width) {

        m_dynamicGrid[position.GetX()][position.GetY()] = static_cast<CellType>(symbol);
    }
}

void GameMap::SetStaticCell(int x, int y, CellType type)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_grid[x][y] = type;
    }
}

std::vector<std::vector<CellType>>& GameMap::GetMap()
{
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

uint32_t GameMap::GetLevel() const
{
    return m_level;
}

void GameMap::Reset(uint8_t level) {
    m_level = level;

    m_grid.clear();
    m_grid.resize(m_height, std::vector<CellType>(m_width, CellType::EMPTY));

    Initialize();
}
