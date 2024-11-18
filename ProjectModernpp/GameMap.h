#pragma once
#include <vector>
#include <iostream>
#include "CellType.h"

class GameMap {
private:
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_level;
    std::vector<std::vector<CellType>> m_grid;

public:
    GameMap(uint32_t width, uint32_t height, uint8_t level);

    void Initialize();

    void CreateWallCluster(int startX, int startY, CellType wallType);

    void Display() const;

    std::vector<std::vector<CellType>>& GetMap();
    const std::vector<std::vector<CellType>>& GetMap() const;

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
};
