#include "Enemy.h"

Enemy::Enemy(GameMap& map)
	: Character(Point(0, 0), Point(0, 0)), m_map(map) {}

void Enemy::PlaceCharacter() {
    uint16_t startX, startY;
    do {
        startX = rand() % m_map.GetHeight();
        startY = rand() % m_map.GetWidth();
    } while (m_map.GetMap()[startX][startY] != CellType::EMPTY);

    m_position = Point(startX, startY);
    m_map.GetMap()[startX][startY] = CellType::ENEMY;
}

void Enemy::MoveRandom() {
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

const Point& Enemy::GetPosition() const { 
    return m_position; 
}
