#pragma once
#include "Point.h"
#include "GameMap.h"

class Bullet {
private:
    bool m_active;                // The bullet is active or not
    Point m_direction;            // The direction of the bullet
    float m_speed;         // The speed of the bullet (cells on second)
    Point m_position;             // The current position of the bullet

    float m_progress;
    float m_interpolationProgress;

public:
    //Constructor
    Bullet(const Point& position = Point(0, 0), const Point& direction = Point(0, 0));

    bool IsActive() const;    //Returns if the bullet is active or not at the current moment

    void SetActive(bool active);     //Sets the bullet active or it desactivates it

    Point GetPosition() const;       //Returns the current position of the bullet

    Point GetDirection() const;     //Returns the current direction of the bullet

    void SetPosition(const Point& position);      //Sets the position of the bullet

    //It actualizes the map based on the movements of the bullet and the collisions (if they exists) or if the bullet exists the matrix
    void Move(GameMap& map);

    bool operator==(const Bullet& other) const;

    void SetSpeed(float speed);
};
