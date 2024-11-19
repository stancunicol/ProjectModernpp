#pragma once
#include "Point.h"

class __declspec(dllexport) Character {
protected:
    Point m_position;//the position of the character
    Point m_direction;//the direction the character is moving to

public:
    Character(const Point& direction = Point(0, 0), const Point& position = Point(0, 0))//constructor
        : m_position(position), m_direction(direction) {}

    virtual void PlaceCharacter() = 0;

    virtual void MoveCharacter(const Point& direction) {//moves the character in the mentioned direction
        m_position = m_position + direction;
    }

    Point GetPosition() const { return m_position; }//returns the current position
    Point GetDirection() const { return m_direction; }//returns the current direction

    void SetPosition(const Point& point) { m_position = point; }//sets the position at the mentioned position
};
