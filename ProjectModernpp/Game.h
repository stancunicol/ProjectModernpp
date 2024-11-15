#pragma once

class Game {
public:
    GameMap map;
    Player player;

    Game(uint32_t width, uint32_t height, uint16_t level, const std::string& name)
        : map(width, height, level), player(name) {
    }
};