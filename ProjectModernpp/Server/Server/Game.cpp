#include "Game.h"
#include <Windows.h>
#include <random>

std::string Game::GenerateRoomCode()
{
    const char characters[] = "0123456789";
    const size_t codeLength = 6;
    std::string code;
    code.reserve(codeLength);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(characters) - 2);

    for (size_t i = 0; i < codeLength; ++i) {
        code += characters[dis(gen)];
    }

    return code;
}

Game::Game()
    : m_map(), m_entityManager(m_map), m_database("GameData.db") {
}

Game::~Game() {}

void Game::InitializeGame() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, m_map.GetWidth() - 1);
    std::uniform_int_distribution<> disY(0, m_map.GetHeight() - 1);

    m_entityManager.PlaceBase(m_map);

    int enemiesToPlace = 1 + m_map.GetLevel();
    int id = 1;

    while (enemiesToPlace > 0) {
        Enemy enemy(m_map);
        enemy.PlaceCharacter(m_map);
        m_entityManager.AddEnemy(id, enemy);
        enemiesToPlace--;
        id++;
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
        if (player.second.HasMovementCommand()) {
            Point direction = player.second.GetMoveDirection();
            player.second.MoveCharacter(direction, m_map);
        }
    }
}

std::mutex& Game::GetGameMutex()
{
    return m_gameMutex;
}

void Game::EndGame(const std::string& winner)
{
    std::cout << "Final Scores: \n";
    for (const auto& player : m_entityManager.GetPlayers()) {
        std::cout << player.second.GetName() << " scored " << player.second.GetScore() << " points.\n";
    }
    std::cout << "Game Over! " << winner << " Wins!\n";
}

void Game::SetLevel(int newLevel) {
    std::lock_guard<std::mutex> lock(m_gameMutex);
    m_map.Reset(newLevel);

    auto& players = m_entityManager.GetPlayersMutable();
    for (auto& [id, player] : players) {
        player.ResetPositions(m_map);
        player.PlaceCharacter();
    }

    InitializeGame();
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
    std::lock_guard<std::mutex> lock(m_roomMutex);
    std::string code = GenerateRoomCode();
    auto map = std::make_unique<GameMap>();
    m_roomManager.CreateRoom(code, std::move(map));
    return code;
}

std::optional<std::string> Game::JoinRoom(const std::string& code, int playerId) {
    std::lock_guard<std::mutex> lock(m_roomMutex);
    Room* room = m_roomManager.GetRoom(code);
    if (!room)
        return std::nullopt;

    std::string playerName;
    auto playerData = m_database.GetPlayerDataById(playerId);
    if (playerData.has_value())
        playerName = std::get<0>(playerData.value());

    if (room->AddPlayer(playerId))
        return room->GetCode();

    return std::nullopt;
}

bool Game::LeaveRoom(const std::string& code, const int& playerId) {
    std::lock_guard<std::mutex> lock(m_roomMutex);
    Room* room = m_roomManager.GetRoom(code);
    if (room && room->RemovePlayer(playerId)) {
        if (room->GetPlayers().empty())
            m_roomManager.RemoveRoom(code);
        return true;
    }
    return false;
}

void Game::CloseRoom(const std::string& code) {
    std::lock_guard<std::mutex> lock(m_roomMutex);

    if (m_roomManager.RemoveRoom(code))
        std::cout << "Room " << code << " closed and resources cleaned up.\n";
}

RoomManager Game::GetRoomManager() {
    return m_roomManager;
}

Room* Game::GetRoom(const std::string& code)
{
    return m_roomManager.GetRoom(code);
}
