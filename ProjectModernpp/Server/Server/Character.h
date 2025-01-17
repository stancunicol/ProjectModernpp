#pragma once
import Point;
#include <cstdint>


class Character {
protected:
    Point m_position;
    Point m_direction;
    uint8_t m_lives;
public:
    Character(const Point& direction = Point(0, 0), const Point& position = Point(0, 0)); //constructor

    virtual void MoveCharacter(const Point& direction); //moves the character in the mentioned direction

    Point GetPosition() const;
    Point GetDirection() const;

    void SetPosition(const Point& point);
    void SetDirection(const Point& point);

    uint8_t GetLives() const;
    void SetLives(uint8_t lives);

    void DecreaseLives(uint8_t amount);

    bool IsDead() const;
};
