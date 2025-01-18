#include "Bullet.h"

Bullet::Bullet(const Point& position, const Point& direction)
    : m_active{ true }, m_speed{ 0.25f }, m_direction{ direction }, m_position{ position }, m_progress{ 0.0f }, m_interpolationProgress{ 0.0f } {
}

bool Bullet::IsActive() const
{
    return m_active;
}

void Bullet::SetActive(bool active)
{
    m_active = active;
}

Point Bullet::GetPosition() const
{
    return m_position;
}

Point Bullet::GetDirection() const
{
    return m_direction;
}

void Bullet::SetPosition(const Point& position)
{
    m_position = position;
}

void Bullet::Move(GameMap& map) {
    if (!IsActive()) return;

    Point newPos = m_position + m_direction;

    if (newPos.GetX() < 0 || newPos.GetX() >= map.GetHeight() ||
        newPos.GetY() < 0 || newPos.GetY() >= map.GetWidth()) {
        SetActive(false);
        return;
    }

    CellType newCellType = map.GetMap()[newPos.GetX()][newPos.GetY()];

    if (newCellType == CellType::BREAKABLE_WALL) {
        map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::EMPTY;
        SetActive(false);
        return;
    }

    if (newCellType == CellType::UNBREAKABLE_WALL) {
        SetActive(false);
        return;
    }

    map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
    m_position = std::move(newPos);
}

bool Bullet::operator==(const Bullet& other) const
{
    return (m_position == other.m_position && m_active == other.m_active);
}

void Bullet::SetSpeed(float speed) {
    m_speed = speed;
}

bool Bullet::HasHit() const {
    return m_hit;
}

Point Bullet::GetHitPosition() const {
    return m_hitPosition;
}

int Bullet::GetOwnerId() const {
    return m_ownerId;
}