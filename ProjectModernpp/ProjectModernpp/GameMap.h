#pragma once
#include <vector>
#include <iostream>
#include "CellType.h"
#include "Point.h"

class  GameMap {
private:
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_level;

    std::vector<std::vector<CellType>> m_dynamicGrid;
    std::vector<std::vector<CellType>> m_grid;

public:
    GameMap(uint32_t width, uint32_t height, uint8_t level);


    void Initialize();//initialize the map based on the level
    bool IsInSafeZone(int x, int y, int safeZoneSize);
    bool HasEmptyNeighbors(int x, int y);

    void Display() const;//displays the map

    void ClearDynamicEntities();

    void DrawEntity(const Point& position, char symbol);

    void SetStaticCell(int x, int y, CellType type);

    std::vector<std::vector<CellType>>& GetMap();//returns the map
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    uint32_t GetLevel() const;


};
