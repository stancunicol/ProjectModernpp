#pragma once
#include "GameMap.h"

class Base : public GameMap {
private:
    bool m_destroyed;//checks if the base is destroyed

public:
    Base( const GameMap& gameMap)//constructor
        : GameMap{ gameMap }, m_destroyed{ false } {}

    void Destroyed() { m_destroyed = true; }//the base is destroyed
    bool IsDestroyed() const { return m_destroyed; }//returns if the base is destroyed
};