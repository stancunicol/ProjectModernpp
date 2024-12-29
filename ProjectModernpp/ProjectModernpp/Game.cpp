﻿#include "Game.h"
#include <Windows.h>
#include <random>

std::string Game::GenerateRoomCode()
{
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string code;
    for (int i = 0; i < 8; i++)
        code += characters[rand() % (sizeof(characters) - 1)];
    return code;
}

Game::Game()
    : m_map(), m_entityManager(m_map), m_database("GameData.db"), m_setLevel(false) {
    //m_database.DeleteGameData();
    m_database.Initialize();
}

Game::~Game() {}

void Game::InitializeGame() { //here, we initialize the game
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, m_map.GetWidth() - 1);
    std::uniform_int_distribution<> disY(0, m_map.GetHeight() - 1);
    Point randomPos(disX(gen), disY(gen));

    m_entityManager.PlaceBase(m_map);

    std::vector<std::string> players(4);

    for (const auto& playerName : players) {
        Player newPlayer(playerName, m_map);
        m_entityManager.AddPlayer(newPlayer);
    }

    int enemiesToPlace = 1 + m_map.GetLevel();

    while (enemiesToPlace > 0) {
        Enemy enemy(m_map);
        enemy.PlaceCharacter(m_map);
        m_entityManager.AddEnemy(enemy);
        enemiesToPlace--;
    }

    int bombsToPlace = m_map.GetLevel();
    while (bombsToPlace > 0) {
        Point randomPos(rand() % m_map.GetHeight(), rand() % m_map.GetWidth());

        if (m_map.GetMap()[randomPos.GetX()][randomPos.GetY()] == CellType::UNBREAKABLE_WALL) {
            float randomActivationTime = static_cast<float>(rand() % 10 + 5); // 10-15 s
            m_entityManager.AddBomb(Bomb(randomPos, randomActivationTime));
            bombsToPlace--;
        }
    }
}

void Game::Run() {
    static float elapsedTime = 0.0f;
    const float shootInterval = 0.3f;
    static float enemyShootTimer = 0.0f;

    // Așteaptă până când nivelul este setat
    {
        std::unique_lock<std::mutex> lock(mutex);
        condition.wait(lock, [this]() { return m_setLevel; });
    }

    while (m_entityManager.GetBase().GetLife()) {
        system("CLS");

        // Here we should check if the player movement data is updated by the server
        // For example, update the player's movement based on the server's handling
        UpdatePlayerMovements();

        // Update entities (e.g., enemies, bombs)
        elapsedTime += 0.15f;
        if (elapsedTime >= shootInterval) {
            m_entityManager.UpdateEntities(m_map, elapsedTime);
            elapsedTime = 0.0f;
        }

        // Display the game map and entities
        m_map.Display();

        // Sleep to control the game loop speed
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    std::cout << m_database;
    EndGame(m_entityManager.GetWinner());
}

void Game::UpdatePlayerMovements() {
    // Loop through all the players and move them based on the server commands
    for (auto& player : m_entityManager.GetPlayersMutable()) {
        if (player.HasMovementCommand()) {
            Point direction = player.GetMoveDirection();
            player.MoveCharacter(direction, m_map);
        }
    }
}

void Game::EndGame(const std::string& winner)
{
    for (size_t i = 0; i < m_entityManager.GetPlayers().size(); i++) {
        std::cout << m_entityManager.GetPlayers()[i].GetName() << " score: "
            << static_cast<int>(m_entityManager.GetPlayers()[i].GetScore()) << '\n'; \
    }
    std::cout << "The game is over! " << winner << " WON!\n";
}

void Game::SetLevel(int newLevel) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        m_map.Reset(newLevel);
        m_setLevel = true;
    }
    // Reinitializam jocul
    InitializeGame();

    condition.notify_all(); // Notifică bucla principală că poate continua
    std::cout << "Game level updated to: " << newLevel << '\n';
}

crow::json::wvalue Game::TranformInJson() {
    crow::json::wvalue jsonMap;
    for (size_t i = 0; i < m_map.GetMap().size(); i++) {
        for (size_t j = 0; j < m_map.GetMap()[i].size(); j++) {
            jsonMap["matrix"][i][j] = static_cast<int>(m_map.GetMap()[i][j]);
        }

    }
    jsonMap["level"] = m_map.GetLevel();
    jsonMap["rows"] = m_map.GetHeight();
    jsonMap["columns"] = m_map.GetWidth();

    return jsonMap;
}

crow::json::wvalue Game::GetGameStateAsJson() {
    return TranformInJson();
}

std::string Game::CreateRoom() {
    std::lock_guard<std::mutex> lock(roomMutex);
    std::string code = GenerateRoomCode();
    m_rooms.emplace(code, Room(code));
    return code;
}

std::optional<std::string> Game::JoinRoom(const std::string& code, const std::string& playerName) {
    std::lock_guard<std::mutex> lock(roomMutex);
    auto it = m_rooms.find(code);
    if (it != m_rooms.end() && it->second.AddPlayer(playerName))
        return it->second.GetCode();
    return std::nullopt;
}

bool Game::LeaveRoom(const std::string& code, const std::string& playerName) {
    std::lock_guard<std::mutex> lock(roomMutex);
    auto it = m_rooms.find(code);
    if (it != m_rooms.end() && it->second.RemovePlayer(playerName)) {
        if (it->second.GetPlayers().empty())
            m_rooms.erase(it);
        return true;
    }
    return false;
}

std::optional<Room> Game::GetRoom(const std::string& code) {
    std::lock_guard<std::mutex> lock(roomMutex);
    auto it = m_rooms.find(code);
    if (it != m_rooms.end())
        return it->second;
    return std::nullopt;
}
