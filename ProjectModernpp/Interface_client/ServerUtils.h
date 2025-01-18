#pragma once

#include <string>
#include <QDebug>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <iostream>
#include <QTimer>

struct Enemy {
    int id;
    int x;
    int y;
};

struct Bomb {
    int id;
    int x;
    int y;
};

struct Point {
    int8_t m_x, m_y;

    Point(int8_t x, int8_t y) : m_x(x), m_y(y) {}
};

class ServerUtils {
private:

    std::string lastStatus;
    static int userId;
    std::string roomCode;
    size_t m_level;
    std::vector<std::vector<int>> m_matrix;
    std::vector<std::pair<Point, std::string>> m_playerPositions;
    //std::vector<Enemy>& m_enemies;

public:
    ServerUtils();

    void GetGeneratedCodeFromServer();

    void SendLevelToServer(int level);

    void SendUserRequestToServer(const std::string& username);

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response);

    std::string GetServerData(const std::string& url);

    void PostServerData(const std::string& url, const std::string& jsonPayload);

    bool CheckServerCode(const std::string& code, int id);

    std::optional<Point> SendMoveToServer(const std::string& direction);

    std::string GetPlayerDataByIdFromServer();
    std::vector<Enemy> GetEnemiesFromServer();
    std::pair<int, int> GetBaseFromServer();
    std::vector<Bomb> GetBombsFromServer();

    void SetUserId(int id);
    int GetUserId() const { return userId; }

    void SetRoomCode(std::string code) { this->roomCode = code; }
    std::string GetRoomCode() const { return roomCode; }

    void SetStatus(const std::string& status) { lastStatus = status; }
    std::string GetStatus() const { return lastStatus; }

    size_t GetLevel() const;
    void SetLevel(const size_t level);

    void GetMapFromServer();
    std::vector<std::vector<int>> GetMap() const;

    void GetPlayerPositionsFromServer();
    std::vector<std::pair<Point, std::string>> GetPlayerPositions();

    void FetchPlayerStates();
    void FetchEnemyStates(std::vector<Enemy>& enemies);
};

