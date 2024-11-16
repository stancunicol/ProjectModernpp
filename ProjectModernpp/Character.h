#pragma once
#include <iostream>
#include "Bullet.h"
#include "Point.h"

class Character {
protected:

	Point m_direction;//The coordonates represent the direction of movement of the character.
	//(first, second) can be (0,1) - the character is going up, (0,-1) - the character is going down
	//(1,0) - the character is going right, (-1,0) - the character is going left.
	uint8_t m_numberOfBulletsReceived;//Counts the number of bullet received.
	//If the number is equal to three, the character is eliminated from the game.
	uint8_t m_shotEnemies;//Counts the enemies that were shot.
	//At every enemy shot, the player receives 100 points.
	Point m_position;//Represents the current position of the character.

public:
	//Default constructor.
	Character() : m_direction{ 0, 0 }, m_numberOfBulletsReceived{ 0 },
		m_shotEnemies{ 0 }, m_position{ 0,0 } {}
	//Constructor.
	Character(const Point& direction, const Point& position)
		: m_direction{ direction }, m_numberOfBulletsReceived{ 0 },
		m_shotEnemies{ 0 }, m_position{ position } {}

	//"MoveCharacter" moves the character in the specified direction.
	void MoveCharacter() {
		m_position += m_direction;
	}
	//"ReceivedBullet" counts the received bullets.
	void ReceivedBullet() {
		m_numberOfBulletsReceived++;
	}
	//"ShotEnemy" counts the enemies that were shot.
	void ShotEnemy() {
		m_shotEnemies++;
	}
	//"SetPosition" sets the position of the character.
	void SetPosition(const Point& position) {
		m_position = position;
	}
	//"GetPosition" returns the current position.
	Point GetPosition() const {
		return m_position;
	}
	//"GetCoordonateX" returns the x coordonate of the current position.
	uint8_t GetCoordonateX() {
		return m_position.GetX();
	}
	//"GetY" returns the y coordonate of the current position.
	uint8_t GetCoordonateY() {
		return m_position.GetY();
	}
};