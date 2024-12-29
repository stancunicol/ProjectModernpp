#include "ServerUtils.h"

void SendLevelToServer(int level)
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

void SendRegisterToServer(const std::string& username) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        std::string url = "http://localhost:8080/register"; // Endpoint server for register
        std::string jsonPayload = "{\"username\": \"" + username + "\"}";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            qDebug() << "Failed to send register data: " << curl_easy_strerror(res);
        }
        else {
            qDebug() << "Register data sent successfully for user: " << QString::fromStdString(username);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

//function for reception answer from the server
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response)
{
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}
std::string GetServerData(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
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

void PostServerData(const std::string& url, const std::string& jsonPayload)
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        qDebug() << "Failed to initialize CURL.";
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
        qDebug() << "Failed to post data to server: " << curl_easy_strerror(res);
    }
    else {
        qDebug() << "Data posted successfully to server: " << QString::fromStdString(jsonPayload);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}


// verify if the code exist on the server
bool CheckServerCode(const std::string& url)
{
    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        //GET configuration task
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // verify the task and the result
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

void SendMoveToServer(int playerId, const std::string& direction)
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        std::string url = "http://localhost:8080/move";  // URL to the server's move endpoint
        std::string jsonPayload = "{\"playerId\": " + std::to_string(playerId) + ", \"direction\": \"" + direction + "\"}";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            qDebug() << "Failed to send move: " << curl_easy_strerror(res);
        }
        else
        {
            qDebug() << "Movement sent successfully for playerId " << playerId << " direction: " << QString::fromStdString(direction);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

std::string GetPlayerDataByIdFromServer(int playerId)
{
    std::string url = "http://localhost:8080/getPlayerScore?playerId=" + std::to_string(playerId);

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

std::vector<Enemy> GetEnemiesFromServer()
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

std::pair<int, int> GetBaseFromServer()
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
