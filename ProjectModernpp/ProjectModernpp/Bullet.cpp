#include "Bullet.h"

Bullet::Bullet(const Point& position, const Point& direction)
    : m_active(true), m_speed(0.25f), m_direction(direction), m_position(position), m_progress{ 0.0f }, m_interpolationProgress{ 0.0f } {}

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

void Bullet::Move(GameMap& map) 
{
    //if (!IsActive()) return; //if the bullet is not active -> return

    //Point newPos = m_position + m_direction; //calculates the new position based on the current position and the direction of the bullet

    ////if the new position of the bullet is out of the matrix or it is identical with a position of an unbreakable wall or with a breakable wall
    ////the bullet is destroyed
    //if (newPos.GetX() < 0 || newPos.GetX() >= map.GetHeight() ||
    //    newPos.GetY() < 0 || newPos.GetY() >= map.GetWidth()) {
    //    SetActive(false);
    //    return;
    //}

    //CellType newCellType = map.GetMap()[newPos.GetX()][newPos.GetY()];

    //if (newCellType == CellType::BREAKABLE_WALL) {
    //    map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::EMPTY; 
    //    SetActive(false);
    //    return;
    //}

    //if (newCellType == CellType::UNBREAKABLE_WALL) {
    //    SetActive(false);
    //    return;
    //}

    //map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;  
    //m_position = newPos; 

    if (!m_active) return;     //if the bullet is not active -> return

    Point newPos = m_position + m_direction;       //calculates the new position based on the current position and the direction of the bullet

    //if the new position of the bullet is out of the matrix or it is identical with a position of an unbreakable wall or with a breakable wall
    //the bullet is destroyed
    if (newPos.GetX() < 0 || newPos.GetX() >= map.GetHeight() ||
        newPos.GetY() < 0 || newPos.GetY() >= map.GetWidth() ||
        map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::UNBREAKABLE_WALL ||
        map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::BREAKABLE_WALL) {
        m_active = false;
        map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
        return;
    }

    if (map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::ENEMY) {
        m_active = false;
        map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
        return;
    }

    //we delete the previous position
    map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
    //we actualize the new position of the bullet, which becomes the current one
    m_position = newPos;
    //and we declare the bullet on the matrix
    map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::BULLET;

}

bool Bullet::operator==(const Bullet& other) const 
{
    return (m_position == other.m_position && m_active == other.m_active);
}

