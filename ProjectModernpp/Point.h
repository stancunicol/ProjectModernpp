#pragma once
#include <cstdint>

class Point {
	size_t m_x, m_y;
public:
	//default constructor.
	Point() = default;

	//copy constructor
	Point(const size_t& x, const size_t& y)
		: m_x{ x }, m_y{ y }
	{}

	//sets the position.
	void SetPosition(const Point& position) {
		m_x = position.m_x;
		m_y = position.m_y;
	}

	//gets the current position.
	Point GetPosition() const {
		return { m_x, m_y };
	}

	//copies an object in the current object
	Point operator=(const Point& position) {
		m_x = position.m_x;
		m_y = position.m_y;
		return *this;
	}

	Point operator+(const Point& position) {
		m_x = m_x + position.m_x;
		m_y = m_y + position.m_y;
		return *this;
	}

	Point operator+=(const Point& position) {
		m_x = m_x + position.m_x;
		m_y = m_y + position.m_y;
		return *this;
	}

	//returns the x coordonate
	size_t GetX() const {
		return m_x;
	}

	//returns the y coordonate
	size_t GetY() const {
		return m_y;
	}
};