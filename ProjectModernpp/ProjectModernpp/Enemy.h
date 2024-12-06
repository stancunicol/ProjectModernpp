#pragma once
#include <cstdlib>
#include "Character.h"
#include "GameMap.h"
#include "Game.h"

class Enemy : virtual public Character {
private:
    GameMap& m_map; //references the game map
    Game& m_game;  //Referenced the game itself
    bool m_active = false;  //If the enemy is active or not, which at first is not
    Bullet m_activeBullet;  //if the bullet is active or not

    Point m_shootDirection = Point(0, 0);

public:
    Enemy(Game& game, GameMap& map);//"puts" the enemies in the game map

    void PlaceCharacter() override;//function from "Character.h"; places the character at a position

    void MoveRandom();//moves the enemies in random movements


    void Shoot(const Point& direction); //shoots a bullet

    void SetActive(const bool& active);  

    const Point& GetPosition() const;//return the current position
    const Point& GetShootDirection() const;
};
