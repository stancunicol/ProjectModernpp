#pragma once
#include "Character.h"
#include "GameMap.h"

class Player : virtual public Character {
private:
    std::string m_name;//the name of the player, that the player will write in the log in window
    GameMap& m_grid;//the map of the ga,e
    uint8_t m_points;//the number of points of the player
    uint8_t m_score;//the number of score points

public:
    Player(const std::string& name, GameMap& grid);//constructor

    void PlaceCharacter() override;//from "Character"; places the charater

    void MoveCharacter(const Point& direction) override;//from "Character"; places the character
};
