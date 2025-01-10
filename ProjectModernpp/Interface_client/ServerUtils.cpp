﻿#include "ServerUtils.h"

int ServerUtils::userId = -1;

void ServerUtils::SetUserId(int userId) {
    qDebug() << "About to set User ID to:" << userId;
    this->userId = userId;
    qDebug() << "User ID set to:" << this->userId;
}

void ServerUtils::SendLevelToServer(int level)
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        std::string url = "http://localhost:8080/submitLevel";
        std::string jsonPayload = "{\"level\": " + std::to_string(level) + "}";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            qDebug() << "Failed to send level: " << curl_easy_strerror(res);
        }
        else
        {
            qDebug() << "Level sent successfully: " << level;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void ServerUtils::SendUserRequestToServer(const std::string& username) {
    try {
        CURL* curl;
        CURLcode res;
        std::string response;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            std::string url = "http://localhost:8080/user";

            nlohmann::json jsonPayload;
            jsonPayload["username"] = username;
            std::string jsonPayloadStr = jsonPayload.dump();

            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");

            curl_easy_setopt(curl, CURLoption::CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayloadStr.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &ServerUtils::WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            res = curl_easy_perform(curl);

            long httpCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

            if (res == CURLE_OK && httpCode == 200) {
                try {
                    auto jsonResponse = nlohmann::json::parse(response);
                    if (jsonResponse.contains("status") && jsonResponse.contains("userId") && jsonResponse["userId"].is_number()) {
                        (*this).SetStatus(jsonResponse["status"].get<std::string>());

                        (*this).SetUserId(jsonResponse["userId"].get<int>());

                        qDebug() << "Request successful. Status:" << QString::fromStdString(GetStatus())
                            << "User ID:" << GetUserId();
                    }
                    else {
                        qDebug() << "Invalid JSON response. Missing 'status' or 'userId'.";
                    }
                }
                catch (const std::exception& e) {
                    qDebug() << "Error parsing server response:" << e.what();
                }
            }
            else {
                qDebug() << "Failed to connect. HTTP Code:" << httpCode;
                qDebug() << "Error:" << curl_easy_strerror(res);
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
    }
    catch (const std::exception& e) {
        qDebug() << "Exception occurred in SendUserRequestToServer: " << e.what();
    }
}

std::string ServerUtils::GetServerData(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &ServerUtils::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // Timeout de 10 secunde

        long httpCode = 0;
        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        if (res != CURLE_OK) {
            qDebug() << "Failed to fetch data from server: " << curl_easy_strerror(res);
        }
        else if (httpCode != 200) {
            qDebug() << "HTTP Error: " << httpCode << " Response: " << QString::fromStdString(response);
        }
        else {
            qDebug() << "Data fetched successfully from server: " << QString::fromStdString(response);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return response;
}

size_t ServerUtils::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    if (userData) {
        userData->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    return 0;
}

void ServerUtils::PostServerData(const std::string& url, const std::string& jsonPayload) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        qCritical() << "Failed to initialize CURL.";
        curl_global_cleanup();
        return;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        qWarning() << "Failed to post data to server:" << curl_easy_strerror(res);
    }
    else {
        qDebug() << "Data posted successfully to server:" << QString::fromStdString(jsonPayload);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

bool ServerUtils::CheckServerCode(const std::string& url)
{
    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &ServerUtils::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            qDebug() << "Failed to check code on server: " << curl_easy_strerror(res);
        }
        else
        {
            qDebug() << "Server response for code check: " << QString::fromStdString(response);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    try {
        auto jsonResponse = nlohmann::json::parse(response);
        qDebug() << "Parsed JSON: " << QString::fromStdString(jsonResponse.dump());

        if (jsonResponse.contains("status") && jsonResponse["status"] == "success")
        {
            return true;
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error parsing JSON: " << e.what();
    }

    return false;
}

void ServerUtils::SendMoveToServer(const std::string& direction) {

    if (GetUserId() == 0) {
        qDebug() << "No connected user. Cannot send move.";
        return;
    }

    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        std::string url = "http://localhost:8080/move";
        std::string jsonPayload = "{\"playerId\": " + std::to_string(GetUserId()) + ", \"direction\": \"" + direction + "\"}";

        qDebug() << "Sending move request. Payload: " << QString::fromStdString(jsonPayload);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            qDebug() << "Failed to send move: " << curl_easy_strerror(res);
        }
        else {
            qDebug() << "Movement sent successfully for ConnectedUserId " << GetUserId() << " direction: " << QString::fromStdString(direction);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

std::string ServerUtils::GetPlayerDataByIdFromServer()
{
    std::string url = "http://localhost:8080/getPlayerScore?playerId=" + std::to_string(GetUserId());

    std::string response = GetServerData(url);

    if (response.empty()) {
        qDebug() << "No data received from server.";
        return "";
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);

        if (jsonResponse.contains("error")) {
            qDebug() << "Error: " << QString::fromStdString(jsonResponse["error"]);
            return "";
        }

        std::string name = jsonResponse["name"];
        int score = jsonResponse["score"];
        qDebug() << "Player Name: " << QString::fromStdString(name);
        qDebug() << "Player Score: " << score;
    }
    catch (const std::exception& e) {
        qDebug() << "Error parsing server response: " << e.what();
    }

    return response;
}

std::vector<Enemy> ServerUtils::GetEnemiesFromServer()
{
    std::vector<Enemy> enemies;
    std::string url = "http://localhost:8080/getEnemies";

    std::string response = GetServerData(url);

    if (response.empty()) {
        qDebug() << "No data received for enemies.";
        return enemies;
    }

    qDebug() << "Raw server response: " << QString::fromStdString(response);

    try {
        auto jsonResponse = nlohmann::json::parse(response);

        if (!jsonResponse.is_object() || !jsonResponse.contains("enemies") || !jsonResponse["enemies"].is_array()) {
            qDebug() << "Invalid JSON structure. Aborting.";
            return enemies;
        }

        for (const auto& enemy : jsonResponse["enemies"]) {
            if (!enemy.contains("id") || !enemy.contains("x") || !enemy.contains("y")) {
                qDebug() << "Incomplete enemy data. Skipping this entry.";
                continue;
            }
            enemies.push_back({
                enemy["id"].get<int>(),
                enemy["x"].get<int>(),
                enemy["y"].get<int>()
                });
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error parsing enemies data: " << e.what();
    }

    return enemies;
}

std::pair<int, int> ServerUtils::GetBaseFromServer()
{
    std::string url = "http://localhost:8080/getBase";

    std::string response = GetServerData(url);

    if (response.empty()) {
        qDebug() << "No data received for base position.";
        return std::make_pair(-1, -1);
    }
    try {
        auto jsonResponse = nlohmann::json::parse(response);

        if (jsonResponse.contains("position") &&
            jsonResponse["position"].contains("x") &&
            jsonResponse["position"].contains("y")) {

            int x = jsonResponse["position"]["x"];
            int y = jsonResponse["position"]["y"];
            return std::make_pair(x, y);
        }
        else {
            qDebug() << "Invalid JSON structure for base position.";
            return std::make_pair(-1, -1);
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error parsing base position data: " << e.what();
        return std::make_pair(-1, -1);
    }
}

std::vector<Bomb> ServerUtils::GetBombsFromServer()
{
    std::vector<Bomb> bombs;
    std::string url = "http://localhost:8080/getBombs";

    std::string response = GetServerData(url);

    if (response.empty()) {
        qDebug() << "No bomb data received from server.";
        return bombs;
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);

        if (jsonResponse.contains("bombs") && jsonResponse["bombs"].is_array()) {
            for (const auto& bomb : jsonResponse["bombs"]) {
                if (bomb.contains("id") && bomb.contains("x") && bomb.contains("y")) {
                    bombs.push_back({
                        bomb["id"].get<int>(),
                        bomb["x"].get<int>(),
                        bomb["y"].get<int>()
                        });
                }
                else {
                    qDebug() << "Incomplete bomb data. Skipping this entry.";
                }
            }
        }
        else {
            qDebug() << "Invalid JSON structure. Bomb data not found.";
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error parsing bomb data: " << e.what();
    }

    return bombs;
}
