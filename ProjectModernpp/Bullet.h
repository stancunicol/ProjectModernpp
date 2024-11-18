#pragma once
#include "Point.h"

class Bullet {
private:
    bool m_active;
    Point m_direction;
    float m_speed;

public:
    Bullet(bool active = false, float speed = 0.25f, const Point& direction = Point(0, 0))
        : m_active(active), m_speed(speed), m_direction(direction) {}

    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

    Point GetDirection() const { return m_direction; }

    Bullet Shoot(const Point& direction) {
        return Bullet(true, 0.25f, direction);
    }
};
