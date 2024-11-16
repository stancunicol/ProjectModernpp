#pragma once
#include <iostream>

class Point {
	uint8_t m_x, m_y;
public:
	//Default constructor.
	Point() : m_x{ 0 }, m_y{0} {}
	//Copy constructor.
	Point(const uint8_t& x, const uint8_t& y): m_x{x}, m_y{y} {}
	//"SetPosition" sets the position.
	void SetPosition(const Point& position) {
		m_x = position.m_x;
		m_y = position.m_y;
	}
	//"GetPosition" gets the current position.
	Point GetPosition() {
		return { m_x, m_y };
	}
	//Copies an object in the current object.
	Point operator=(const Point& position) {
		m_x = position.m_x;
		m_y = position.m_y;
		return *this;
	}
	Point operator+=(const Point& position) {
		m_x = m_x + position.m_x;
		m_y = m_y + position.m_y;
		return *this;
	}
	//"GetX" returns the x coordonate.
	uint8_t GetX() {
		return m_x;
	}
	//"GetY" returns the y coordonate.
	uint8_t GetY() {
		return m_y;
	}
};