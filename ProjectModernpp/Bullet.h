#pragma once

class Bullet {
private:
	bool active = false;
	static const float speed;
	int x, y;
public:
	Bullet(const bool& active, const int& x, const int& y)
		: active{ active }, x{ x }, y{ y } {}
};