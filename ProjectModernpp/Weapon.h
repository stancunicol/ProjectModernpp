#pragma once

#ifndef BULLET_H
#include "Bullet.h"
#endif 

#include <vector>

#ifndef POINT_H
#include "Point.h"
#endif 

class Weapon {
	std::vector<Bullet> m_bullets;//Containes the list of active/shooted bullets.
public:
	//Shoots and memories the bullet.
	Bullet Shoot(const Point& direction);
};