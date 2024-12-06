#pragma once
#include "Point.h"

class Character {
protected:
    Point m_position;
    Point m_direction;

public:
    Character(const Point& direction = Point(0, 0), const Point& position = Point(0, 0)); //constructor

    virtual void PlaceCharacter() = 0;

    virtual void MoveCharacter(const Point& direction); //moves the character in the mentioned direction

    Point GetPosition() const;

    Point GetDirection() const;

    void SetPosition(const Point& point);
};
