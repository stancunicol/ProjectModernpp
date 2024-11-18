#pragma once
#include <cstdlib>
#include "Character.h"
#include "GameMap.h"

class Enemy : virtual public Character {
private:
    GameMap& m_map; // Referință la harta jocului

public:
    Enemy(GameMap& map) : Character(Point(0, 0), Point(0, 0)), m_map(map) {}

    void PlaceCharacter() override {
        uint16_t startX, startY;
        do {
            startX = rand() % m_map.GetHeight();
            startY = rand() % m_map.GetWidth();
        } while (m_map.GetMap()[startX][startY] != CellType::EMPTY);

        m_position = Point(startX, startY);
        m_map.GetMap()[startX][startY] = CellType::ENEMY;
    }

    void MoveRandom() {
        int dx = rand() % 3 - 1;
        int dy = rand() % 3 - 1;
        Point newPos = Point(m_position.GetX() + dx, m_position.GetY() + dy);

        // Validare mutare
        if (newPos.GetX() >= 0 && newPos.GetX() < m_map.GetHeight() &&
            newPos.GetY() >= 0 && newPos.GetY() < m_map.GetWidth() &&
            m_map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {
            m_map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
            m_position = newPos;
            m_map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::ENEMY;
        }
    }

    const Point& GetPosition() const { return m_position; }
};
