#include "Enemy.h"
#include <algorithm>
#include <random>

Enemy::Enemy(GameMap& map) //constructor
    : Character{ Point(0, 0), Point(0, 0) }
{
    PlaceCharacter(map);
}

Enemy::Enemy()
{
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

std::vector<std::pair<int8_t, int8_t>> lastMoves;

void Enemy::MoveRandom(GameMap& map)
{
    std::vector<Point> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
    std::vector<Point> validPositions;

    for (const auto& direction : directions) {
        Point newPos = m_position + Point(direction.GetX(), direction.GetY());

        if (newPos.GetX() >= 0 && newPos.GetX() < map.GetHeight() &&
            newPos.GetY() >= 0 && newPos.GetY() < map.GetWidth()) {

            CellType cellType = map.GetMap()[newPos.GetX()][newPos.GetY()];

            if (cellType != CellType::BREAKABLE_WALL && cellType != CellType::UNBREAKABLE_WALL) {
                if (cellType == CellType::EMPTY) {
                    validPositions.push_back(newPos);
                    std::cout << static_cast<int>(cellType) << " ";
                }
            }
        }
    }

    m_shootDirection = Point(0, 0);

    if (!validPositions.empty()) {
        int randomIndex = rand() % validPositions.size();
        Point randomFreePos = validPositions[randomIndex];
        m_shootDirection = randomFreePos - m_position;

        std::cout << "Enemy moving from (" << static_cast<int>(m_position.GetX()) << ", " << static_cast<int>(m_position.GetY()) << ") to ("
            << static_cast<int>(randomFreePos.GetX()) << ", " << static_cast<int>(randomFreePos.GetY()) << '\n';

        //map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
        //if(map.GetMap()[randomFreePos.GetX()][randomFreePos.GetY()] == CellType::EMPTY)
        m_position = randomFreePos;
    }
    else {
        std::cout << "No valid positions available for movement!\n";
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

/*void Enemy::Update(float deltaTime) {
    m_timeSinceLastMove += deltaTime;
    if (m_timeSinceLastMove >= 1000) {
        MoveRandom(map);

    }
}*/
