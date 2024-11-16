#pragma once
#include "Bullet.h"
#include <vector>
#include "Point.h"

class Weapon {
	std::vector<Bullet> m_bullets;//Containes the list of active/shooted bullets.
public:
	//Shoots and memories the bullet.
	Bullet Shoot(const Point& direction);
};