#pragma once
#include <vector>
#include <conio.h>
#include "Player.h"
#include "Enemy.h"
#include "Base.h"

class Game {
private:
    GameMap m_map;
    Player m_player;
    std::vector<Enemy> m_enemies;

public:
    Game(uint32_t width, uint32_t height, uint8_t level);

    void InitializeGame();

    // Obține lista de inamici // nefolosita inca
    const std::vector<Enemy>& GetEnemies() const;

    void MovePlayer(const Point& direction);

    void Run();
};
