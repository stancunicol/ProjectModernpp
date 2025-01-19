#pragma once
import Point;

#include "GameMap.h"

class Bullet 
{
private:
    bool m_active;               
    Point m_direction;      
    Point m_position;           

    bool m_hit;
    Point m_hitPosition;
    int m_ownerId;

public:
    Bullet(const Point& position = Point(0, 0), const Point& direction = Point(0, 0));

    bool IsActive() const;  

    void SetActive(bool active); 

    Point GetPosition() const; 

    Point GetDirection() const; 

    void SetPosition(const Point& position);

    void Move(GameMap& map);

    bool operator==(const Bullet& other) const;

    bool HasHit() const;

    Point GetHitPosition() const;

    int GetOwnerId() const;
};
