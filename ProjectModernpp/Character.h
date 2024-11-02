#pragma once
#include <iostream>
#include "Bullet.h"

class Character {
	std::pair<int,int> m_direction;//Coordonatele reprezintă direcția de deplasare a unui personaj.
	//(first, second) pot fi (0,1) - personajul se va duce în sus, (0,-1) - personajul se va duce în jos
	//(1,0) - personajul se va duce în dreapta, (-1,0) - personajul se va duce în stânga.
	int m_numberOfBulletsReceived;//Numără câte glonțuri a primit personajul.
	//Dacă numărul ajunge să fie egal cu 3, personajul este eliminat din joc.
	int m_shotEnemies;//Numără câți inamici a împușcat jucătorul.
	//La fiecare inamic împușcat, jucătorul primește 100 de puncte.
	std::pair<int, int> m_position;//Reprezintă poziția curentă a personajului.
public:
	//Constructor implicit.
	Character() : m_direction{ 0, 0 }, m_numberOfBulletsReceived{ 0 },
		m_shotEnemies{ 0 }, m_position{ 0,0 } {}
	//Constructor.
	Character(const std::pair<int,int>& direction, const std::pair<int,int>& position)
		: m_direction{ direction }, m_numberOfBulletsReceived{ 0 },
		m_shotEnemies{ 0 }, m_position{ position } {}
	//Funcția "MoveCharacter" mută personajul cu o unitate în direcția indicată.
	void MoveCharacter() {
		m_position.first += m_direction.first;
		m_position.second += m_direction.second;
	}
	//Funcția "ReceivedBullet" identifică că personajul a fost împușcat.
	void ReceivedBullet() {
		m_numberOfBulletsReceived++;
	}
	//Funția "ShotEnemy" identifică că personajul a împușcat un inamic.
	void ShotEnemy() {
		m_shotEnemies++;
	}
};