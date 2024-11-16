#include "Enemy.h"

//Constructor.
Enemy::Enemy(const Point& position)
	: Character({ 0, 0 }, position), m_active(true)
{
}

void Enemy::RandomMovement() {
	int generate = rand() % 4;//Generate the four directions.
	Point direction;
	switch (generate)
	{
	case 0:
		generate = ( uint8_t(0), uint8_t(-1) );//Up.
		break;
	case 1:
		generate = (uint8_t(0), uint8_t(1));//Down.
		break;
	case 2:
		generate = (uint8_t(-1), uint8_t(0));//Left.
		break;
	case 3:
		generate = (uint8_t(1), uint8_t(0));//Right.
		break;
	}
	Point newPosition = m_position + direction;//The position the enemy will move to.
	if (gameMap.isPositionFree(newPosition.GetX(), newPosition.GetY())) {
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
