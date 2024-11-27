#pragma once
#include <cstdlib>
#include "Character.h"
#include "GameMap.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Game.h"
#include <chrono>

class Enemy : virtual public Character {
private:
    GameMap& m_map; //references the game map
    Weapon m_weapon; //the enemy's weapon to shoot
    Game& m_game;
    bool m_active = false;
    std::chrono::steady_clock::time_point m_lastShoot;
    Bullet m_activeBullet;

public:
    Enemy(Game& game, GameMap& map);//"puts" the enemies in the game map

    void PlaceCharacter() override;//function from "Character.h"; places the character at a position

    void MoveRandom();//moves the enemies in random movements

    const Point& GetPosition() const;//return the current position

    void Shoot(const Point& direction);

    void SetActive(const bool& active);
};
