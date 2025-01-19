export module Bomb;

import Point;

export
class Bomb 
{
private:
    Point m_position;

    bool m_active;

public:
    Bomb(const Point& position);

    bool IsActive() const;

    const Point& GetPosition() const;
};
