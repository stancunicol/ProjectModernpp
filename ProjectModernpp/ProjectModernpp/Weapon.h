#pragma once
#include "Bullet.h"
#include <vector>

class Weapon {
private:
    std::vector<Bullet> m_bullets;//lists of bullets

public:
    Bullet Shoot(const Point& direction) {//shoots the bullet in the mentioned direction
        Bullet bullet(true, 0.25f, direction);
        m_bullets.push_back(bullet);
        return bullet;
    }
};
