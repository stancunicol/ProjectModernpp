#include "Player.h"

Player::Player(const std::string& name)
	: Character({ 0, 1 }, { 0, 0 }), m_points{ 0 }, m_score{ 0 }, m_waitingTime{ 4 }, m_name{ name }
{
}

//Shoots the bullet.
void Player::ShootBullet() {
	Point direction = m_direction;
	m_weapon.Shoot(direction);
}
