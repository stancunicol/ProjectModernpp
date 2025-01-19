#pragma once

import Point;
#include <vector>
#include <iostream>
#include "CellType.h"

class  GameMap 
{
private:
    uint8_t m_width;

    uint8_t m_height;
    
    uint8_t m_level;

    std::vector<std::vector<CellType>> m_grid;

    void MatrixSizeGenerator();

public:
    GameMap(uint8_t level = 1);

    GameMap(GameMap&& other);

    void Initialize();

    bool IsInSafeZone(uint8_t x, uint8_t y, uint8_t safeZoneSize);
    
    bool HasEmptyNeighbors(uint8_t x, uint8_t y);

    void Reset(uint8_t level);


    void SetStaticCell(uint8_t x, uint8_t y, CellType type);


    uint8_t GetWidth() const;

    uint8_t GetHeight() const;
    
    uint8_t GetLevel() const;

    std::vector<std::vector<CellType>>& GetMap();
    

    std::vector<CellType>& operator[](int index);

    const std::vector<CellType>& operator[](int index) const;
};
