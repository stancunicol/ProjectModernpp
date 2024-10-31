#pragma once
#include "CellType.h"
#include <cstdint>
#include <vector>
#include <ctime>

class GameMap
{
private:
	uint32_t m_width, m_height;
    // 2D grid representing the map layout
	std::vector<std::vector<CellType>> m_grid;

public:
    // Constructor
	GameMap(int width, int height) : m_width(width), m_height(height) {
		m_grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
	}

    // Initializes the grid with random cell types; default - Empty
    void initialize() {
        srand(static_cast<unsigned>(time(0)));
        for (int i = 0; i < m_height; ++i) {
            for (int j = 0; j < m_width; ++j) {
                int randVal = rand() % 10;
                if (randVal < 3)
                    // Set cell as BREAKABLE_WALL with 30% probability
                    m_grid[i][j] = CellType::BREAKABLE_WALL;
                else if (randVal < 5) 
                    // Set cell as UNBREAKABLE_WALL with 20% probability
                    m_grid[i][j] = CellType::UNBREAKABLE_WALL;
                //Defaut - EMPTY - 50% probability
            }
        }
    }


};