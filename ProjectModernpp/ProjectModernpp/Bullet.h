#pragma once
#include "Point.h"
#include "GameMap.h"

class Bullet {
private:
    bool m_active;                // The bullet is active or not
    Point m_direction;            // The direction of the bullet
    float m_speed;                // The speed of the bullet (cells on second)
    Point m_position;             // The current position of the bullet

public:

    //Constructor
    Bullet(bool active = false, float speed = 0.25f, const Point& direction = Point(0, 0), const Point& position = Point(0, 0))
        : m_active(active), m_speed(speed), m_direction(direction), m_position(position) {}

    bool IsActive() const { return m_active; }              //Returns if the bullet is active or not at the current moment
    void SetActive(bool active) { m_active = active; }      //Sets the bullet active or it desactivates it

    Point GetPosition() const { return m_position; }        //Returns the current position of the bullet
    Point GetDirection() const { return m_direction; }      //Returns the current direction of the bullet
    void SetPosition(const Point& position) { m_position = position; }        //Sets the position of the bullet

    //It actualizes the map based on the movements of the bullet and the collisions (if they exists) or if the bullet exists the matrix
    void Move(GameMap& map) {
        if (!m_active) return;     //if the bullet is not active -> return

        Point newPos = m_position + m_direction;       //calculates the new position based on the current position and the direction of the bullet

        //if the new position of the bullet is out of the matrix or it is identical with a position of an unbreakable wall or with a breakable wall
        //the bullet is destroyed
        if (newPos.GetX() < 0 || newPos.GetX() >= map.GetHeight() ||
            newPos.GetY() < 0 || newPos.GetY() >= map.GetWidth() ||
            map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::UNBREAKABLE_WALL ||
            map.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::BREAKABLE_WALL) {
            m_active = false;
            return;
        }

        //we delete the previous position
        map.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;
        //we actualize the new position of the bullet, which becomes the current one
        m_position = newPos;
        //and we declare the bullet on the matrix
        map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::BULLET;
    }

};
