#pragma once
#include "Character.h"

class Player : public Character {
	int m_points;//numără punctele câștigate în urma meciurilor jucate
	int m_score;//numără punctele de score în urma meciurilor câștigate
	int m_waitingTime;//reține timpul minim de așteptare între două gloanțe
public:
	Player() 
		: Character({ 0, 1 }, { 0, 0 }), m_points{0}, m_score{0}, m_waitingTime{4} {}
};