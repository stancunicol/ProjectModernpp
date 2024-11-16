#include <iostream>
#include "Game.h"
#include "Enemy.h"
#include <chrono>
#include <thread>

int main() {
    
    int level;
    std::cout << "Aleveti un level (1 = easy, 2 = medium, 3 = hard): ";
    std::cin >> level;
    Game game(15, 15, level, "John");
    game.map.AddEnemy(new Enemy(Point(3, 3)));
    game.map.AddEnemy(new Enemy(Point(7, 4)));
    game.map.AddEnemy(new Enemy(Point(5, 9)));
    auto lastMoveTime = std::chrono::steady_clock::now();
    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - lastMoveTime;
        if (elapsedTime.count() >= 0.5) {
            game.map.MoveEnemies();
            lastMoveTime = currentTime;
        }
        game.map.display();
        std::cout << "Introduceti o comanda (w/a/s/d pentru miscare, q pentru a iesi): ";
        char command;
        std::cin >> command;
        if (command == 'q')
            break;
        game.movePlayer(command);
    }
    
    return 0;
}
