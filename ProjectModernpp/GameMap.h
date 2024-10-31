#pragma once
#include "CellType.h"
#include <cstdint>
#include <vector>
#include <ctime>

class GameMap
{
private:
	uint32_t m_width, m_height;
	std::vector<std::vector<CellType>> m_grid;

public:
	GameMap(int width, int height) : m_width(width), m_height(height) {
		m_grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
	}

    void initialize() {
        srand(static_cast<unsigned>(time(0)));
        for (int i = 0; i < m_height; ++i) {
            for (int j = 0; j < m_width; ++j) {
                int randVal = rand() % 10;
                if (randVal < 3)
                    m_grid[i][j] = CellType::BREAKABLE_WALL;
                else if (randVal < 5) 
                    m_grid[i][j] = CellType::UNBREAKABLE_WALL;
            }
        }
    }


};