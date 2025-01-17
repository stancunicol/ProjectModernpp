#include "Enemy.h"
#include <algorithm>
#include <random>

Enemy::Enemy(GameMap& map) //constructor
    : Character{ Point(0, 0), Point(0, 0) }
{
    PlaceCharacter(map);
}


void Enemy::PlaceCharacter(GameMap& map)
{
    uint16_t startX, startY;
    do {
        startX = rand() % map.GetHeight();
        startY = rand() % map.GetWidth();
    } while (map.GetMap()[startX][startY] != CellType::EMPTY);

    m_position = Point(startX, startY);
}

void Enemy::MoveRandom(GameMap& map)
{
    RandomMove randomMove; 
    std::vector<Point> validPositions; 

    for (size_t i = 0; i < 4; ++i) { 
        auto direction = randomMove.GenerateNextDirection(); 
        Point newPos = m_position + Point(direction.first, direction.second); 

        if (newPos.GetX() >= 0 && newPos.GetX() < map.GetHeight() &&
            newPos.GetY() >= 0 && newPos.GetY() < map.GetWidth() &&
            map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {

            if (std::ranges::find(validPositions, newPos) == validPositions.end()) {
                validPositions.push_back(newPos);
            }
        }
    }

    m_shootDirection = Point(0, 0);

    if (!validPositions.empty()) {
        int randomIndex = rand() % validPositions.size();
        Point randomFreePos = validPositions[randomIndex];
        m_shootDirection = randomFreePos - m_position;
        map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
        m_position = randomFreePos; 
    }
}

void Enemy::SetActive(const bool& active) //set the bullet active or desactivates it
{
    m_active = active;
}

const Point& Enemy::GetPosition() const
{
    return m_position;
}

const Point& Enemy::GetShootDirection() const
{
    return m_shootDirection;
}