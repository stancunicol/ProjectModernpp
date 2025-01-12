#pragma once
#include "Character.h"
#include "GameMap.h"
#include <array>
#include <memory>

class  Player : virtual public Character {
private:
    std::string m_name;//the name of the player, that the player will write in the log in window

    uint32_t m_points;//the number of points of the player; score/ per game

    uint32_t m_score;//the number of score points

    static std::shared_ptr<std::array<std::pair<Point, bool>, 4>> m_positions;

    Point m_shootDirection = Point(0, 0);

    Point m_moveDirection = Point(0, 0);
    bool m_isActive = true;

public:
    Player();

    Player(const std::string& name, const GameMap& grid);//constructor

    void PlaceCharacter();//from "Character"; places the charater

    void MoveCharacter(const Point& direction, GameMap& m_grid);//from "Character"; places the character


    const Point& GetShootDirection() const;

    void ResetPositions();

    std::string GetName() const;
    uint32_t GetPoints() const;
    uint32_t GetScore() const;


    void SetShootDirection(const Point& direction);
    void SetPoints(uint32_t points);
    void SetScore();

    void SetMoveDirection(const Point& direction) {
        m_moveDirection = direction;
    }

    bool HasMovementCommand() const {
        return m_moveDirection != Point(0, 0);
    }

    const Point& GetMoveDirection() const {
        return m_moveDirection;
    }
    void Deactivate() {
        m_isActive = false;
    }

    void Activate() {
        m_isActive = true;
    }

    bool IsActive() const {
        return m_isActive;
    }

};
