#pragma once
#include <iostream>
#include "Bullet.h"
#include "Point.h"

class Character {
protected:

	Point m_direction;//The coordonates represent the direction of movement of the character.
	uint8_t m_numberOfBulletsReceived;//Counts the number of bullet received.
	//If the number is equal to three, the character is eliminated from the game.
	uint8_t m_shotEnemies;//Counts the enemies that were shot.
	//At every enemy shot, the player receives 100 points.
	Point m_position;//Represents the current position of the character.

public:
	//Default constructor.
	Character();

	//Constructor.
	Character(const Point& direction, const Point& position);

	//"MoveCharacter" moves the character in the specified direction.
	void MoveCharacter(const Point& direction);

	//"ReceivedBullet" counts the received bullets.
	void ReceivedBullet();

	//"ShotEnemy" counts the enemies that were shot.
	void ShotEnemy();

	//"SetPosition" sets the position of the character.
	void SetPosition(const Point& position);

	//"GetPosition" returns the current position.
	Point GetPosition() const;

	//"GetCoordonateX" returns the x coordonate of the current position.
	uint8_t GetCoordonateX();

	//"GetY" returns the y coordonate of the current position.
	uint8_t GetCoordonateY();
};