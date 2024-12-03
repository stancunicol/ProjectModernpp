#pragma once
#include <cstdint>

class Point {
	size_t m_x, m_y;
public:
	//Default constructor.
	Point() = default;

	//Copy constructor.
	Point(const size_t& x, const size_t& y);

	//"SetPosition" sets the position.
	void SetPosition(const Point& position);

	//"GetPosition" gets the current position.
	Point GetPosition() const;

	//Copies an object in the current object.
	Point operator=(const Point& position);

	Point operator+(const Point& position) const {
		return Point(m_x + position.m_x, m_y + position.m_y);
	}

	Point operator-(const Point& position) const {
		return Point(m_x - position.m_x, m_y - position.m_y);
	}

	Point operator+=(const Point& position);

	bool operator==(const Point& position) const;

	//"GetX" returns the x coordonate.
	size_t GetX() const;

	//"GetY" returns the y coordonate.
	size_t GetY() const;
};