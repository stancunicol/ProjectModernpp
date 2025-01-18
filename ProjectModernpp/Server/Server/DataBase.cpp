#include "DataBase.h"

DataBase::DataBase(const std::string& dbName)
{
    if (sqlite3_open(dbName.c_str(), &m_db))
    {
        std::cerr << "Error opening base: " << sqlite3_errmsg(m_db) << '\n';
        m_db = nullptr;
    }
}

DataBase::~DataBase()
{
    if (m_db)
    {
        sqlite3_close(m_db);
    }
}

void DataBase::Initialize()
{
    const std::string createRoomsTableQuery = R"(
        CREATE TABLE IF NOT EXISTS Rooms (
            id INTEGER PRIMARY KEY,
            roomCode TEXT UNIQUE NOT NULL,
            mapData TEXT, -- Datele hărții, codificate ca text JSON sau alt format
            createdAt DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";
    executeQuery(createRoomsTableQuery);

    const std::string createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS GameData (
            id INTEGER PRIMARY KEY,
            playerName TEXT UNIQUE,
            score INTEGER, 
            level INTEGER,
            roomCode TEXT,
            lastConnected DATETIME DEFAULT NULL
        );
    )";
    executeQuery("PRAGMA foreign_keys = ON;");
    executeQuery(createTableQuery);
}

void DataBase::InsertGameData(const std::string& playerName, uint16_t score, uint8_t level)
{
    const std::string insertQuery =
        "INSERT INTO GameData (playerName, score, level, roomCode) "
        "VALUES ('" + playerName + "', " + std::to_string(score) + ", " + std::to_string(level) + ", NULL ) "
        "ON CONFLICT(playerName) DO UPDATE SET "
        "score = " + std::to_string(score) + ", "
        "level = " + std::to_string(level) + "";

    executeQuery(insertQuery);
    UpdateLastConnected(playerName);
}

void DataBase::UpdateGameData(const std::string& playerName, uint16_t score)
{
    const std::string updateQuery = "UPDATE GameData SET score=" + std::to_string(score) + " WHERE playerName='" + playerName + "';";
    executeQuery(updateQuery);
}

void DataBase::DeleteGameData()
{
    const std::string deleteQuery = "DELETE FROM GameData;";
    executeQuery(deleteQuery);
}

bool DataBase::UserExists(const std::string& username)
{
    std::lock_guard<std::mutex> lock(m_dbMutex);

    const std::string query = "SELECT EXISTS(SELECT 1 FROM GameData WHERE playerName = ?);";
    sqlite3_stmt* stmt = nullptr;
    bool exists = false;

    try {
        if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << '\n';
            return false;
        }

        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            exists = sqlite3_column_int(stmt, 0) != 0;
        }
    }
    catch (...) {
        std::cerr << "Unexpected error during query execution.\n";
    }

    UpdateLastConnected(username);

    if (stmt) sqlite3_finalize(stmt);

    return exists;
}

void DataBase::AddUser(const std::string& username)
{
    std::lock_guard<std::mutex> lock(m_dbMutex);

    const std::string insertQuery = "INSERT INTO GameData (playerName, score, level, roomCode) "
        "VALUES ('" + username + "', 0, 0, NULL);";

    executeQuery(insertQuery);
    UpdateLastConnected(username);
}

int DataBase::GetUserId(const std::string& playerName)
{
    std::lock_guard<std::mutex> lock(m_dbMutex);

    const std::string query = "SELECT id FROM GameData WHERE playerName = ?";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(m_db) << std::endl;
        return -1;
    }

    sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);

    int8_t userId = -1;
    if (rc == SQLITE_ROW)
    {
        userId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return userId;
}


