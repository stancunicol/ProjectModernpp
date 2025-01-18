#pragma once

import Point;
#include <vector>
#include <iostream>
#include "CellType.h"

class  GameMap {
private:
    uint8_t m_width;
    uint8_t m_height;
    uint8_t m_level;

    std::vector<std::vector<CellType>> m_dynamicGrid;
    std::vector<std::vector<CellType>> m_grid;

    void MatrixSizeGenerator();

public:
    GameMap(uint8_t level = 1);

    GameMap(GameMap&& other);

    void Initialize();//initialize the map based on the level
    bool IsInSafeZone(uint8_t x, uint8_t y, uint8_t safeZoneSize);
    bool HasEmptyNeighbors(uint8_t x, uint8_t y);

    void Display() const;//displays the map

    void ClearDynamicEntities();

    void SetStaticCell(uint8_t x, uint8_t y, CellType type);

    std::vector<std::vector<CellType>>& GetMap();//returns the map
    uint8_t GetWidth() const;
    uint8_t GetHeight() const;
    uint8_t GetLevel() const;

    void Reset(uint8_t level);

    std::vector<CellType>& operator[](int index) {
        return m_grid[index];
    }

    const std::vector<CellType>& operator[](int index) const {
        return m_grid[index];
    }
};
