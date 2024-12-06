#include "Enemy.h"
#include <algorithm>
#include <random>

Enemy::Enemy(Game& game, GameMap& map) //constructor
    : Character(Point(0, 0), Point(0, 0)), m_game(game), m_map(map),
      m_activeBullet( Point(0,0), Point(0,0)){
}

void Enemy::PlaceCharacter() {
    //we place every enemy on a random position
    uint16_t startX, startY;
    do {
        startX = rand() % m_map.GetHeight();
        startY = rand() % m_map.GetWidth();
    } while (m_map.GetMap()[startX][startY] != CellType::EMPTY);

    m_position = Point(startX, startY);
    //we put the enemy on the map
    m_map.GetMap()[startX][startY] = CellType::ENEMY;
}

void Enemy::MoveRandom() {
    //we initialize the possible directions:
    std::vector<Point> directions { 
        Point(-1, 0), //up
        Point(1, 0), //down
        Point(0, -1), //left
        Point(0, 1)};//right
    //we shuffle the elements of the vector so the order is different every time
    std::shuffle(directions.begin(), directions.end(), std::mt19937(std::random_device{}()));

    std::vector<Point> validPositions; //here we keep the valid positions that an enemy can move to
    Point shootingDirection = Point(0, 0);
    for (const auto& direction : directions) {
        Point newPos = m_position + direction; //we initialize the new position
        //and check if it is correct and if is an empty cell; if so, the position is valid
        if (newPos.GetX() >= 0 && newPos.GetX() < m_map.GetHeight() &&
            newPos.GetY() >= 0 && newPos.GetY() < m_map.GetWidth() &&
            m_map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {
            validPositions.push_back(newPos);
        }
    }

    if (!validPositions.empty()) {  //if we have valid positions
        int randomIndex = rand() % validPositions.size();  //we take a random index so we can take a random valid position from the vector
        Point randomFreePos = validPositions[randomIndex];
        shootingDirection = randomFreePos - m_position;
        m_map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY; //we set the previous cell empty
        m_position = randomFreePos; //we update the current position
        m_map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::ENEMY; // and we place the enemy on the current position
        Shoot(shootingDirection);
    }
    else {
        m_map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::ENEMY;
    }
}


//the enemy shoots a bullet
void Enemy::Shoot(const Point& direction) {
    if (direction == Point(0,0)) //if the direction is a null one, there's nothing to do
        return;

    if (m_activeBullet.IsActive()) //if the bullet is not active, there's nothing to do
        return;

    Point newPosition = m_position + direction;
    if (newPosition.GetX() >= 0 && newPosition.GetX() < m_map.GetHeight() &&
        newPosition.GetY() >= 0 && newPosition.GetY() < m_map.GetWidth() &&
        m_map.GetMap()[newPosition.GetX()][newPosition.GetY()] == CellType::EMPTY) {
        Bullet bullet(direction, m_position);
        m_game.AddBullet(bullet);
        m_activeBullet = bullet;
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