#include <iostream>
#include "Character.h"

//Default constructor.
Character::Character()
	: m_direction{ 0, 0 }, m_numberOfBulletsReceived{ 0 }, m_shotEnemies{ 0 }, m_position{ 0,0 }
{
}

//Constructor.
Character::Character(const Point& direction, const Point& position)
	: m_direction{ direction }, m_numberOfBulletsReceived{ 0 }, m_shotEnemies{ 0 }, m_position{ position }
{
}

//"MoveCharacter" moves the character in the specified direction.
void Character::MoveCharacter(const Point& direction) {
	m_position += direction;
}

//"ReceivedBullet" counts the received bullets.
void Character::ReceivedBullet() {
	m_numberOfBulletsReceived++;
}

//"ShotEnemy" counts the enemies that were shot.
void Character::ShotEnemy() {
	m_shotEnemies++;
}

//"SetPosition" sets the position of the character.
void Character::SetPosition(const Point& position) {
	m_position = position;
}

//"GetPosition" returns the current position.
Point Character::GetPosition() const {
	return m_position;
}

//"GetCoordonateX" returns the x coordonate of the current position.
uint8_t Character::GetCoordonateX() {
	return m_position.GetX();
}

//"GetY" returns the y coordonate of the current position.
uint8_t Character::GetCoordonateY() {
	return m_position.GetY();
}
