#pragma once
#include <cstdlib>
#include "Character.h"
#include "GameMap.h"

class Enemy : virtual public Character {
private:
    GameMap& m_map; // Referință la harta jocului

public:
    Enemy(GameMap& map);

    void PlaceCharacter() override;

    void MoveRandom();

    const Point& GetPosition() const;
};
