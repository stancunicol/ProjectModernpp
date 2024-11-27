#pragma once
#include <vector>
#include <iostream>
#include "CellType.h"
#include "Character.h"

class Bullet;

class  GameMap {
private:
    uint32_t m_width;//the width of the game map
    uint32_t m_height;//the height of the game map
    uint32_t m_level;//the chosen level
    std::vector<std::vector<CellType>> m_grid;//the matrix of the game, with the celltypes specified in the CellType enum class
    std::vector<Bullet> m_bullets;

public:
    GameMap(uint32_t width, uint32_t height, uint8_t level);//constructor


    void Initialize();//initialize the map based on the level
    bool IsInSafeZone(int x, int y, int safeZoneSize);
    bool HasEmptyNeighbors(int x, int y);

    //void CreateWallCluster(int startX, int startY, CellType wallType);//creates clusters for the cell type

    void Display() const;//displays the map

    std::vector<std::vector<CellType>>& GetMap();//returns the map

    uint32_t GetWidth() const;//returns the width of the map
    uint32_t GetHeight() const;//returns the height of the map
    void AddBullet(const Bullet& bullet);
};
