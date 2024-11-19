#pragma once
#include "Point.h"

class __declspec(dllexport) Bullet {
private:
    bool m_active;//the bullet is activated or not
    Point m_direction;//the direction of the bullet
    float m_speed;//the speed of the bullet

public:
    Bullet(bool active = false, float speed = 0.25f, const Point& direction = Point(0, 0))//constructor
        : m_active(active), m_speed(speed), m_direction(direction) {}

    bool IsActive() const { return m_active; }//returns if the bullet is activated or not
    void SetActive(bool active) { m_active = active; }//activates the bullet

    Point GetDirection() const { return m_direction; }//returns the direction of the bullet

    Bullet Shoot(const Point& direction) {//shoots in the mentioned direction
        return Bullet(true, 0.25f, direction);
    }
};
