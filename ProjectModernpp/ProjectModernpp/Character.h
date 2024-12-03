#pragma once
#include "Point.h"

class Character {
protected:
    Point m_position;//the position of the character
    Point m_direction;//the direction the character is moving to

public:
    Character(const Point& direction = Point(0, 0), const Point& position = Point(0, 0));//constructor

    virtual void PlaceCharacter() = 0;

    virtual void MoveCharacter(const Point& direction);//moves the character in the mentioned direction

    Point GetPosition() const;//returns the current position

    Point GetDirection() const;//returns the current direction

    void SetPosition(const Point& point);//sets the position at the mentioned position
};
