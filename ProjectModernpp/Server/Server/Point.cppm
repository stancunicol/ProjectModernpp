export module Point;

import <cstdint>;

export
class Point 
{
private:
    int8_t m_x, m_y;

    int8_t* m_data;

public:
    Point() : m_x(0), m_y(0), m_data(nullptr) {}

    Point(int8_t x, int8_t y);

    Point(const Point& other);

    Point(Point&& other) noexcept;

    ~Point();


    Point& operator=(const Point& position);

    Point operator+(const Point& position) const;

    Point operator-(const Point& position) const;

    Point operator+=(const Point& position);

    bool operator==(const Point& position) const;

    bool operator!=(const Point& other) const;


    void SetPosition(const Point& position);

    Point GetPosition() const;

    int8_t GetX() const;

    int8_t GetY() const;
};
