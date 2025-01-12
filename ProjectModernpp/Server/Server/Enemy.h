#pragma once
#include "..\RandomMove\RandomMove.h" 
#include <cstdlib>
#include "Character.h"
#include "GameMap.h"

class Enemy : virtual public Character
{
private:
    bool m_active = false;
    Point m_shootDirection = Point(0, 0);

public:
    Enemy(GameMap& map);//"puts" the enemies in the game map

    void PlaceCharacter(GameMap& map);

    void MoveRandom(GameMap& grid);

    void SetActive(const bool& active);  //sets an enemy active

    const Point& GetPosition() const;
    const Point& GetShootDirection() const;
};
