#include <iostream>
#include "Point.h"

//Default constructor.
Point::Point()
	: m_x{ 0 }, m_y{ 0 }
{
}

//Copy constructor.
Point::Point(const uint16_t& x, const uint16_t& y)
	: m_x{ x }, m_y{ y }
{
}

//"SetPosition" sets the position.
void Point::SetPosition(const Point& position) {
	m_x = position.m_x;
	m_y = position.m_y;
}

//"GetPosition" gets the current position.
Point Point::GetPosition() {
	return { m_x, m_y };
}

//Copies an object in the current object.
Point Point::operator=(const Point& position) {
	m_x = position.m_x;
	m_y = position.m_y;
	return *this;
}

Point Point::operator+(const Point& position)
{
	m_x = m_x + position.m_x;
	m_y = m_y + position.m_y;
	return *this;
}

Point Point::operator+=(const Point& position) {
	m_x = m_x + position.m_x;
	m_y = m_y + position.m_y;
	return *this;
}

//"GetX" returns the x coordonate.
uint16_t Point::GetX() {
	return m_x;
}

//"GetY" returns the y coordonate.
uint16_t Point::GetY() {
	return m_y;
}
