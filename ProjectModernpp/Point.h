#pragma once
#include <iostream>

class Point {
	uint8_t m_x, m_y;
public:
	//Default constructor.
	Point();

	//Copy constructor.
	Point(const uint8_t& x, const uint8_t& y);

	//"SetPosition" sets the position.
	void SetPosition(const Point& position);

	//"GetPosition" gets the current position.
	Point GetPosition();

	//Copies an object in the current object.
	Point operator=(const Point& position);

	Point operator+=(const Point& position);

	//"GetX" returns the x coordonate.
	uint8_t GetX();

	//"GetY" returns the y coordonate.
	uint8_t GetY();
};