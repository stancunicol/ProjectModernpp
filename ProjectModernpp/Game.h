#pragma once
#include <vector>
#include <conio.h>
#include "Player.h"
#include "Enemy.h"
#include "Base.h"

class Game {
private:
    GameMap m_map;//the game map
    Player m_player;//the current player
    std::vector<Enemy> m_enemies;//the enemies

public:
    Game(uint32_t width, uint32_t height, uint8_t level);//constructor

    void InitializeGame();//the game is initialed

    const std::vector<Enemy>& GetEnemies() const;//returns the list of enemies; unused for now

    void MovePlayer(const Point& direction);//moves the player in the mentioned direction

    void Run();//runs the game
};
