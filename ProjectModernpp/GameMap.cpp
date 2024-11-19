#include "GameMap.h"

GameMap::GameMap(uint32_t width, uint32_t height, uint8_t level)
    : m_width(width), m_height(height), m_level(level) {
    m_grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    Initialize();
}

void GameMap::Initialize() {

    int breakableWallChance;
    int unbreakableWallChance;
    // Set wall chances based on difficulty level
    switch (m_level) {
    case 1: // easy
        breakableWallChance = 10;   // 10% chance for breakable walls
        unbreakableWallChance = 5;  // 5% chance for unbreakable walls
        break;
    case 2: // medium
        breakableWallChance = 20;   // 20% chance for breakable walls
        unbreakableWallChance = 10; // 10% chance for unbreakable walls
        break;
    case 3: // hard
        breakableWallChance = 30;   // 30% chance for breakable walls
        unbreakableWallChance = 15; // 15% chance for unbreakable walls
        break;
    default:
        break;
    }

    // Generate walls structured in clusters
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (m_grid[i][j] != CellType::EMPTY) continue; // Skip if the cell is already occupied

            int randVal = rand() % 100; // Generate a random number between 0 and 99
            if (randVal < breakableWallChance) {
                // Create a cluster for breakable walls
                CreateWallCluster(i, j, CellType::BREAKABLE_WALL);
            }
            else if (randVal < breakableWallChance + unbreakableWallChance) {
                // Create a cluster for unbreakable walls
                CreateWallCluster(i, j, CellType::UNBREAKABLE_WALL);
            }
            // Otherwise, leave the cell empty (default)
        }
    }
}

void GameMap::CreateWallCluster(int startX, int startY, CellType wallType) {
    int clusterSize = rand() % 3 + 1; // Cluster size: between 1 and 3 cells

    for (int i = 0; i < clusterSize; ++i) {
        for (int j = 0; j < clusterSize; ++j) {
            int x = startX + i;
            int y = startY + j;

            // Check if the new position is within bounds and if the cell is empty
            if (x < m_height && y < m_width && m_grid[x][y] == CellType::EMPTY) {
                m_grid[x][y] = wallType; // Set the cell type to the specified wall type
            }
        }
    }
}

void GameMap::Display() const {
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {

            switch (m_grid[i][j]) {
            case CellType::PLAYER: std::cout << "\033[32m" << " P " << "\033[0m"; break;
            case CellType::EMPTY: std::cout << "\033[36m" << " . " << "\033[0m"; break;
            case CellType::BREAKABLE_WALL: std::cout << "\033[35m" << " * " << "\033[0m"; break;
            case CellType::UNBREAKABLE_WALL: std::cout << "\033[33m" << " # " << "\033[0m"; break;
            case CellType::ENEMY: std::cout << "\033[31m" << " E " << "\033[0m"; break;
            }
        }
        std::cout << '\n';
    }
}

std::vector<std::vector<CellType>>& GameMap::GetMap() {
    return m_grid; 
}

uint32_t GameMap::GetWidth() const{
    return m_width;
}

uint32_t GameMap::GetHeight() const{
    return m_height;
}