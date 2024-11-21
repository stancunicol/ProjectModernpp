#include "Game.h"
#include <iostream>

int main() {

    int level;
    std::cout << "Alegeti un nivel (1 = usor, 2 = mediu, 3 = greu): ";
    std::cin >> level;

    Game game(21, 21, level);
    game.InitializeGame();
    game.Run();

	return 0;
}