#pragma once
#include <iostream>

class Point {
	uint16_t m_x, m_y;
public:
	//Default constructor.
	Point();

	//Copy constructor.
	Point(const uint16_t& x, const uint16_t& y);

	//"SetPosition" sets the position.
	void SetPosition(const Point& position);

	//"GetPosition" gets the current position.
	Point GetPosition();

	//Copies an object in the current object.
	Point operator=(const Point& position);

	Point operator+(const Point& position);

	Point operator+=(const Point& position);

	//"GetX" returns the x coordonate.
	uint16_t GetX();

	//"GetY" returns the y coordonate.
	uint16_t GetY();
};