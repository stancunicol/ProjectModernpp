#pragma once
#include "Point.h"

class Bomb {
private:
    Point m_position;
    float m_activationTime;
    bool m_active;

public:
    Bomb(const Point& position, float activationTime);

    void Update(float deltaTime);

    bool IsActive() const;
    const Point& GetPosition() const;
};

