#pragma once
#include <vector>
#include <conio.h>
#include "Player.h"
#include "Base.h"
#include "Bullet.h"

class Enemy;

class __declspec(dllexport) Game {
private:
    GameMap m_map;//the game map
    Player m_player;//the current player
    std::vector<Enemy*> m_enemies;//the enemies
    std::vector<Player*> m_players;//the players
    std::vector<Bullet> m_bullets;//the bullets from the enemies

public:
    Game(uint32_t width, uint32_t height, uint8_t level);//constructor

    ~Game();

    void InitializeGame();//the game is initialed

    const std::vector<Enemy*> GetEnemies() const;//returns the list of enemies; unused for now

    void MovePlayer(const Point& direction);//moves the player in the mentioned direction

    void Run();//runs the game

    void AddBullet(const Bullet& bullet);

    void UpdateBullets(std::vector<Bullet>& bullets, GameMap& map);
};
