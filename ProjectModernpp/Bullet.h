#pragma once
#include <iostream>
#include "Point.h"

class Bullet {
private:
	bool m_active = false;//Specifies if the bullet is activated (if the character is going to shoot).
	float m_speed = 0.25f;//The bullet's speed.
	Point m_direction;//first și second represent the direction the bullet is going.
	//(first, second) can be (0,1) - the bullet is going up, (0,-1) - the bullet is going down
	//(1,0) - the bullet is going right, (-1,0) - the bullet is going left.
public:
	//Default constructor.
	Bullet();
	//Constructor.
	Bullet(const bool& active, const float& speed, const Point& direction);
	//"Shoot" creates a bullet in the direction the character shoots.
	Bullet Shoot(const Point& direction);
};