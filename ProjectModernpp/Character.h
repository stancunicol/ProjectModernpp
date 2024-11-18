#pragma once
#include "Point.h"

class Character {
protected:
    Point m_position;
    Point m_direction;

public:
    Character(const Point& direction = Point(0, 0), const Point& position = Point(0, 0))
        : m_position(position), m_direction(direction) {}

    virtual void PlaceCharacter() = 0;

    virtual void MoveCharacter(const Point& direction) {
        m_position = m_position + direction;
    }

    Point GetPosition() const { return m_position; }
    Point GetDirection() const { return m_direction; }

    void SetPosition(const Point& point) { m_position = point; }
};
