#include "Point.h"

Point::Point(const size_t& x, const size_t& y)
	: m_x{ x }, m_y{ y } {
}

void Point::SetPosition(const Point& position) {
	m_x = position.m_x;
	m_y = position.m_y;
}

Point Point::GetPosition() const {
	return { m_x, m_y };
}

Point Point::operator=(const Point& position) {
	m_x = position.m_x;
	m_y = position.m_y;
	return *this;
}

Point Point::operator+=(const Point& position) {
	m_x = m_x + position.m_x;
	m_y = m_y + position.m_y;
	return *this;
}

bool Point::operator==(const Point& position) const {
	if (m_x == position.m_x && m_y == position.m_y)
		return true;
	return false;
}

size_t Point::GetX() const {
	return m_x;
}

size_t Point::GetY() const {
	return m_y;
}
