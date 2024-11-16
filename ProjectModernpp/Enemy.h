#pragma once

#include "Point.h"
#include "Character.h"
#include "Bullet.h"
#include "GameMap.h"
#include "Player.h"
#include "CellType.h"
#include <ctime>
#include "Weapon.h"

class Enemy : public Character {
	bool m_active;
	GameMap m_gameMap;
public:
	//Constructor.
	Enemy();
	Enemy(const Point& position);
	Enemy(const Point& position, GameMap gameMap);

	//"Move" determina miscarea inamicului.
	void RandomMovement();

	//The enemy shoots a bullet.
	Bullet Shoot();

	//The enemy is shot.
	void IsShot();

	//The enemy is active.
	void IsActive();

	//Returns the current position.
	Point GetPosition() const;
};

