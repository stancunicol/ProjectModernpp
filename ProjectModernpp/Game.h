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
    Game(uint32_t width, uint32_t height, uint8_t level)
        : m_map(width, height, level), m_player("Player", m_map) {
    }

    void InitializeGame() {
        m_player.PlaceCharacter();
        for (int i = 0; i < 3; ++i) {
            m_enemies.emplace_back(m_map);
            m_enemies.back().PlaceCharacter();
        }
    }

    // Obține lista de inamici
    const std::vector<Enemy>& GetEnemies() const {
        return m_enemies;
    }

    void MovePlayer(const Point& direction) {
        m_player.MoveCharacter(direction);
    }
    // Adaugă un inamic pe hartă
    void AddEnemy(Enemy enemy) {
        m_enemies.push_back(enemy);
        enemy.PlaceCharacter(); // Plasează inamicul pe hartă
        //enemy.MoveRandom();
    }

    void Run() {
        while (true) {
            system("CLS");
            m_map.Display();

            // Citirea comenzii
            std::cout << "Introduceti o comanda (w/a/s/d, q pentru a iesi): ";
            char command;
            std::cin >> command;
            if (command != 'q') {
                switch (command) {
                case 'w': MovePlayer(Point(-1, 0)); break;
                case 'a': MovePlayer(Point(0, -1)); break;
                case 's': MovePlayer(Point(1, 0)); break;
                case 'd': MovePlayer(Point(0, 1)); break;
                }
            }
            else break;
            // Mișcarea inamicilor
            for (auto& enemy : m_enemies) {
                enemy.MoveRandom();
            }

        }
    }
};
