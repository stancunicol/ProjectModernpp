#pragma once
#include <iostream>

class Bullet {
private:
	bool m_active = false;//specifică dacă glonțul este activat, adică urmează ca personajul să împuște pe cineva
	float m_speed = 0.25f;//viteza, care este constantă
	std::pair<int,int> m_direction;//x și y reprezintă direcția către care glonțul se va duce
	//(x,y) pot fi (0,1) - glonțul se va duce în sus, (0,-1) - glonțul se va duce în jos
	//(1,0) - glonțul se va duce în dreapta, (-1,0) - glonțul se va duce în stânga
public:
	Bullet(const bool& active, const std::pair<int,int>& direction, const int& y)//constructor
		: m_active{ active }, m_direction{ direction } {}
};