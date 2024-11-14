#include "GameMap.h"

GameMap::GameMap(uint32_t width, uint32_t height)
    : m_width(width),
    m_height(height)
{
    m_grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
}

void GameMap::initialize(uint8_t level) {

    srand(static_cast<unsigned>(time(0)));

    int breakableWallChance;
    int unbreakableWallChance;
    // Set wall chances based on difficulty level
    switch (level) {
    case 1: // easy 
        breakableWallChance = 10;
        unbreakableWallChance = 5;
    case 2: // medium
        breakableWallChance = 20;
        unbreakableWallChance = 10;
    case 3: // hard
        breakableWallChance = 30;
        unbreakableWallChance = 15;
    default:
        break;
    }

    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            int randVal = rand() % 100;
            if (randVal < unbreakableWallChance)
                // Set cell as BREAKABLE_WALL 
                m_grid[i][j] = CellType::BREAKABLE_WALL;
            else if (randVal < breakableWallChance + unbreakableWallChance)
                // Set cell as UNBREAKABLE_WALL 
                m_grid[i][j] = CellType::UNBREAKABLE_WALL;
            //Defaut - EMPTY 
        }
    }
}