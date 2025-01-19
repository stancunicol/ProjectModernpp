import Bomb;

Bomb::Bomb(const Point& position)
    : m_position{ position }, m_active{ false } {
}

bool Bomb::IsActive() const
{
    return m_active;
}

const Point& Bomb::GetPosition() const
{
    return m_position;
}