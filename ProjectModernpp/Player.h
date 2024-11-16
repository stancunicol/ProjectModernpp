#pragma once
#include "Character.h"

class Player : public Character {
	int m_points;//Counts the points received for the winned games.
	int m_score;//Counts the score for the winned games.
	int m_waitingTime;//It containes the waiting time between shooting bullets.
	std::string m_name;//The name of the player.
public:
	//Constructor.
	Player(const std::string& name) 
		: Character({ 0, 1 }, { 0, 0 }), m_points{ 0 }, m_score{ 0 }, m_waitingTime{ 4 }, m_name{ name } {}
	//Copy constructor.
	Player(const std::string& name, uint8_t startX, uint8_t startY)
		: Character({ 0, 1 }, { startX, startY }), m_points{ 0 }, m_score{ 0 }, m_waitingTime{ 4 }, m_name{ name } {}

};