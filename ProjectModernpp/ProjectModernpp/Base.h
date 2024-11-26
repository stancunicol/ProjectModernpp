#pragma once
#include "GameMap.h"
#include "CellType.h"

// Forward declaration of GameMap to avoid circular dependency
class GameMap;

class  Base {
private:
    bool m_destroyed;   // Checks if the base is destroyed
    GameMap& m_gameMap; // Reference to the game map

public:
    // Constructor
    explicit Base(GameMap& gameMap)
        : m_gameMap(gameMap), m_destroyed(false) {}

    // Places the base on the game map
    void PlaceBase() {
        const int centerX = m_gameMap.GetHeight() - 1; // Bottom row
        const int centerY = m_gameMap.GetWidth() / 2;  // Middle column

        // Access the map grid directly
        auto& map = m_gameMap.GetMap();
        map[centerX][centerY] = CellType::BASE;
        map[centerX][centerY - 1] = CellType::BREAKABLE_WALL;
        map[centerX][centerY + 1] = CellType::BREAKABLE_WALL;
        map[centerX - 1][centerY - 1] = CellType::BREAKABLE_WALL;
        map[centerX - 1][centerY] = CellType::BREAKABLE_WALL;
        map[centerX - 1][centerY + 1] = CellType::BREAKABLE_WALL;
    }

    // Marks the base as destroyed
    void Destroyed() { m_destroyed = true; }

    // Checks if the base is destroyed
    [[nodiscard]] bool IsDestroyed() const { return m_destroyed; }
};
