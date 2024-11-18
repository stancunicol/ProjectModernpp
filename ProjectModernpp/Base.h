#pragma once
#include "GameMap.h"

class Base : public GameMap {
private:
    bool m_destroyed;

public:
    Base( const GameMap& gameMap)
        : GameMap(gameMap),m_destroyed{ false } {}

    void Destroyed() { m_destroyed = true; }
    bool IsDestroyed() const { return m_destroyed; }
};
