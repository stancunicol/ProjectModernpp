#include "Base.h"

Base::Base(GameMap& gameMap)
    : m_gameMap(gameMap), m_destroyed(false) {
}

void Base::PlaceBase() {
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

void Base::Destroyed() {
    m_destroyed = true;
}

bool Base::IsDestroyed() const
{
    return m_destroyed;
}
