#pragma once

class Bullet {
private:
	bool active = false;//specifică dacă glonțul este activat, adică urmează ca personajul să împuște pe cineva
	static const float speed;//viteza, care este constantă
	int x, y;//x și y reprezintă direcția către care glonțul se va duce
	//(x,y) pot fi (0,1) - glonțul se va duce în sus, (0,-1) - glonțul se va duce în jos
	//(1,0) - glonțul se va duce în dreapta, (-1,0) - glonțul se va duce în stânga
public:
	Bullet(const bool& active, const int& x, const int& y)//constructor
		: active{ active }, x{ x }, y{ y } {}
};