void DataBase::UpdateLevel(const std::string& playerName, uint8_t level)
{
    const std::string updateLevelQuery = R"(
        UPDATE GameData
        SET level = ? 
        WHERE playerName = ?;
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, updateLevelQuery.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, level);
    sqlite3_bind_text(stmt, 2, playerName.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error executing query: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void DataBase::UpdateDataByRoomCode(uint8_t playerId, const std::string& code)
{
    std::lock_guard<std::mutex> lock(m_dbMutex);

    uint8_t level = 0;
    std::string roomCode;

    {
        std::string query = "SELECT level, roomCode FROM GameData WHERE roomCode = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare query: " << sqlite3_errmsg(m_db) << std::endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_STATIC);  // Bind roomCode

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            level = static_cast<uint8_t>(sqlite3_column_int(stmt, 0));
            roomCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        }
        else
        {
            std::cerr << "No player found with roomCode " << code << std::endl;
            sqlite3_finalize(stmt);
            return;
        }

        sqlite3_finalize(stmt);
    }

    std::string updateQuery = R"(
        UPDATE GameData
        SET level = ?,
            roomCode = ?
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, updateQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare update query: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, level);
    sqlite3_bind_text(stmt, 2, roomCode.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, playerId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Failed to update player data: " << sqlite3_errmsg(m_db) << std::endl;
    }
    else {
        std::cout << "Successfully updated player data for playerId " << std::to_string(playerId)
            << " with roomCode " << roomCode << " and level " << static_cast<int>(level) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void DataBase::InsertRoomCode(const std::string& playerName, const std::string& roomCode)
{
    const std::string checkRoomQuery = "SELECT COUNT(*) FROM GameData WHERE roomCode = '" + roomCode + "';";

    sqlite3_stmt* stmt;
    uint8_t playerCount = 0;

    if (sqlite3_prepare_v2(m_db, checkRoomQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, roomCode.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            playerCount = sqlite3_column_int(stmt, 0);
        }
        else std::cerr << "Error executing checkRoomQuery: " << sqlite3_errmsg(m_db) << std::endl;
    }
    else std::cerr << "Error preparing checkRoomQuery: " << sqlite3_errmsg(m_db) << std::endl;

    sqlite3_finalize(stmt);

    if (playerCount >= 4)
    {
        throw std::runtime_error("Room is full. Cannot join.");
    }

    const std::string addCodeQuery =
        "UPDATE GameData "
        "SET roomCode = '" + roomCode + "', "
        "level = CASE WHEN level = 0 OR level != (SELECT level FROM GameData WHERE roomCode = '" + roomCode + "' ORDER BY lastConnected LIMIT 1) "
        "THEN (SELECT level FROM GameData WHERE roomCode = '" + roomCode + "' ORDER BY lastConnected LIMIT 1) "
        "ELSE level END "
        "WHERE playerName = '" + playerName + "';";

    sqlite3_stmt* stmtAdd;
    int rc = sqlite3_prepare_v2(m_db, addCodeQuery.c_str(), -1, &stmtAdd, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmtAdd, 1, roomCode.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtAdd, 2, roomCode.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtAdd, 3, playerName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtAdd, 4, playerName.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmtAdd);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error executing query: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmtAdd);
    UpdateLastConnected(playerName);
}

std::optional<std::string> DataBase::FindRoomByCode(const std::string& code)
{
    const std::string countQuery = "SELECT COUNT(*) FROM GameData WHERE roomCode = '" + code + "';";
    sqlite3_stmt* countStmt;
    int playerCount = 0;

    if (sqlite3_prepare_v2(m_db, countQuery.c_str(), -1, &countStmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(countStmt) == SQLITE_ROW)
        {
            playerCount = sqlite3_column_int(countStmt, 0);
        }
    }
    else
    {
        std::cerr << "Error checking the number of players: " << sqlite3_errmsg(m_db) << '\n';
    }

    sqlite3_finalize(countStmt);

    if (playerCount >= 4)
    {
        std::cerr << "The room is full: " << code << '\n';
        return {};
    }

    const std::string findQuery = "SELECT playerName FROM GameData WHERE roomCode = '" + code + "';";
    sqlite3_stmt* stmt;
    std::optional<std::string> result;

    if (sqlite3_prepare_v2(m_db, findQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
    }
    else
    {
        std::cerr << "Error searching the room code: " << sqlite3_errmsg(m_db) << '\n';
    }

    sqlite3_finalize(stmt);
    return result;
}


std::optional<std::string> DataBase::GetRecentPlayer()
{
    const std::string getPlayerQuery = R"(
        SELECT playerName
        FROM GameData
        ORDER BY lastConnected DESC
        LIMIT 1;
    )";

    sqlite3_stmt* stmt;
    std::optional<std::string> result;

    if (sqlite3_prepare_v2(m_db, getPlayerQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
    }
    else
    {
        std::cerr << "Error executing query: " << sqlite3_errmsg(m_db) << '\n';
    }

    sqlite3_finalize(stmt);
    return result;
}

void DataBase::UpdateLastConnected(const std::string& playerName)
{

    const std::string updateQuery = R"(
        UPDATE GameData
        SET lastConnected = CURRENT_TIMESTAMP
        WHERE playerName = ')" + playerName + R"(';
    )";

    executeQuery(updateQuery);
}

sqlite3* DataBase::GetDatabaseConnection() const
{
    return m_db;
}

