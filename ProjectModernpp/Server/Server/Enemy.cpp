#include "Enemy.h"
#include <algorithm>
#include <random>

Enemy::Enemy(GameMap& map) 
    : Character{ Point(0, 0), Point(0, 0) }
{
    PlaceCharacter(map);
}

Enemy::Enemy() {}

void Enemy::PlaceCharacter(GameMap& map)
{
    uint16_t startX, startY;
    do {
        startX = rand() % map.GetHeight();
        startY = rand() % map.GetWidth();
    } while (map.GetMap()[startX][startY] != CellType::EMPTY);

    m_position = Point(startX, startY);
}

std::unordered_map<size_t, std::deque<Point>> Enemy::enemyLastMoves;

void Enemy::MoveRandom(GameMap& grid)
{
    std::vector<Point> validPositions;

    auto& lastMoves = enemyLastMoves[reinterpret_cast<size_t>(this)];

    for (size_t i = 0; i < 4; ++i)
    {
        auto direction = randomMove.GenerateNextDirection();
        Point newPos = m_position + Point(direction.first, direction.second);

        if (newPos.GetX() >= 0 && newPos.GetX() < grid.GetHeight() &&
            newPos.GetY() >= 0 && newPos.GetY() < grid.GetWidth() &&
            grid.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::EMPTY)
        {
            if (std::find(validPositions.begin(), validPositions.end(), newPos) == validPositions.end())
            {
                validPositions.push_back(newPos);
            }
        }
    }

    m_shootDirection = Point(0, 0);

    if (!validPositions.empty())
    {
        if (lastMoves.size() >= 3) 
        {
            validPositions.erase(std::remove_if(validPositions.begin(), validPositions.end(), [&](const Point& pos) {
                return pos == lastMoves[0] || pos == lastMoves[1] || pos == lastMoves[2];
                }), validPositions.end());
        }

        if (validPositions.empty() && !lastMoves.empty()) 
        {
            validPositions.push_back(lastMoves.back());
        }

        int randomIndex = rand() % validPositions.size();
        Point randomFreePos = validPositions[randomIndex];
        m_shootDirection = randomFreePos - m_position;

        grid.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
        m_position = randomFreePos;

        lastMoves.push_back(randomFreePos);
        if (lastMoves.size() > 3) 
        {
            lastMoves.pop_front(); 
        }
    }
    else {
        std::cout << "No valid positions available for movement!\n";
    }
}

const Point& Enemy::GetPosition() const
{
    return m_position;
}

const Point& Enemy::GetShootDirection() const
{
    return m_shootDirection;
}