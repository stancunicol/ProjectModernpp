#pragma once
import Point;
#include <cstdint>


class Character 
{
protected:
    Point m_position;

    Point m_direction;

    uint8_t m_lives;

    Point m_initialPosition;
public:
    Character(const Point& direction = Point(0, 0), const Point& position = Point(0, 0)); 

    virtual void MoveCharacter(const Point& direction);

    uint8_t GetLives() const;
    
    Point GetPosition() const;
    
    Point GetDirection() const;

    void SetLives(uint8_t lives);
    
    void SetPosition(const Point& point);
    
    void SetDirection(const Point& point);

    void DecreaseLives(uint8_t amount);

    void ResetPosition();
};