std::vector<std::string> DataBase::GetPlayersFromRoom(const std::string& roomCode)
{
    std::vector<std::string> players;
    const std::string query = "SELECT playerName FROM GameData WHERE roomCode = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(m_db) << std::endl;
        return players;
    }

    if (sqlite3_bind_text(stmt, 1, roomCode.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
    {
        std::cerr << "Error binding parameter: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_finalize(stmt);
        return players;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char* playerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (playerName)
        {
            players.emplace_back(playerName);
        }
    }

    if (sqlite3_errcode(m_db) != SQLITE_DONE && sqlite3_errcode(m_db) != SQLITE_OK)
    {
        std::cerr << "Error executing query: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return players;
}

std::vector<uint8_t> DataBase::GetPlayersFromRoom(uint8_t playerId)
{
    std::vector<uint8_t> playerIds;

    const std::string query = R"(
        SELECT p1.id
        FROM GameData AS p1
        JOIN GameData AS p2 ON p1.roomCode = p2.roomCode
        WHERE p2.id = ? AND p1.id != ?;
    )";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(m_db) << std::endl;
        return playerIds;
    }

    sqlite3_bind_int(stmt, 1, playerId);
    sqlite3_bind_int(stmt, 2, playerId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        playerIds.push_back(static_cast<uint8_t>(sqlite3_column_int(stmt, 0)));
    }

    if (sqlite3_errcode(m_db) != SQLITE_DONE && sqlite3_errcode(m_db) != SQLITE_OK)
    {
        std::cerr << "Error executing query: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return playerIds;
}

std::optional<std::tuple<std::string, uint8_t>> DataBase::GetPlayerDataById(uint8_t playerId) const
{
    const std::string selectQuery = "SELECT playerName, score FROM GameData WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, selectQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, playerId);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int score = sqlite3_column_int(stmt, 1);

            sqlite3_finalize(stmt);

            return std::make_tuple(name, score);
        }
    }
    else
    {
        std::cerr << "Error executing query: " << sqlite3_errmsg(m_db) << '\n';
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<std::tuple<std::string, uint16_t, uint8_t, std::string>> DataBase::GetGameData() const
{
    const std::string selectQuery = "SELECT playerName, score, level, roomCode FROM GameData;";

    sqlite3_stmt* stmt;
    std::vector<std::tuple<std::string, uint16_t, uint8_t, std::string>> results;

    if (sqlite3_prepare_v2(m_db, selectQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int score = sqlite3_column_int(stmt, 1);
            int level = sqlite3_column_int(stmt, 2);

            std::string roomCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            results.emplace_back(name, score, level, roomCode);
        }
    }
    else
    {
        std::cerr << "Error executing query: " << sqlite3_errmsg(m_db) << '\n';
    }

    sqlite3_finalize(stmt);
    return results;
}

void DataBase::executeQuery(const std::string& query)
{
    char* errMsg = nullptr;

    if (sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "Error executing query: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

std::ostream& operator<<(std::ostream& out, const DataBase& db)
{
    auto data = db.GetGameData();

    if (data.empty())
    {
        out << "Database is empty.\n";
        return out;
    }

    out << "Player Name\tScore\tLevel\tRoom Code\n";
    out << "------------------------------------------\n";

    for (const auto& row : data)
    {
        const std::string& playerName = std::get<0>(row);
        uint16_t score = std::get<1>(row);
        uint8_t level = std::get<2>(row);

        const std::string& roomCode = std::get<3>(row);

        out << playerName << "\t\t" << score << "\t" << level << "\t" << roomCode << "\n";
    }

    return out;
}

void DataBase::DeleteRoom(const std::string& roomCode)
{
    const std::string updateQuery = "UPDATE GameData SET roomCode = NULL WHERE roomCode = '" + roomCode + "'";
    executeQuery(updateQuery);

    const std::string checkQuery = "SELECT COUNT(*) FROM GameData WHERE roomCode = '" + roomCode + "';";
    sqlite3_stmt* stmt;
    int playerCount = 0;

    if (sqlite3_prepare_v2(m_db, checkQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            playerCount = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);

    if (playerCount == 0)
    {
        const std::string deleteQuery = "DELETE FROM Rooms WHERE roomCode = '" + roomCode + "';";
        executeQuery(deleteQuery);
    }
}

void DataBase::UpdateRoomData(const std::string& roomCode, const std::string& newMapData)
{
    const std::string updateQuery = "UPDATE Rooms SET mapData = '" + newMapData + "'WHERE roomCode = '" + roomCode + "';";

    executeQuery(updateQuery);
}

void DataBase::RemovePlayerFromRoom(int playerId)
{
    const std::string updateQuery = R"(
        UPDATE GameData SET roomCode = NULL WHERE id = ?;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, updateQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, playerId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void DataBase::AddRoom(const std::string& roomCode)
{
    const std::string insertQuery = R"(
        INSERT INTO GameRooms (roomCode, status) VALUES (?, ?);
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, insertQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, roomCode.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, "available", -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(m_db) << std::endl;
    }

    sqlite3_finalize(stmt);
}