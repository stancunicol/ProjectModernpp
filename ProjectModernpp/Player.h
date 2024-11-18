#pragma once
#include "Character.h"
#include "GameMap.h"

class Player : virtual public Character {
private:
    std::string m_name;
    GameMap& m_grid;

public:
    Player(const std::string& name, GameMap& grid);

    void PlaceCharacter() override;

    void MoveCharacter(const Point& direction) override;
};
