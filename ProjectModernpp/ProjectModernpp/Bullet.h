#pragma once
#include "Point.h"

class __declspec(dllexport) Bullet {
private:
    bool m_active;//the bullet is activated or not
    Point m_direction;//the direction of the bullet
    float m_speed;//the speed of the bullet
    Point m_position;//the current position of the bullet

public:
    Bullet(bool active = false, float speed = 0.25f, const Point& direction = Point(0, 0), const Point& position = Point(0, 0))//constructor
        : m_active(active), m_speed(speed), m_direction(direction), m_position(position) {}

    bool IsActive() const { return m_active; }//returns if the bullet is activated or not
    void SetActive(bool active) { m_active = active; }//activates the bullet

    Point GetDirection() const { return m_direction; }//returns the direction of the bullet

    Bullet Shoot(const Point& direction) {//shoots in the mentioned direction
        return Bullet(true, 0.25f, direction);
    }

    Point GetPosition() const { return m_position; }

    void Move(GameMap& map) {
        if (!m_active)
            return;
        if (m_position.GetX() >= 0 && m_position.GetX() < map.GetHeight() &&
            m_position.GetY() >= 0 && m_position.GetY() < map.GetWidth()) {
            if (map.GetMap()[m_position.GetX()][m_position.GetY()] == CellType::BULLET)
                map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
        }

        Point newPos = m_position + m_direction;

        if (newPos.GetX() < 0 || newPos.GetX() >= map.GetHeight() ||
            newPos.GetY() < 0 || newPos.GetY() >= map.GetWidth()) {
            m_active = false;
            return;
        }

        if (map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::BREAKABLE_WALL) {
            map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::EMPTY;
            m_active = false;
            return;
        }
        if (map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::UNBREAKABLE_WALL) {
            m_active = false;
            return;
        }

        m_position = newPos;
        map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::BULLET;
    }
};
