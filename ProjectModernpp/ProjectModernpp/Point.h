#pragma once
#include <cstdint>

class Point {
private:
	int m_x, m_y;
	int* m_data;

public:
	//Default constructor.
	Point() = default;

	//Copy constructor.
	Point(int x, int y);
	Point(const Point& other);

	~Point();

	Point& operator=(const Point& position);

	Point operator+(const Point& position) const;

	Point operator-(const Point& position) const;

	Point operator+=(const Point& position);

	bool operator==(const Point& position) const;

	bool operator!=(const Point& other) const;

	void SetPosition(const Point& position);
	Point GetPosition() const;

	size_t GetX() const;
	size_t GetY() const;
};