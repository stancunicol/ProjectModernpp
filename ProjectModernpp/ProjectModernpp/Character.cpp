#include "Character.h"

Character::Character(const Point& direction, const Point& position)
	: m_position{ position }, m_direction{ direction }, m_lives{ 3 } {
}

void Character::MoveCharacter(const Point& direction) {
	m_position = m_position + direction;
}

Point Character::GetPosition() const {
	return m_position;
}

Point Character::GetDirection() const {
	return m_direction;
}

void Character::SetPosition(const Point& point) {
	m_position = point;
}

void Character::SetDirection(const Point& point)
{
	m_direction = point;
}

uint8_t Character::GetLives() const
{
	return m_lives;
}

void Character::SetLives(uint8_t lives)
{
	m_lives = lives;
}

void Character::DecreaseLives(uint8_t amount) {
	if (m_lives > amount)
		m_lives -= amount;
	else m_lives = 0;
}

bool Character::isDead() const {
	return m_lives == 0;
}
