#pragma once
#include <iostream>

class Bullet {
private:
	bool m_active = false;//Specifică dacă glonțul este activat, adică urmează ca personajul să împuște pe cineva.
	float m_speed = 0.25f;//Viteza glonțului.
	std::pair<int,int> m_direction;//first și second reprezintă direcția către care glonțul se va duce.
	//(first, second) pot fi (0,1) - glonțul se va duce în sus, (0,-1) - glonțul se va duce în jos
	//(1,0) - glonțul se va duce în dreapta, (-1,0) - glonțul se va duce în stânga.
public:
	//Constructor implicit.
	Bullet() : m_active{ false }, m_direction{ 0, 0 }, m_speed{ 0.25f } {}
	//Constructor.
	Bullet(const bool& active, const float& speed, const std::pair<int,int>& direction)
		: m_active{ active }, m_speed{ speed }, m_direction{ direction } {}
};