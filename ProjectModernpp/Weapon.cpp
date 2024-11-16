#include <iostream>
#include "Weapon.h"

//Shoots and memories the bullet.
Bullet Weapon::Shoot(const Point& direction) {
	Bullet bullet(true, 0.25f, direction);
	m_bullets.emplace_back(bullet);
	return bullet;
}
