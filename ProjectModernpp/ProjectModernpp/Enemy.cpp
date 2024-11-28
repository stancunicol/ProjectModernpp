#include "Enemy.h"
#include <algorithm>
#include <chrono>

Enemy::Enemy(Game& game, GameMap& map) //constructor
    : Character(Point(0, 0), Point(0, 0)), m_game(game), m_map(map),
      m_activeBullet(false, 0.25f, Point(0,0), Point(0,0))
{
    m_lastShoot = std::chrono::steady_clock::now();  //we shoot the first bullet and containes the time 
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
    std::random_shuffle(directions.begin(), directions.end());//we shuffle the elements of the vector so the order is different every time

    std::vector<Point> validPositions; //here we keep the valid positions that an enemy can move to
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

        Point currentPos = m_position;
        m_map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY; //we set the previous cell empty
        m_position = randomFreePos; //we update the current position
        m_map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::ENEMY; // and we place the enemy on the current position
        Point directionToShoot = randomFreePos - currentPos; //we calculate the direction to shoot
        Shoot(directionToShoot);
    }
}

const Point& Enemy::GetPosition() const { //returns the current position
    return m_position;
}

//the enemy shoots a bullet
void Enemy::Shoot(const Point& direction) {
    if (direction == Point(0,0)) //if the direction is a null one, there's nothing to do
        return;

    if (m_activeBullet.IsActive()) //if the bullet is not active, there's nothing to do
        return;

    auto now = std::chrono::steady_clock::now(); //we update the shootinf of the bullet
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastShoot);  //we calculate the distance between the bullet shot now and
    //the previous one

    if (elapsed.count() >= 1) { //if the distance is bigger than one second, we shoot
        Bullet bullet(true, 0.25f, direction, m_position);
        m_game.AddBullet(bullet);
        m_lastShoot = now;
    }
}

void Enemy::SetActive(const bool& active) { //set the bullet active or desactivates it
    if (m_active == active)
        return;
    m_active = active;
}
