#pragma once
#include "Point.h"
#include "CellType.h"

class  Base {
private:
    bool m_destroyed;   // Checks if the base is destroyed
    uint8_t m_life;
    Point m_position;

public:
    // Constructor
    Base(const Point& position, uint8_t initialLife = 10);

    // Marks the base as destroyed
    void Destroyed();

    // Checks if the base is destroyed
    bool IsDestroyed() const;

    // Scade viața bazei și returnează dacă jocul s-a încheiat
    void TakeHit();

    void SetPosition(const Point& point);

    int GetLife() const;
    const Point& GetPosition() const;
};
