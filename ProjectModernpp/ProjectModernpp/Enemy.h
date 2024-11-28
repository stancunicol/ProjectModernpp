#pragma once
#include <cstdlib>
#include "Character.h"
#include "GameMap.h"
#include "Game.h"
#include <chrono>

class Enemy : virtual public Character {
private:
    GameMap& m_map; //references the game map
    Game& m_game;  //Referenced the game itself
    bool m_active = false;  //If the enemy is active or not, which at first is not
    std::chrono::steady_clock::time_point m_lastShoot;   //containes the time of the last shot bullet
    Bullet m_activeBullet;  //if the bullet is active or not

public:
    Enemy(Game& game, GameMap& map);//"puts" the enemies in the game map

    void PlaceCharacter() override;//function from "Character.h"; places the character at a position

    void MoveRandom();//moves the enemies in random movements

    const Point& GetPosition() const;//return the current position

    void Shoot(const Point& direction);//shoots a bullet

    void SetActive(const bool& active);  //sets an enemy active
};
