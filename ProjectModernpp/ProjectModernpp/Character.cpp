#include "Character.h"

Character::Character(const Point& direction, const Point& position)
	: m_position(position), m_direction(direction) {
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
