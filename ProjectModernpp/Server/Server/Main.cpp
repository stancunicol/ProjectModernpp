﻿#include "..\RandomMove\RandomMove.h"
#include "Game.h"
#include <iostream>
#include <crow.h>
#include <regex>
#include <crow/json.h>
#include "Player.h"
#include <thread>
#include <chrono>

std::ostream& operator<<(std::ostream& os, const crow::json::wvalue& value) 
{
    os << value.dump();
    return os;
}

template <typename Func>
crow::response handleRequest(Func&& func) 
{
    try {
        return std::forward<Func>(func)();
    }
    catch (const std::exception& e) {
        return crow::response(500, std::string("Error processing request: ") + e.what());
    }
}

void UpdateEnemyPositionsPeriodically(Game& game) 
{
    while (true) 
    {
        game.GetEntityManager().UpdateEnemyPositions(game.GetMap());
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void StartServer(Game& game) 
{
    crow::SimpleApp serverApp;

    CROW_ROUTE(serverApp, "/game")([&game]() {
        return game.GetGameStateAsJson();
        });

    CROW_ROUTE(serverApp, "/submitLevel").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        try {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("level")) {
                std::cerr << "Invalid JSON payload. Expected a 'level' field." << std::endl;
                return crow::response(400, "Invalid JSON payload. Expected a 'level' field.");
            }

            uint8_t level = jsonBody["level"].i();
            if (level < 1 || level > 3) {
                std::cerr << "Invalid level. Level must be between 1 and 3." << std::endl;
                return crow::response(400, "Invalid level. Must be 1, 2, or 3.");
            }

            game.SetLevel(level);

            auto& db = game.GetDatabase();

            auto recentPlayer = db.GetRecentPlayer();

            if (recentPlayer) {
                db.UpdateLevel(*recentPlayer, level);
            }
            return crow::response(200, "Level updated successfully.");
        }
        catch (const std::exception& e) {
            std::cerr << "Error processing request: " << e.what() << std::endl;
            return crow::response(500, std::string("Error processing request: ") + e.what());
        }
        });

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
            uint8_t userId;
            if (db.UserExists(username))
            {
                userId = db.GetUserId(username);

                crow::json::wvalue jsonResponse;
                jsonResponse["status"] = "login";
                jsonResponse["userId"] = userId;
                game.GetEntityManager().AddPlayer(userId, username, game.GetMap());

                return crow::response(200, jsonResponse);
            }
            else {
                db.AddUser(username);
                userId = db.GetUserId(username);
                crow::json::wvalue jsonResponse;
                jsonResponse["status"] = "register";
                jsonResponse["userId"] = userId;
                game.GetEntityManager().AddPlayer(userId, username, game.GetMap());

                return crow::response(200, jsonResponse);
            }
            });
        });

    CROW_ROUTE(serverApp, "/registerRoom").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        return handleRequest([&game, &req]() {
            std::string code = game.GenerateRoomCode();

            auto map = std::make_unique<GameMap>();

            if (!game.GetRoomManager().CreateRoom(code, std::move(map))) 
            {
                return crow::response(500, "Error creating room.");
            }

            game.GetDatabase().AddRoom(code);

            auto& db = game.GetDatabase();
            std::optional<std::string> recentPlayer = db.GetRecentPlayer();
            if (!recentPlayer.has_value()) {
                return crow::response(400, "No recently declared user available.");
            }

            db.InsertRoomCode(recentPlayer.value(), code);

            crow::json::wvalue response;
            response["message"] = "Room created successfully.";
            response["code"] = code;
            return crow::response(200, response);
            });
        });

    CROW_ROUTE(serverApp, "/joinRoom").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        try {
            auto jsonBody = crow::json::load(req.body);

            if (!jsonBody.has("code") || !jsonBody.has("playerId")) 
            {
                std::cerr << "Invalid JSON payload: Missing 'code' or 'playerId'." << std::endl;
                return crow::response(400, "Invalid JSON payload. Expected 'code' and 'playerId'.");
            }

            std::string code = jsonBody["code"].s();
            int playerId = jsonBody["playerId"].i();

            auto result = game.JoinRoom(code, playerId);

            if (!result.has_value()) 
            {
                game.GetDatabase().UpdateDataByRoomCode(playerId, code);

                crow::json::wvalue response;
                response["message"] = "Player joined room successfully.";
                response["roomCode"] = code;
                return crow::response(200, response);
            }
            else {
                return crow::response(400, "Unable to join room. Room may be full or not exist.");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error processing /joinRoom request: " << e.what() << std::endl;
            return crow::response(500, std::string("Error processing request: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/leaveRoom").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        return handleRequest([&game, &req]() {

            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("code") || !jsonBody.has("playerName"))
                return crow::response(400, "Invalid JSON payload. Expected 'code' and 'playerName' fields.");

            std::string code = jsonBody["code"].s();
            int playerId = jsonBody["id"].i();

            game.GetDatabase().RemovePlayerFromRoom(playerId);

            if (game.LeaveRoom(code, playerId))
                return crow::response(200, "Player left room successfully.");
            else
                return crow::response(400, "Unable to leave room. Room may not exist or player is not in the room.");
            });
        });

    CROW_ROUTE(serverApp, "/roomStatus").methods(crow::HTTPMethod::GET)([&game](const crow::request& req) {
        return handleRequest([&game, &req]() {

            auto params = req.url_params;
            if (!params.get("code"))
                return crow::response(400, "Missing 'code' parameter.");

            std::string roomCode = params.get("code");
            auto room = game.GetRoom(roomCode);

            if (room) 
            {
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

    CROW_ROUTE(serverApp, "/move").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        try {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("playerId") || !jsonBody.has("direction")) 
            {
                return crow::response(400, "Invalid JSON payload. Expected 'playerId' and 'direction'.");
            }

            uint8_t playerId = jsonBody["playerId"].i();
            std::string direction = jsonBody["direction"].s();

            Point moveDirection;
            if (direction == "up") moveDirection = Point(-1, 0);
            else if (direction == "down") moveDirection = Point(1, 0);
            else if (direction == "left") moveDirection = Point(0, -1);
            else if (direction == "right") moveDirection = Point(0, 1);
            else return crow::response(400, "Invalid direction.");

            auto& players = game.GetEntityManager().GetPlayersMutable();
            auto it = players.find(playerId);

            if (it == players.end()) 
            {
                return crow::response(404, "Player not found.");
            }

            Point currentPosition = it->second.GetPosition();

            it->second.MoveCharacter(moveDirection, game.GetMap());
            Point newPosition = it->second.GetPosition();
            
            crow::json::wvalue response;
            response["status"] = "success";
            response["currentPosition"] = { {"x", currentPosition.GetX()}, {"y", currentPosition.GetY()} };
            response["newPosition"] = { {"x", newPosition.GetX()}, {"y", newPosition.GetY()} };

            return crow::response(200, response);
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Server error: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/getPlayerScore").methods(crow::HTTPMethod::GET)([&game](const crow::request& req) {
        try {
            auto params = req.url_params;
            if (!params.get("playerId")) 
            {
                return crow::response(400, "Missing 'playerId' parameter.");
            }

            uint8_t playerId = std::stoi(params.get("playerId"));

            auto& db = game.GetDatabase();
            auto playerData = db.GetPlayerDataById(playerId);

            if (!playerData) 
            {
                return crow::response(404, "Player not found.");
            }

            std::vector<uint8_t> playersInRoom = db.GetPlayersFromRoom(playerId);

            playersInRoom.push_back(playerId);

            crow::json::wvalue::list playerScores;
            for (uint8_t id : playersInRoom) {
                auto playerInfo = db.GetPlayerDataById(id);
                if (playerInfo) {
                    playerScores.push_back({
                        {"name", std::get<0>(*playerInfo)},
                        {"score", std::get<1>(*playerInfo)}
                        });
                }
            }

            crow::json::wvalue response;
            response["players"] = std::move(playerScores);

            return crow::response(200, response);
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error processing request: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/getEnemies").methods(crow::HTTPMethod::GET)([&game]() {
        try {
            auto& enemies = game.GetEntityManager().GetEnemies();

            if (enemies.empty()) {
                crow::json::wvalue response;
                response["enemyCount"] = 0;
                response["enemies"] = crow::json::wvalue::list();
                return crow::response(200, response);
            }

            crow::json::wvalue response;
            response["enemyCount"] = enemies.size();

            crow::json::wvalue::list enemyList;
            for (const auto& enemyPair : enemies) {

                crow::json::wvalue enemyData;
                enemyData["id"] = enemyPair.first;
                enemyData["x"] = enemyPair.second.GetPosition().GetX();
                enemyData["y"] = enemyPair.second.GetPosition().GetY();
                
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
            std::vector<Point> updatedBombPositions = game.GetUpdatedBombs();

            crow::json::wvalue response;
            response["bombCount"] = updatedBombPositions.size();

            crow::json::wvalue::list bombList;
            for (size_t i = 0; i < updatedBombPositions.size(); ++i) 
            {
                const auto& bombPosition = updatedBombPositions[i];

                crow::json::wvalue bombData;
                bombData["id"] = static_cast<int>(i);
                bombData["x"] = bombPosition.GetX();
                bombData["y"] = bombPosition.GetY();

                bombList.push_back(bombData);
            }

            response["bombs"] = std::move(bombList);

            return crow::response(200, response);
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error retrieving bombs: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/getPlayerPositions").methods(crow::HTTPMethod::GET)([&game]() {
        try {
            crow::json::wvalue response;

            auto positions = game.GetEntityManager().GetAllPlayerPositions();
            crow::json::wvalue::list positionList;

            for (const auto& [position, name] : positions) 
            {
                uint8_t playerId = game.GetDatabase().GetUserId(name);
                crow::json::wvalue playerJson;
                playerJson["id"] = playerId; 
                playerJson["position"] = { {"x", position.GetX()}, {"y", position.GetY()} };
                positionList.push_back(playerJson);
            }

            response["positions"] = std::move(positionList);
            response["message"] = "Player positions retrieved successfully.";
            return crow::response(200, response);
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error retrieving player positions: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/playerState").methods(crow::HTTPMethod::GET)(
        [&game](const crow::request& req) {
            try {
                auto queryParams = crow::query_string(req.url_params);
                const char* playerIdParam = queryParams.get("playerId");
                if (!playerIdParam) 
                {
                    return crow::response(400, "Missing 'playerId' query parameter.");
                }

                uint8_t playerId = std::stoi(playerIdParam);

                auto playersFromDB = game.GetEntityManager().GetPlayerIdsFromRoom(playerId);

                auto& players = game.GetEntityManager().GetPlayers();
                if (players.find(playerId) == players.end()) {
                    return crow::response(404, "Player not found.");
                }

                crow::json::wvalue response;
                response["status"] = "success";
                crow::json::wvalue::list playerStates;

                for (uint8_t id : playersFromDB) 
                {
                    auto it = players.find(id);
                    if (it != players.end()) 
                    {
                        const Player& player = it->second;
                        playerStates.push_back({
                            {"id", id},
                            {"position", {{"x", player.GetPosition().GetX()}, {"y", player.GetPosition().GetY()}}},
                            {"username", player.GetName()}
                            });
                    }
                }

                response["players"] = std::move(playerStates);
                return crow::response(200, response);
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Server error: ") + e.what());
            }
        });

    CROW_ROUTE(serverApp, "/getEnemiesState").methods(crow::HTTPMethod::GET)(
        [&game](const crow::request& req) {
            try {
                auto& enemies = game.GetEntityManager().GetEnemies();

                crow::json::wvalue response;
                response["status"] = "success";
                crow::json::wvalue::list enemyStates;

                for (const auto& enemyPair : enemies) {
                    enemyStates.push_back({
                        {"id", enemyPair.first},
                        {"position", {{"x", enemyPair.second.GetPosition().GetX()}, {"y", enemyPair.second.GetPosition().GetY()}}}
                        });
                }

                response["enemies"] = std::move(enemyStates);
                return crow::response(200, response);
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Server error: ") + e.what());
            }
        });

    CROW_ROUTE(serverApp, "/fireBullet").methods(crow::HTTPMethod::POST)(
        [&game](const crow::request& req) {
            try {
                // Parse JSON body
                auto jsonBody = crow::json::load(req.body);
                if (!jsonBody || !jsonBody.has("playerId") || !jsonBody.has("position") || !jsonBody.has("direction")) 
                {
                    return crow::response(400, "Invalid JSON payload. Expected 'playerId', 'position', and 'direction'.");
                }

                uint8_t playerId = jsonBody["playerId"].i();
                Point bulletPos(jsonBody["position"]["x"].i(), jsonBody["position"]["y"].i());
                Point bulletDir(jsonBody["direction"]["x"].i(), jsonBody["direction"]["y"].i());

                Bullet bullet(bulletPos, bulletDir);

                Point hitPos;
                bool isPlayerHit = false;
                bool isEnemyHit = false;
                bool collision = game.CheckBulletCollision(bullet, hitPos, isPlayerHit, isEnemyHit);

                game.GetEntityManager().AddPlayerBullet(bullet, playerId);

                game.GetEntityManager().HandleCollisions(game.GetMap());

                crow::json::wvalue response;
                response["status"] = "success";
                response["collision"] = collision;
                response["hitPosition"] = { {"x", hitPos.GetX()}, {"y", hitPos.GetY()} };

                if (collision) 
                {
                    if (isPlayerHit) 
                    {
                        response["hitObject"] = "player";
                    }
                    else if (isEnemyHit) 
                    {
                        response["hitObject"] = "enemy";
                    }
                    else {
                        CellType cellType = game.GetMap().GetMap()[hitPos.GetX()][hitPos.GetY()];
                        if (cellType == CellType::BREAKABLE_WALL || cellType == CellType::UNBREAKABLE_WALL) 
                        {
                            response["hitObject"] = "wall";
                        }
                        else 
                        {
                            response["hitObject"] = "unknown";
                        }
                    }
                }
                else 
                {
                    response["hitObject"] = "none";
                }

                return crow::response(200, response);
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Error processing request: ") + e.what());
            }
        });

    CROW_ROUTE(serverApp, "/getBaseState").methods(crow::HTTPMethod::GET)(
        [&game](const crow::request& req) {
            try {
                const auto& base = game.GetEntityManager().GetBase();

                crow::json::wvalue response;
                response["status"] = "success";
                response["baseState"] = base.GetLife() > 0 ? "intact" : "destroyed";

                return crow::response(200, response);
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Server error: ") + e.what());
            }
        });

    CROW_ROUTE(serverApp, "/getBullets").methods(crow::HTTPMethod::GET)(
        [&game](const crow::request& req) {
            try {
                if (!req.url_params.get("playerId")) 
                {
                    return crow::response(400, "Missing 'playerId' parameter.");
                }

                int excludePlayerId = std::stoi(req.url_params.get("playerId"));

                auto& entityManager = game.GetEntityManager();
                auto& enemyBullets = entityManager.GetBullets();
                auto& playerBulletsMap = entityManager.GetPlayersBullets();

                crow::json::wvalue response;
                response["status"] = "success";
                crow::json::wvalue::list bulletList;

                for (const Bullet& bullet : enemyBullets) {
                    bulletList.push_back({
                        {"position", {{"x", bullet.GetPosition().GetX()}, {"y", bullet.GetPosition().GetY()}}},
                        {"direction", {{"x", bullet.GetDirection().GetX()}, {"y", bullet.GetDirection().GetY()}}}
                        });
                }

                for (const auto& [playerId, bullets] : playerBulletsMap) {
                    if (playerId == excludePlayerId) continue;

                    for (const Bullet& bullet : bullets) {
                        bulletList.push_back({
                            {"position", {{"x", bullet.GetPosition().GetX()}, {"y", bullet.GetPosition().GetY()}}},
                            {"direction", {{"x", bullet.GetDirection().GetX()}, {"y", bullet.GetDirection().GetY()}}}
                            });
                    }
                }

                response["bullets"] = std::move(bulletList);
                return crow::response(200, response);
            }
            catch (const std::exception& e) {
                return crow::response(500, std::string("Server error: ") + e.what());
            }
        });

    serverApp.port(8080).multithreaded().run();
}

int main() {
    Game game;
    std::thread updateThread(UpdateEnemyPositionsPeriodically, std::ref(game));
    std::thread serverThread(StartServer, std::ref(game));
    serverThread.join();
    updateThread.detach();
    return 0;
}