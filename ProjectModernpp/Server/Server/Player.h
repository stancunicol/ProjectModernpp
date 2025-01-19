#pragma once
#include "Character.h"
#include "GameMap.h"
#include <array>
#include <memory>

class  Player : virtual public Character 
{
private:
    std::string m_name;

    uint16_t m_points;

    uint16_t m_score;

    static std::shared_ptr<std::array<std::pair<Point, bool>, 4>> m_positions;

    Point m_moveDirection = Point(0, 0);

    bool m_isActive = true;

public:
    Player();

    Player(const std::string& name, const GameMap& grid);

    void PlaceCharacter();

    void MoveCharacter(const Point& direction, GameMap& m_grid);

    void ResetPositions(const GameMap& grid);

    void Deactivate();

    void Activate();

    void SetPoints(uint16_t points);
    
    void SetScore(int8_t points);

    std::string GetName() const;
    
    uint16_t GetPoints() const;
    
    uint16_t GetScore() const;

};
