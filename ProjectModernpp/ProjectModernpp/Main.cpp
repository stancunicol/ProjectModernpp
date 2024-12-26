#include "Game.h"
#include <iostream>
#include <crow.h>
#include <regex>

void StartServer(Game& game) {
    crow::SimpleApp serverApp;

    /*CROW_ROUTE(serverApp, "/game")([&game]() {
        crow::json::wvalue gameState = game.GetGameStateAsJson();
        std::cout << "Game state: " << gameState.dump(4) << '\n';
        return gameState;
        });*/

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
            auto& db = game.GetDatabase();

            auto recentPlayer = db.GetRecentPlayer();
            if (recentPlayer) {
                db.UpdateLevel(*recentPlayer, level);
            }
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

    CROW_ROUTE(serverApp, "/register").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        try {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("username")) {
                return crow::response(400, "Invalid JSON payload. Expected a 'username' field.");
            }

            std::string username = jsonBody["username"].s();

            bool validUsername = true;
            std::regex usernameRegex("^[a-zA-Z]([a-zA-Z]*[0-9]*)*$");
            if (!std::regex_match(username, usernameRegex)) {
                validUsername = false;
                return crow::response(400, "Invalid 'username' format.");
            }
            if (validUsername) {
                auto& db = game.GetDatabase();
                if (db.UserExists(username)) {
                    return crow::response(409, "User already exists.");
                }
                else {
                    db.AddUser(username);
                    return crow::response(200, "registered");
                }
            }
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error processing request: ") + e.what());
        }
        });

    CROW_ROUTE(serverApp, "/registerRoom").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        try {
            auto jsonBody = crow::json::load(req.body);

            if (!jsonBody || !jsonBody.has("code")) {
                return crow::response(400, "Invalid JSON payload. Expected 'code' field.");
            }

            std::string roomCode = jsonBody["code"].s();
            auto& db = game.GetDatabase();

            std::optional<std::string> recentPlayer = db.GetRecentPlayer();
            if (!recentPlayer.has_value()) {
                return crow::response(400, "No recently declared user available.");
            }

            db.InsertRoomCode(recentPlayer.value(), roomCode);
            return crow::response(200, "Room registered successfully.");
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error processing request: ") + e.what());
        }
        });



    CROW_ROUTE(serverApp, "/checkRoom").methods(crow::HTTPMethod::GET)([&game](const crow::request& req) {
        try {
            auto params = req.url_params;
            if (!params.get("code")) {
                return crow::response(400, "Missing 'code' parameter.");
            }

            std::string code = params.get("code");
            auto& db = game.GetDatabase();
            auto playerName = db.FindRoomByCode(code);

            if (playerName) {
                auto recentPlayer = db.GetRecentPlayer();
                if (recentPlayer) {
                    try {
                        db.InsertRoomCode(*recentPlayer, code);
                    }

                    //TODO: Cand camera este plina tot cod invalid returneaza
                    catch (const std::runtime_error& e) {
                        return crow::response(403, "Room is full. Cannot join.");
                    }
                }

                crow::json::wvalue response;
                response["status"] = "success";
                response["playerName"] = *playerName;
                response["updatedUser"] = recentPlayer ? *recentPlayer : "none";
                return crow::response(200, response);
            }
            else {
                return crow::response(404, "Room code not found.");
            }
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error processing request: ") + e.what());
        }
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

    CROW_ROUTE(serverApp, "/createRoom").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        auto jsonBody = crow::json::load(req.body);
        if (!jsonBody || !jsonBody.has("capacity")) {
            return crow::response(400, "Invalid JSON payload. Expected 'capacity'.");
        }

        std::string roomCode = game.CreateRoom();
        crow::json::wvalue response;
        response["status"] = "success";
        response["roomCode"] = roomCode;
        return crow::response(response);
        });

    CROW_ROUTE(serverApp, "/joinRoom").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        auto jsonBody = crow::json::load(req.body);
        if (!jsonBody || !jsonBody.has("code") || !jsonBody.has("username")) {
            return crow::response(400, "Invalid JSON payload. Expected 'code' and 'username'.");
        }

        std::string roomCode = jsonBody["code"].s();
        std::string username = jsonBody["username"].s();

        auto result = game.JoinRoom(roomCode, username);
        if (result) {
            return crow::response(200, "Joined room successfully.");
        }
        else {
            return crow::response(403, "Room is full or doesn't exist.");
        }
        });

    CROW_ROUTE(serverApp, "/leaveRoom").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        auto jsonBody = crow::json::load(req.body);
        if (!jsonBody || !jsonBody.has("code") || !jsonBody.has("username")) {
            return crow::response(400, "Invalid JSON payload. Expected 'code' and 'username'.");
        }

        std::string roomCode = jsonBody["code"].s();
        std::string username = jsonBody["username"].s();

        if (game.LeaveRoom(roomCode, username)) {
            return crow::response(200, "Left room successfully.");
        }
        else {
            return crow::response(404, "Room or user not found.");
        }
        });

    CROW_ROUTE(serverApp, "/roomStatus").methods(crow::HTTPMethod::GET)([&game](const crow::request& req) {
        auto params = req.url_params;
        if (!params.get("code")) {
            return crow::response(400, "Missing 'code' parameter.");
        }

        std::string roomCode = params.get("code");
        auto room = game.GetRoom(roomCode);
        if (room) {
            crow::json::wvalue response;
            response["roomCode"] = room->GetCode();
            response["capacity"] = room->GetCapacity();
            response["players"] = crow::json::wvalue::list(room->GetPlayers().begin(), room->GetPlayers().end());
            response["slotsRemaining"] = 4 - room->GetPlayers().size();
            return crow::response(response);
        }
        return crow::response(404, "Room not found.");
        });

    CROW_ROUTE(serverApp, "/move").methods(crow::HTTPMethod::POST)([&game](const crow::request& req) {
        try {
            auto jsonBody = crow::json::load(req.body);
            if (!jsonBody || !jsonBody.has("playerId") || !jsonBody.has("direction")) {
                return crow::response(400, "Invalid JSON payload. Expected 'playerId' and 'direction'.");
            }

            int playerId = jsonBody["playerId"].i();
            std::string direction = jsonBody["direction"].s();

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
                return crow::response(400, "Invalid direction. Must be 'up', 'down', 'left', or 'right'.");
            }

            //if (playerId >= 0 && playerId < game.GetEntityManager().GetPlayersMutable().size()) {
            //    game.GetEntityManager().GetPlayersMutable()[playerId].MoveCharacter(moveDirection, game.GetMap());

            //    // Obținem poziția jucătorului după mișcare
            //    Point currentPosition = game.GetEntityManager().GetPlayersMutable()[playerId].GetPosition();

            //    // Pregătim răspunsul JSON cu poziția actualizată
            //    crow::json::wvalue response;
            //    response["playerId"] = playerId;
            //    response["newPosition"]["x"] = currentPosition.GetX();
            //    response["newPosition"]["y"] = currentPosition.GetY();

            //    // Afișează în log pentru debug
            //    std::cout << "Player " << playerId << " moved to position: ("
            //        << currentPosition.GetX() << ", " << currentPosition.GetY() << ")\n";

            //    return crow::response(200, response);
            //}
            //else {
            //    return crow::response(404, "Player not found.");
            //}
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
    //std::thread gameThread(&Game::Run, &game);

    serverThread.join();
    //gameThread.join();

    return 0;
}


//http://localhost:8080/game