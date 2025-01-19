#include "ServerUtils.h"

int ServerUtils::userId = -1;

using json = nlohmann::json;

void ServerUtils::SetUserId(int userId) {
    qDebug() << "About to set User ID to:" << userId;
    this->userId = userId;
    qDebug() << "User ID set to:" << this->userId;
}

ServerUtils::ServerUtils()
{
}

void ServerUtils::GetGeneratedCodeFromServer() {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        std::string url = "http://localhost:8080/registerRoom";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &ServerUtils::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        res = curl_easy_perform(curl);

        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        if (res == CURLE_OK && httpCode == 200) {
            try {
                auto jsonResponse = nlohmann::json::parse(response);
                if (jsonResponse.contains("code") && jsonResponse.contains("message")) {
                    std::string code = jsonResponse["code"].get<std::string>();
                    std::string message = jsonResponse["message"].get<std::string>();

                    qDebug() << "Code received from server:" << code;
                    qDebug() << "Message:" << QString::fromStdString(message);

                    this->SetRoomCode(code);
                }
                else {
                    qDebug() << "Invalid JSON response. Missing 'code' or 'message'.";
                }
            }
            catch (const std::exception& e) {
                qDebug() << "Error parsing server response:" << e.what();
            }
        }
        else {
            qDebug() << "Request failed. HTTP Code:" << httpCode;
            qDebug() << "Error:" << curl_easy_strerror(res);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
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

bool ServerUtils::CheckServerCode(const std::string& code, int id)
{
    CURL* curl;
    CURLcode res;
    std::string response;

    nlohmann::json jsonPayload;
    jsonPayload["code"] = code;
    jsonPayload["playerId"] = id;

    std::string jsonString = jsonPayload.dump();

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/joinRoom");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &ServerUtils::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            qDebug() << "Failed to join room on server: " << curl_easy_strerror(res);
        }
        else
        {
            qDebug() << "Server response for joining room: " << QString::fromStdString(response);
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();

    try {
        auto jsonResponse = nlohmann::json::parse(response);
        qDebug() << "Parsed JSON: " << QString::fromStdString(jsonResponse.dump());

        if (jsonResponse.contains("message") && jsonResponse["message"] == "Player joined room successfully.")
        {
            return true;
        }
        else
        {
            qDebug() << "Error: " << QString::fromStdString(jsonResponse["message"].get<std::string>());
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error parsing JSON: " << e.what();
    }

    return false;
}

std::optional<Point> ServerUtils::SendMoveToServer(const std::string& direction) {
    if (GetUserId() == -1) {
        qWarning() << "No connected user. Cannot send move.";
        return std::nullopt;
    }

    CURL* curl;
    CURLcode res;
    std::string response;

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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            auto jsonResponse = nlohmann::json::parse(response);
            if (jsonResponse.contains("status") && jsonResponse["status"] == "success") {
                int x = jsonResponse["newPosition"]["x"];
                int y = jsonResponse["newPosition"]["y"];
                return Point(x, y);
            }
            else {
                qWarning() << "Move rejected. Server response: " << QString::fromStdString(response);
            }
        }
        else {
            qWarning() << "Failed to send move: " << curl_easy_strerror(res);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return std::nullopt;
}

std::vector<std::pair<std::string, int>> ServerUtils::GetPlayerDataByIdFromServer()
{
    std::string url = "http://localhost:8080/getPlayerScore?playerId=" + std::to_string(GetUserId());

    std::string response = GetServerData(url);
    std::vector<std::pair<std::string, int>> playersData;

    if (response.empty()) {
        qDebug() << "No data received from server.";
        return playersData;
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);

        if (jsonResponse.contains("error")) {
            qDebug() << "Error: " << QString::fromStdString(jsonResponse["error"]);
            return playersData;
        }

        if (jsonResponse.contains("players")) {
            const auto& players = jsonResponse["players"];

            for (const auto& player : players) {
                std::string name = player["name"];
                int score = player["score"];
                playersData.emplace_back(name, score);
            }
        }
        else {
            qDebug() << "Response does not contain 'players' field.";
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error parsing server response: " << e.what();
    }

    return playersData;
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

        if (jsonResponse["enemyCount"].get<int>() == 0) {
            qDebug() << "No enemies found.";
            return enemies;
        }

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

size_t ServerUtils::GetLevel() const {
    return m_level;
}

void ServerUtils::SetLevel(const size_t level) {
    m_level = level;
}

void ServerUtils::GetMapFromServer()
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        std::string readBuffer;

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/game");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            qDebug() << "CURL error: " << curl_easy_strerror(res);
        }
        else
        {
            try
            {
                json jsonResponse = json::parse(readBuffer);

                int columns = jsonResponse.value("rows", -1);
                int rows = jsonResponse.value("columns", -1);

                qDebug() << "Rows:" << rows << "Columns:" << columns;

                if (jsonResponse.contains("matrix"))
                {
                    auto jsonMatrix = jsonResponse["matrix"];

                    if (!jsonMatrix.is_array())
                    {
                        qDebug() << "Error: Matrix is not an array.";
                        return;
                    }

                    m_matrix.clear(); 

                    for (size_t rowIndex = 0; rowIndex < jsonMatrix.size(); ++rowIndex) {
                        const auto& row = jsonMatrix[rowIndex];
                        if (!row.is_array()) {
                            qDebug() << "Error: Row in matrix is not an array.";
                            return;
                        }

                        std::vector<int> rowVector;
                        for (const auto& cell : row) {
                            if (cell.is_number_integer()) {
                                rowVector.push_back(cell.get<int>());
                            }
                            else {
                                qDebug() << "Error: Cell is not an integer.";
                            }
                        }
                        // Adăugăm rândul exact așa cum este primit de la server
                        m_matrix.push_back(rowVector);
                    }

                    qDebug() << "Matrix loaded successfully.";
                }
                else
                {
                    qDebug() << "Matrix not found in response.";
                }
            }
            catch (const json::parse_error& e)
            {
                qDebug() << "JSON Parsing error: " << e.what();
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

std::vector<std::vector<int>> ServerUtils::GetMap() const {
    return m_matrix;
}

void ServerUtils::GetPlayerPositionsFromServer() {
    std::string url = "http://localhost:8080/getPlayerPositions";
    try {
        std::string response = GetServerData(url);
        auto json = nlohmann::json::parse(response);

        m_playerPositions.clear();

        for (const auto& player : json["positions"]) {
            Point point = { player["position"]["x"], player["position"]["y"] };
            int id = player["id"];
            m_playerPositions.emplace_back(point, std::to_string(id));
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error fetching player positions:" << e.what();
    }
}


std::vector<std::pair<Point, std::string>> ServerUtils::GetPlayerPositions() {
    return m_playerPositions;
}

void ServerUtils::FetchPlayerStates() {
    uint8_t playerId = GetUserId();

    std::string url = "http://localhost:8080/playerState?playerId=" + std::to_string(playerId);
    std::string response = GetServerData(url);

    if (response.empty()) {
        qWarning() << "Failed to fetch player states.";
        return;
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);
        if (jsonResponse.contains("players") && jsonResponse["players"].is_array()) {
            for (const auto& player : jsonResponse["players"]) {
                int id = player["id"];
                int x = player["position"]["x"];
                int y = player["position"]["y"];
                std::string username = player["username"];

                qDebug() << "Player ID:" << id << "Position: (" << x << "," << y << ") Username:" << QString::fromStdString(username);
            }
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Error parsing player state response: " << e.what();
    }
}

void ServerUtils::FetchEnemyStates(std::vector<Enemy>& enemies) {
    std::string url = "http://localhost:8080/getEnemiesState";
    std::string response = GetServerData(url);

    if (response.empty()) {
        qWarning() << "Failed to fetch enemy states.";
        return;
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);
        if (jsonResponse.contains("enemies") && jsonResponse["enemies"].is_array()) {
            enemies.clear();
            for (const auto& enemy : jsonResponse["enemies"]) {
                int id = enemy["id"];
                int x = enemy["position"]["x"];
                int y = enemy["position"]["y"];

                if (x < 0 || x >= GetMap().size() || y < 0 || y >= GetMap()[0].size() || GetMap()[x][y] == 2) {
                    qWarning() << "Invalid enemy position: (" << x << "," << y << ")";
                    continue;
                }

                enemies.push_back({ id, x, y });
                qDebug() << "Enemy ID:" << id << "Position: (" << x << "," << y << ")";
            }
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Error parsing enemy state response: " << e.what();
    }
}

BulletResponse ServerUtils::FireBullet(const Point& bulletPosition, const Point& bulletDirection) {
    std::string url = "http://localhost:8080/fireBullet";

    // Creăm obiectul JSON pentru trimiterea datelor
    nlohmann::json jsonRequest;

    jsonRequest["playerId"] = GetUserId();
    jsonRequest["position"] = { {"x", bulletPosition.m_x}, {"y", bulletPosition.m_y} };
    jsonRequest["direction"] = { {"x", bulletDirection.m_x}, {"y", bulletDirection.m_y} };

    std::string postData = jsonRequest.dump();

    // Trimitem cererea către server și primim răspunsul
    std::string response = GetServerBulletData(url, postData);

    BulletResponse bulletResponse;

    if (response.empty()) {
        qWarning() << "Failed to fetch bullet status.";
        return bulletResponse;  // Returnăm un obiect gol sau cu valori implicite
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);

        if (jsonResponse.contains("status") && jsonResponse["status"] == "error") {
            qWarning() << "Server error: " << QString::fromStdString(jsonResponse["message"]);
            bulletResponse.success = false;
            return bulletResponse;
        }

        // Verificăm răspunsul de la server
        if (jsonResponse["status"] == "success") {
            bulletResponse.success = true;

            bool hit = jsonResponse["collision"];
            bulletResponse.collision = hit;

            if (hit) {
                int x = jsonResponse["hitPosition"]["y"];
                int y = jsonResponse["hitPosition"]["x"];
                bulletResponse.hitObject = jsonResponse["hitObject"].get<std::string>();
            }
            else {
                bulletResponse.hitObject = "";
            }

            qDebug() << "Server response: " << QString::fromStdString(response);
        }
        else {
            qWarning() << "Server response was not successful.";
            bulletResponse.success = false;
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Error parsing bullet status response: " << e.what();
        bulletResponse.success = false;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return bulletResponse;
}



std::string ServerUtils::GetServerBulletData(const std::string& url, const std::string& postData) {
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

        // Setăm metoda POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Setăm datele pentru body-ul cererii
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

        // Setăm header-ul pentru a trimite JSON
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

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
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return response;
}

bool ServerUtils::GetBaseState() {
    std::string url = "http://localhost:8080/getBaseState";

    std::string response = GetServerData(url);

    if (response.empty()) {
        qDebug() << "No data received for base state.";
        return false; 
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);

        if (jsonResponse.contains("status") && jsonResponse["status"] == "success") {
            if (jsonResponse.contains("baseState")) {
                std::string baseState = jsonResponse["baseState"].get<std::string>();
                bool isIntact = (baseState == "intact");
                qDebug() << "Base state fetched successfully. Intact:" << isIntact;
                return isIntact;
            }
            else {
                qDebug() << "JSON response missing 'baseState' field.";
            }
        }
        else {
            qDebug() << "Invalid JSON structure or status.";
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error parsing base state data: " << e.what();
    }

    return false; // Default to destroyed in case of error
}

