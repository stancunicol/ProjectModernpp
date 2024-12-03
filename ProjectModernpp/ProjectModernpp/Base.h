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
    explicit Base(GameMap& gameMap);

    // Places the base on the game map
    void PlaceBase();

    // Marks the base as destroyed
    void Destroyed();

    // Checks if the base is destroyed
    [[nodiscard]] bool IsDestroyed() const;
};
