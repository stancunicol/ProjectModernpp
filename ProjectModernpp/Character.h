#pragma once
#include <iostream>

class Character {
	std::pair<int,int> m_direction;//coordonatele reprezintă direcția de deplasare a unuia dintre personaje
	//(x,y) pot fi (0,1) - personajul se va duce în sus, (0,-1) - personajul se va duce în jos
	//(1,0) - personajul se va duce în dreapta, (-1,0) - personajul se va duce în stânga
	int m_numberOfBulletsReceived;//numără câte glonțuri a primit personajul. Dacă numărul
	//ajunge să fie egal cu 3, personajul este eliminat din joc.
	int m_shotEnemies;//numără câți inamici a împușcat jucătorul. La fiecare inamic
	//împușcat, jucătorul primește 100 de puncte.
	std::pair<int, int> m_startPosition;//reprezintă poziția de start
	//a personajului
public:
	//constructor implicit
	Character() : m_direction{ 0, 0 }, m_numberOfBulletsReceived{ 0 },
		m_shotEnemies{ 0 }, m_startPosition{ 0,0 } {}
	Character(const std::pair<int,int>& direction, const std::pair<int,int>& startPosition)
		: m_direction{ direction }, m_numberOfBulletsReceived{ 0 },
		m_shotEnemies{ 0 }, m_startPosition{ startPosition } {}
};