#include "Enemy.h"
#include <algorithm>

Enemy::Enemy(Game& game, GameMap& map)
    : Character(Point(0, 0), Point(0, 0)), m_game(game), m_map(map) {}

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
    std::vector<Point> directions { 
        Point(-1, 0), //up
        Point(1, 0), //down
        Point(0, -1), //left
        Point(0, 1)};//right
    std::random_shuffle(directions.begin(), directions.end());

    std::vector<Point> validPositions;
    for (const auto& direction : directions) {
        Point newPos = m_position + direction;
        if (newPos.GetX() >= 0 && newPos.GetX() < m_map.GetHeight() &&
            newPos.GetY() >= 0 && newPos.GetY() < m_map.GetWidth() &&
            m_map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {
            validPositions.push_back(newPos);
        }
    }

    if (!validPositions.empty()) {
        int randomIndex = rand() % validPositions.size();
        Point randomFreePos = validPositions[randomIndex];

        m_map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
        m_position = randomFreePos;
        m_map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::ENEMY; // Plasăm jucătorul pe noua poziție
        Shoot(m_game);
    }
}

const Point& Enemy::GetPosition() const {
    return m_position;
}

void Enemy::Shoot(Game& game) {
    std::vector<Point> directions{
        Point(-1, 0), //up
        Point(1, 0), //down
        Point(0, -1), //left
        Point(0, 1) };//right
    std::random_shuffle(directions.begin(), directions.end());

    Bullet bullet = m_weapon.Shoot(directions[0]);
    game.AddBullet(bullet);
}
