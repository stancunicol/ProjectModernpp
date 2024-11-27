#pragma once
#include "Point.h"
#include "GameMap.h"
#include <chrono>

class Bullet {
private:
    bool m_active;                // Glonțul este activ sau nu
    Point m_direction;            // Direcția glonțului
    float m_speed;                // Viteza glonțului (celule pe secundă)
    Point m_position;             // Poziția curentă a glonțului
    std::chrono::steady_clock::time_point m_lastMoveTime; // Ultima actualizare

public:
    Bullet(bool active = false, float speed = 0.25f, const Point& direction = Point(0, 0), const Point& position = Point(0, 0))
        : m_active(active), m_speed(speed), m_direction(direction), m_position(position) {
        m_lastMoveTime = std::chrono::steady_clock::now(); // Inițializare timp
    }

    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

    Point GetPosition() const { return m_position; }
    Point GetDirection() const { return m_direction; }
    void SetPosition(const Point& position) { m_position = position; }

    // Actualizează poziția pe baza timpului
    void Move(GameMap& map) {
        if (!m_active) return;

        Point newPos = m_position + m_direction;

        // Coliziuni sau ieșire din limite
        if (newPos.GetX() < 0 || newPos.GetX() >= map.GetHeight() ||
            newPos.GetY() < 0 || newPos.GetY() >= map.GetWidth() ||
            map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::UNBREAKABLE_WALL ||
            map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::BREAKABLE_WALL) {
            m_active = false;
            return;
        }

        // Mutăm glonțul
        map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
        m_position = newPos;
        map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::BULLET;
    }

};
