import Point;

#include <utility> 

Point::Point(int8_t x, int8_t y)
	: m_x{ x }, m_y{ y }, m_data{ nullptr } {}


Point::Point(const Point& other)
{
	if (&other != nullptr) { // Verificare pentru siguranță
		m_x = other.m_x;
		m_y = other.m_y;
		m_data = new int8_t[2]{ other.m_x, other.m_y };
	}
	else {
		m_x = 0;
		m_y = 0;
		m_data = nullptr; // Sau poți inițializa cu un vector implicit
	}
}


Point::Point(Point&& other) noexcept
	: m_x(other.m_x), m_y(other.m_y), m_data(other.m_data) {
	other.m_data = nullptr;
}

Point::~Point()
{
	delete m_data;
}

void Point::SetPosition(const Point& position)
{
	m_x = position.m_x;
	m_y = position.m_y;
}

Point Point::GetPosition() const
{
	return { m_x, m_y };
}

Point& Point::operator=(const Point& position)
{
	if (this == &position) {
		return *this;
	}

	delete m_data;

	m_x = position.m_x;
	m_y = position.m_y;

	if (position.m_data) {
		m_data = new int8_t(*position.m_data);
	}
	else {
		m_data = nullptr;
	}
	return *this;
}

Point Point::operator+(const Point& position) const
{
	return Point(m_x + position.m_x, m_y + position.m_y);
}

Point Point::operator-(const Point& position) const
{
	return Point(m_x - position.m_x, m_y - position.m_y);
}

Point Point::operator+=(const Point& position)
{
	m_x = m_x + position.m_x;
	m_y = m_y + position.m_y;
	return *this;
}

bool Point::operator==(const Point& position) const {
	return (m_x == position.m_x && m_y == position.m_y);
}

bool Point::operator!=(const Point& other) const
{
	return !(*this == other);
}

int8_t Point::GetX() const
{
	return m_x;
}

int8_t Point::GetY() const
{
	return m_y;
}
