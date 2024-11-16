#include "Enemy.h"
#include "Point.h"

Enemy::Enemy()
{
}

Enemy::Enemy(const Point& position)
{
}

//Constructor.
Enemy::Enemy(const Point& position, GameMap gameMap)
	: Character(Point(0, 0), position), m_gameMap(gameMap), m_active{true}
{
	srand(time(NULL));
}

void Enemy::RandomMovement() {
	int generate = rand() % 4;//Generate the four directions.
	Point direction;
	switch (generate)
	{
	case 0:
		direction = Point(0, -1);//Up.
		break;
	case 1:
		direction = Point(0, 1);//Down.
		break;
	case 2:
		direction = Point(-1, 0);//Left.
		break;
	case 3:
		direction = Point(1, 0);//Right.
		break;
	}
	Point newPosition = m_position + direction;//The position the enemy will move to.
	if (newPosition.GetX() >=0 && newPosition.GetX() < m_gameMap.GetHeight() && newPosition.GetY() >= 0 && newPosition.GetY() < m_gameMap.GetWidth() && m_gameMap.isPositionFree(newPosition.GetX(), newPosition.GetY())) {
		m_position = newPosition;
		m_direction = direction;
	}
}

Bullet Enemy::Shoot() {
	if(!m_active)
		return Bullet();
	return Bullet(true, 0.25f, m_direction);
}

void Enemy::IsShot() {
	m_active = false;
}

void Enemy::IsActive() {
	m_active = true;
}

Point Enemy::GetPosition() const {
	return m_position;
}
