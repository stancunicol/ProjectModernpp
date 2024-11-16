#pragma once

#include "CellType.h"
#ifndef PLAYER_H
#include "Player.h"
#endif 

#ifndef GAMEMAP_H
#include "GameMap.h"
#endif 

#include <vector>
#include <ctime>
#include <iostream>

class Game {
public:
    GameMap map;
    Player player;

    Game(uint32_t width, uint32_t height, uint16_t level, const std::string& name)
        : map(width, height, level), player(name) {
        map.placePlayer(&player);
    }

    void movePlayer(char direction) {
        switch (direction)
        {
        case 'w':
            player.MoveCharacter({ uint8_t(-1), uint8_t(0) });
            break;
        case 'a':
            player.MoveCharacter({ uint8_t(0), uint8_t(-1) });
            break;
        case 's':
            player.MoveCharacter({ uint8_t(1), uint8_t(0) });
            break;
        case 'd':
            player.MoveCharacter({ uint8_t(0), uint8_t(1) });
            break;
        case 'q':
            std::cout << "Iesire din joc." << '\n';
            exit(0);
            break;
        default:
            std::cout << "Directie invalida." << '\n';
            return;
        }
    }
};