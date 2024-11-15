#pragma once
#include "Character.h"

class Player : public Character {
	int m_points;//Numără punctele câștigate în urma meciurilor jucate.
	int m_score;//Numără punctele de score în urma meciurilor câștigate.
	int m_waitingTime;//Reține timpul minim de așteptare între două gloanțe.
	std::string m_name;//Numele jucătorului.
public:
	//Constructor.
	Player(const std::string& name) 
		: Character({ 0, 1 }, { 0, 0 }), m_points{ 0 }, m_score{ 0 }, m_waitingTime{ 4 }, m_name{ name } {}

	Player(const std::string& name, uint32_t startX, uint32_t startY)
		: Character({ 0, 1 }, { startX, startY }), m_points{ 0 }, m_score{ 0 }, m_waitingTime{ 4 }, m_name{ name } {}

};