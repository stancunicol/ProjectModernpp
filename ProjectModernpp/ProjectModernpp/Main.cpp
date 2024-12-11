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

    CROW_ROUTE(serverApp, "/submitLevel").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        try {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("level")) {
                return crow::response(400, "Invalid JSON payload. Expected a 'level' field.");
            }

            int level = jsonBody["level"].i();
            if (level < 1 || level > 3) {
                return crow::response(400, "Invalid level. Must be 1, 2, or 3.");
            }

            game.SetLevel(level);

            return crow::response(200, "Level updated successfully.");
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error processing request: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/checkUser").methods(crow::HTTPMethod::GET)([&game](const crow::request& req) {
        try {

            auto params = req.url_params;
            if (!params.get("username")) {
                return crow::response(400, "Missing 'username' parameter.");
            }

            std::string username = params.get("username");
            std::cout << username << '\n';

            auto& db = game.GetDatabase();
            if (db.UserExists(username)) {
                return crow::response(200, "success");
            }
            else {
                return crow::response(404, "User not found.");
            }
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error processing request: ") + e.what());
        }
        });

    serverApp.port(8080).multithreaded().run();
}

int main() {
    Game game;

    std::thread serverThread(StartServer, std::ref(game));
    std::thread gameThread(&Game::Run, &game);

    serverThread.join();
    gameThread.join();

    return 0;
}


//http://localhost:8080/game