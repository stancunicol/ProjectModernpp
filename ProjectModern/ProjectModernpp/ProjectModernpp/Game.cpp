#include "Game.h"

Game::Game(uint32_t width, uint32_t height, uint8_t level)
    : m_map(width, height, level), m_player("Player", m_map) {
}

void Game::InitializeGame() {
    m_player.PlaceCharacter();
    for (int i = 0; i < 3; ++i) {
        m_enemies.emplace_back(m_map);
        m_enemies.back().PlaceCharacter();
    }
}

const std::vector<Enemy>& Game::GetEnemies() const {
    return m_enemies;
}

void Game::MovePlayer(const Point& direction) {
    m_player.MoveCharacter(direction);
}

void Game::Run() {
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
