#pragma once
#include <cstdlib>
#include "Character.h"
#include "GameMap.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Game.h"

class __declspec(dllexport) Enemy : virtual public Character {
private:
    GameMap& m_map; //references the game map
    Weapon m_weapon; //the enemy's weapon to shoot
    Game& m_game;

public:
    Enemy(Game& game, GameMap& map);//"puts" the enemies in the game map

    void PlaceCharacter() override;//function from "Character.h"; places the character at a position

    void MoveRandom();//moves the enemies in random movements

    const Point& GetPosition() const;//return the current position

    void Shoot(Game& game);
};
