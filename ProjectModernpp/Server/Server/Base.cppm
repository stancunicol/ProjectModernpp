export module Base;

import Point;  
import <cstdint>;
#include "CellType.h"

export
class Base {
private:
    bool m_destroyed;
    uint8_t m_life;
    Point m_position;

public:
    Base(const Point& position, uint8_t initialLife = 10);

    void Destroyed();

    bool IsDestroyed() const;

    void TakeHit();

    void SetPosition(const Point& point);

    int GetLife() const;

    const Point& GetPosition() const;
};
