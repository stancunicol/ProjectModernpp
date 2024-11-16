#include <iostream>
#include "Bullet.h"

//Default constructor.
Bullet::Bullet()
	: m_active{ false }, m_direction{ 0, 0 }, m_speed{ 0.25f }
{
}

//Constructor.
Bullet::Bullet(const bool& active, const float& speed, const Point& direction)
	: m_active{ active }, m_speed{ speed }, m_direction{ direction }
{
}

//"Shoot" creates a bullet in the direction the character shoots.
Bullet Bullet::Shoot(const Point& direction) {
	return Bullet(true, 0.25f, m_direction);
}