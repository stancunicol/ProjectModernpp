#include "Enemy.h"
#include <algorithm>
#include <random>

Enemy::Enemy(GameMap& map) //constructor
    : Character(Point(0, 0), Point(0, 0))  {}


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
    std::vector<Point> directions{
        Point(-1, 0), //up
        Point(1, 0), //down
        Point(0, -1), //left
        Point(0, 1) };//right
    
    //we shuffle the elements of the vector so the order is different every time
    std::shuffle(directions.begin(), directions.end(), std::mt19937(std::random_device{}()));

    std::vector<Point> validPositions; //here we keep the valid positions that an enemy can move to

    for (const auto& direction : directions) {
        Point newPos = m_position + direction; //we initialize the new position
        //and check if it is correct and if is an empty cell; if so, the position is valid
        if (newPos.GetX() >= 0 && newPos.GetX() < map.GetHeight() &&
            newPos.GetY() >= 0 && newPos.GetY() < map.GetWidth() &&
            map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {
            validPositions.push_back(newPos);
        }
    }
    m_shootDirection = Point(0, 0);
    if (!validPositions.empty()) {  //if we have valid positions
        int randomIndex = rand() % validPositions.size();  //we take a random index so we can take a random valid position from the vector
        Point randomFreePos = validPositions[randomIndex];
        m_shootDirection = randomFreePos - m_position;
        map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY; //we set the previous cell empty
        m_position = randomFreePos; //we update the current position
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