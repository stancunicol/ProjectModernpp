#pragma once
#include "Point.h"
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
    bool CanMove() {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - m_lastMoveTime;

        if (elapsed.count() >= (1.0f / m_speed)) {
            m_lastMoveTime = now;
            return true;
        }
        return false;
    }
};
