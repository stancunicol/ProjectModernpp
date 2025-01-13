#pragma once

import Point;
#include <vector>
#include <iostream>
#include "CellType.h"

class  GameMap {
private:
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_level;

    std::vector<std::vector<CellType>> m_dynamicGrid;
    std::vector<std::vector<CellType>> m_grid;

    void MatrixSizeGenerator();

public:
    GameMap(uint8_t level = 1);


    void Initialize();//initialize the map based on the level
    bool IsInSafeZone(int x, int y, int safeZoneSize);
    bool HasEmptyNeighbors(int x, int y);

    void Display() const;//displays the map

    void ClearDynamicEntities();

    void SetStaticCell(int x, int y, CellType type);

    std::vector<std::vector<CellType>>& GetMap();//returns the map
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    uint32_t GetLevel() const;

    void Reset(uint8_t level);
};
