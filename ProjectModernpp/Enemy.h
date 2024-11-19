#pragma once
#include <cstdlib>
#include "Character.h"
#include "GameMap.h"

class Enemy : virtual public Character {
private:
    GameMap& m_map; //references the game map

public:
    Enemy(GameMap& map);//"puts" the enemies in the game map

    void PlaceCharacter() override;//function from "Character.h"; places the character at a position

    void MoveRandom();//moves the enemies in random movements

    const Point& GetPosition() const;//return the current position
};
