#pragma once
#include "Character.h"
#include "GameMap.h"
#include <vector>

class  Player : virtual public Character {
private:
    std::string m_name;//the name of the player, that the player will write in the log in window

    uint32_t m_points;//the number of points of the player; score/ per game

    uint32_t m_score;//the number of score points

    std::vector<std::pair<Point, bool>> m_positions;

    Point m_shootDirection = Point(0, 0);

public:
    Player(const std::string& name, const GameMap& grid);//constructor

    void PlaceCharacter();//from "Character"; places the charater

    void MoveCharacter(const Point& direction, GameMap& m_grid);//from "Character"; places the character


    const Point& GetShootDirection() const;

    std::string GetName() const;
    uint32_t GetPoints() const;
    uint32_t GetScore() const;


    void SetShootDirection(const Point& direction);
    void SetPoints(uint32_t points);
    void SetScore();

};
