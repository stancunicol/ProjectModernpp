#pragma once
#include "Character.h"
#include "Point.h"
#include "Bullet.h"
#include "GameMap.h"

class Enemy : public Character
{
	bool m_active;
	GameMap gameMap;
public:
	//Constructor.
	Enemy(const Point& position);

	//"Move" determina miscarea inamicului.
	void RandomMovement();

	//The enemy shoots a bullet.
	Bullet Shoot();

	//The enemy is shot.
	void IsShot();

	//The enemy is active.
	void IsActive();
};

