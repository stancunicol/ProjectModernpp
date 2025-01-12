#include "..\RandomMove\RandomMove.h"
#include "Game.h"
#include <iostream>
#include <crow.h>
#include <regex>
#include <iostream>
#include <crow/json.h>

std::ostream& operator<<(std::ostream& os, const crow::json::wvalue& value) {
    os << value.dump();
    return os;
}

template <typename Func>
crow::response handleRequest(Func&& func) {
    try {
        return std::forward<Func>(func)();
    }
    catch (const std::exception& e) {
        return crow::response(500, std::string("Error processing request: ") + e.what());
    }
}

void StartServer(Game& game) {
    crow::SimpleApp serverApp;

    // GET game state
    CROW_ROUTE(serverApp, "/game")([&game]() {
        return game.GetGameStateAsJson();
        });

    // POST submitLevel
    CROW_ROUTE(serverApp, "/submitLevel").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        return handleRequest([&game, &req]() {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("level"))
                return crow::response(400, "Invalid JSON payload. Expected a 'level' field.");

            int level = jsonBody["level"].i();
            if (level < 1 || level > 3)
                return crow::response(400, "Invalid level. Must be 1, 2, or 3.");

            std::lock_guard<std::mutex> lock(game.GetGameMutex());
            game.SetLevel(level);

            auto& db = game.GetDatabase();
            auto recentPlayer = db.GetRecentPlayer();
            if (recentPlayer) {
                db.UpdateLevel(*recentPlayer, level);
            }

            return crow::response(200, "Level updated successfully.");
            });
        });

    // POST user
    CROW_ROUTE(serverApp, "/user").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        return handleRequest([&game, &req]() {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("username"))
                return crow::response(400, "Invalid JSON payload. Expected a 'username' field.");

            std::string username = jsonBody["username"].s();
            std::regex usernameRegex("^[a-zA-Z]([a-zA-Z]*[0-9]*)*$");
            if (!std::regex_match(username, usernameRegex))
                return crow::response(400, "Invalid 'username' format.");

            auto& db = game.GetDatabase();
            int userId;
            if (db.UserExists(username)) {
                userId = db.GetUserId(username);
                crow::json::wvalue jsonResponse;
                jsonResponse["status"] = "login";
                jsonResponse["userId"] = userId;
                return crow::response(200, jsonResponse);
            }
            else {
                db.AddUser(username);
                userId = db.GetUserId(username);
                crow::json::wvalue jsonResponse;
                jsonResponse["status"] = "register";
                jsonResponse["userId"] = userId;
                return crow::response(200, jsonResponse);
            }
            });
        });

    // POST registerRoom
    CROW_ROUTE(serverApp, "/registerRoom").methods(crow::HTTPMethod::POST)([&game]() {
        return handleRequest([&game]() {
            std::string roomCode = game.CreateRoom();
            crow::json::wvalue response;
            response["code"] = roomCode;
            response["message"] = "Room registered successfully.";
            return crow::response(200, response);
            });
        });

    // POST joinRoom
    CROW_ROUTE(serverApp, "/joinRoom").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        return handleRequest([&game, &req]() {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("code") || !jsonBody.has("playerName"))
                return crow::response(400, "Invalid JSON payload. Expected 'code' and 'playerName' fields.");

            std::string code = jsonBody["code"].s();
            std::string playerName = jsonBody["playerName"].s();
            auto result = game.JoinRoom(code, playerName);

            if (result.has_value()) {
                crow::json::wvalue response;
                response["message"] = "Player joined room successfully.";
                response["roomCode"] = result.value();
                return crow::response(200, response);
            }
            else {
                return crow::response(400, "Unable to join room. Room may be full or not exist.");
            }
            });
        });

    // POST leaveRoom
    CROW_ROUTE(serverApp, "/leaveRoom").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        return handleRequest([&game, &req]() {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("code") || !jsonBody.has("playerName"))
                return crow::response(400, "Invalid JSON payload. Expected 'code' and 'playerName' fields.");

            std::string code = jsonBody["code"].s();
            std::string playerName = jsonBody["playerName"].s();

            if (game.LeaveRoom(code, playerName))
                return crow::response(200, "Player left room successfully.");
            else
                return crow::response(400, "Unable to leave room. Room may not exist or player is not in the room.");
            });
        });

    // GET roomStatus
    CROW_ROUTE(serverApp, "/roomStatus").methods(crow::HTTPMethod::GET)([&game](const crow::request& req) {
        return handleRequest([&game, &req]() {
            auto params = req.url_params;
            if (!params.get("code"))
                return crow::response(400, "Missing 'code' parameter.");

            std::string roomCode = params.get("code");
            auto room = game.GetRoom(roomCode);
            if (room) {
                crow::json::wvalue response;
                response["roomCode"] = room->GetCode();
                response["capacity"] = room->GetCapacity();
                response["players"] = crow::json::wvalue::list(room->GetPlayers().begin(), room->GetPlayers().end());
                response["slotsRemaining"] = 4 - room->GetPlayers().size();
                return crow::response(200, response);
            }
            return crow::response(404, "Room not found.");
            });
        });

    // POST move
    CROW_ROUTE(serverApp, "/move").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        return handleRequest([&game, &req]() {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("playerId") || !jsonBody.has("direction"))
                return crow::response(400, "Invalid JSON payload. Expected 'playerId' and 'direction' fields.");

            int playerId = jsonBody["playerId"].i();
            std::string direction = jsonBody["direction"].s();

            // Logica de mișcare pentru jucător
            Point moveDirection;
            if (direction == "up") {
                moveDirection = Point(-1, 0);
            }
            else if (direction == "down") {
                moveDirection = Point(1, 0);
            }
            else if (direction == "left") {
                moveDirection = Point(0, -1);
            }
            else if (direction == "right") {
                moveDirection = Point(0, 1);
            }
            else {
                return crow::response(400, "Invalid movement or player.");
            }
            });
        });

    CROW_ROUTE(serverApp, "/getPlayerScore").methods(crow::HTTPMethod::GET)([&game](const crow::request& req) {
        try {
            auto params = req.url_params;
            if (!params.get("playerId")) {
                return crow::response(400, "Missing 'playerId' parameter.");
            }

            int playerId = std::stoi(params.get("playerId"));

            auto& db = game.GetDatabase();
            auto playerData = db.GetPlayerDataById(playerId);

            if (!playerData) {
                return crow::response(404, "Player not found.");
            }

            crow::json::wvalue response;
            response["name"] = std::get<0>(*playerData);
            response["score"] = std::get<1>(*playerData);

            return crow::response{ response };
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error processing request: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/getEnemies").methods(crow::HTTPMethod::GET)([&game]() {
        try {
            auto& enemies = game.GetEntityManager().GetEnemies();

            crow::json::wvalue response;
            response["enemyCount"] = enemies.size();

            crow::json::wvalue::list enemyList;
            for (size_t i = 0; i < enemies.size(); ++i) {
                const auto& enemy = enemies[i];

                crow::json::wvalue enemyData;
                enemyData["id"] = static_cast<int>(i);
                enemyData["x"] = enemy.GetPosition().GetX();
                enemyData["y"] = enemy.GetPosition().GetY();

                enemyList.push_back(enemyData);
            }

            response["enemies"] = std::move(enemyList);

            return crow::response(200, response);
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error retrieving enemies: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/getBase").methods(crow::HTTPMethod::GET)([&game]() {
        try {
            auto& base = game.GetEntityManager().GetBase();

            crow::json::wvalue response;

            response["position"]["x"] = base.GetPosition().GetX();
            response["position"]["y"] = base.GetPosition().GetY();

            return crow::response(200, response);
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error retrieving base: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/getBombs").methods(crow::HTTPMethod::GET)([&game]() {
        try {
            auto& bombs = game.GetEntityManager().GetBombs();

            crow::json::wvalue response;
            response["bombCount"] = bombs.size();

            crow::json::wvalue::list bombList;
            for (size_t i = 0; i < bombs.size(); ++i) {
                const auto& bomb = bombs[i];

                crow::json::wvalue bombData;
                bombData["id"] = static_cast<int>(i);
                bombData["x"] = bomb.GetPosition().GetX();
                bombData["y"] = bomb.GetPosition().GetY();

                bombList.push_back(bombData);
            }

            response["bombs"] = std::move(bombList);

            return crow::response(200, response);
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error retrieving bombs: ") + e.what());
        }
        });

    serverApp.port(8080).multithreaded().run();
}

int main() {
    Game game;
    std::thread serverThread(StartServer, std::ref(game));
    serverThread.join();
    return 0;
}
