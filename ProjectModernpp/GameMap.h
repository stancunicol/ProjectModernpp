#pragma once
#include "CellType.h"
#include "Character.h"
#include "Player.h"
#include <vector>
#include <ctime>
#include <iostream>

class GameMap {
private:
    uint32_t m_width;
    uint32_t m_height;
    uint8_t m_level; // easy = 1, medium = 2, hard = 3

    Player* player;

    // 2D grid representing the map layout
    std::vector<std::vector<CellType>> m_grid;
public:
    //Default constructor.
    GameMap();

    // Constructor
    GameMap(uint32_t width, uint32_t height, uint8_t level);

    // Initializes the grid with random cell types; default - Empty
    void initialize();
    void createWallCluster(int startX, int startY, CellType wallType);

    bool isPositionFree(int x, int y);
    void placePlayer(Player* p);

    // Display the map
    void display() const;

    std::vector<std::vector<CellType>> GetMap() const;

    uint32_t GetWidth();

    uint32_t GetHeight();
};
