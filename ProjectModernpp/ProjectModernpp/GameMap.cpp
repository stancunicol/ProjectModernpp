#include "GameMap.h"

GameMap::GameMap(uint32_t width, uint32_t height, uint8_t level)
    : m_width(width), m_height(height), m_level(level) {
    m_grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    Initialize();
}
void GameMap::Initialize() {
    int breakableWallChance;
    int unbreakableWallChance;
    int safeZoneSize = 3; // Size of the safe zone around the players

    // Set chances based on difficulty level
    switch (m_level) {
    case 1: // easy
        breakableWallChance = 15;   // 15% chance
        unbreakableWallChance = 5;  // 5% chance
        break;
    case 2: // medium
        breakableWallChance = 20;   // 20% chance
        unbreakableWallChance = 10; // 10% chance
        break;
    case 3: // hard
        breakableWallChance = 30;   // 30% chance
        unbreakableWallChance = 15; // 15% chance
        break;
    default:
        break;
    }

    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            // Avoid generating walls in the safe zone
            if (IsInSafeZone(i, j, safeZoneSize)) {
                continue;
            }

            // Probability for each type of wall
            int randVal = rand() % 100;

            // If the position is empty and has at least two empty neighbors
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
    // The zone in the top-left / top-right / bottom-left / bottom-right corners
    if (x < safeZoneSize && y < safeZoneSize ||
        x < safeZoneSize && y >= m_width - safeZoneSize ||
        x >= m_height - safeZoneSize && y < safeZoneSize ||
        x >= m_height - safeZoneSize && y >= m_width - safeZoneSize) {
        return true;
    }

    return false;
}

bool GameMap::HasEmptyNeighbors(int x, int y) {
    int emptyCount = 0;
    const int dx[] = { -1, 0, 1, 0 };
    const int dy[] = { 0, -1, 0, 1 };

    for (int k = 0; k < 4; ++k) {
        int nx = x + dx[k];
        int ny = y + dy[k];
        if (nx >= 0 && nx < m_height && ny >= 0 && ny < m_width && m_grid[nx][ny] == CellType::EMPTY)
            emptyCount++;
    }
    return emptyCount >= 2; // Ensures the cell has at least 2 empty neighbors
}

/*
void GameMap::CreateWallCluster(int startX, int startY, CellType wallType) {
    // Cluster foarte mic: între 1 și 2 celule
    int clusterSize = rand() % 2 + 1;

    for (int i = 0; i < clusterSize; ++i) {
        for (int j = 0; j < clusterSize; ++j) {
            int x = startX + i;
            int y = startY + j;

            // Verifică dacă poziția e validă și liberă
            if (x >= 0 && x < m_height && y >= 0 && y < m_width && m_grid[x][y] == CellType::EMPTY) {
                m_grid[x][y] = wallType;
            }
        }
    }
}
*/
void GameMap::Display() const {
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {

            switch (m_grid[i][j]) {
            case CellType::PLAYER: std::cout << "\033[32m" << " P " << "\033[0m"; break;
            case CellType::EMPTY: std::cout << "\033[36m" << " . " << "\033[0m"; break;
            case CellType::BREAKABLE_WALL: std::cout << "\033[35m" << " * " << "\033[0m"; break;
            case CellType::UNBREAKABLE_WALL: std::cout << "\033[33m" << " # " << "\033[0m"; break;
            case CellType::ENEMY: std::cout << "\033[31m" << " E " << "\033[0m"; break;
            case CellType::BASE: std::cout << " B "; break;
            }
        }
        std::cout << '\n';
    }
}

std::vector<std::vector<CellType>>& GameMap::GetMap() {
    return m_grid;
}

uint32_t GameMap::GetWidth() const {
    return m_width;
}

uint32_t GameMap::GetHeight() const {
    return m_height;
}