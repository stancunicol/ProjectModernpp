import Point;

#include <utility> 

Point::Point(int x, int y)
	: m_x{ x }, m_y{ y }, m_data{ nullptr } {}


Point::Point(const Point& other)
	: m_x(other.m_x), m_y(other.m_y), m_data(new int[2] {other.m_x, other.m_y}) {}

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
		m_data = new int(*position.m_data);
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

size_t Point::GetX() const
{
	return m_x;
}

size_t Point::GetY() const
{
	return m_y;
}
