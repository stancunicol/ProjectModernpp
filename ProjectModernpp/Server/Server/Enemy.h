#pragma once
#include "..\RandomMove\RandomMove.h"

#include <cstdlib>
#include <unordered_map>
#include <deque>
#include "Character.h"
#include "GameMap.h"

class Enemy : virtual public Character
{
private:
    RandomMove randomMove;
    
    bool m_active = false;

    Point m_shootDirection = Point(0, 0);

    static std::unordered_map<size_t, std::deque<Point>> enemyLastMoves;

public:
    Enemy(GameMap& map);

    Enemy();

    void PlaceCharacter(GameMap& map);

    void MoveRandom(GameMap& grid);

    const Point& GetPosition() const;

    const Point& GetShootDirection() const;

};
