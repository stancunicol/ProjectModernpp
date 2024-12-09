#include "Base.h"

Base::Base(const Point& position, uint8_t initialLife)
    : m_destroyed{ false }, m_life{ initialLife }, m_position{ position } {}

void Base::Destroyed()
{
    m_destroyed = true;
}

bool Base::IsDestroyed() const
{
    return m_destroyed;
}

bool Base::TakeHit()
{
    if (m_life > 0) {
        m_life--;
        if (m_life == 0) {
            m_destroyed = true;
            return true; // Jocul s-a încheiat
        }
    }
    return false;
}

int Base::GetLife() const
{
    return m_life;
}

const Point& Base::GetPosition() const
{
    return m_position;
}
