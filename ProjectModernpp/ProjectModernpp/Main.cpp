#include "Game.h"
#include <iostream>
#include <crow.h>

void StartServer(Game& game) {
    crow::SimpleApp serverApp;

    CROW_ROUTE(serverApp, "/game")([&game]() {
            crow::json::wvalue gameState = game.GetGameStateAsJson();
            std::cout << "Game state: " << gameState.dump(4) << '\n';
            return gameState;
        });
    serverApp.port(8080).multithreaded().run();
}

int main() {

    int level;
    std::cout << "Alegeti un nivel (1 = usor, 2 = mediu, 3 = greu): ";
    std::cin >> level;

    Game game(15, 15, level);
    game.InitializeGame();

    std::thread serverThread(StartServer, std::ref(game));

    std::thread gameThread(&Game::Run, &game);

    serverThread.join();

    gameThread.join();

    return 0;
}

//http://localhost:8080/game