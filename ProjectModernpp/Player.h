﻿#pragma once
#include "Bullet.h"
#include "Character.h"
#include "Weapon.h"

class Player : public Character {
	int m_points;//Counts the points received for the winned games.
	int m_score;//Counts the score for the winned games.
	int m_waitingTime;//It containes the waiting time between shooting bullets.
	std::string m_name;//The name of the player.
	Weapon m_weapon;
public:
	//Constructor.
	Player(const std::string& name);

	//Shoots the bullet.
	void ShootBullet();
};