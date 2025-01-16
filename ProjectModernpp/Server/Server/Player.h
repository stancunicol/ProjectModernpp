#pragma once
#include "Character.h"
#include "GameMap.h"
#include <array>
#include <memory>

class  Player : virtual public Character {
private:
    std::string m_name;

    uint16_t m_points;

    uint16_t m_score;

    static std::shared_ptr<std::array<std::pair<Point, bool>, 4>> m_positions;

    Point m_shootDirection = Point(0, 0);

    Point m_moveDirection = Point(0, 0);

    bool m_isActive = true;

public:
    Player();

    Player(const std::string& name, const GameMap& grid);

    void PlaceCharacter();

    void MoveCharacter(const Point& direction, GameMap& m_grid);

    const Point& GetShootDirection() const;

    void ResetPositions(const GameMap& grid);

    std::string GetName() const;
    uint16_t GetPoints() const;
    uint16_t GetScore() const;


    void SetShootDirection(const Point& direction);
    void SetPoints(uint16_t points);
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
