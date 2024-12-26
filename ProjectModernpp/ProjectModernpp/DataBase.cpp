#include "DataBase.h"

DataBase::DataBase(const std::string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db)) {
        std::cerr << "Eroare la deschiderea bazei de date: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

DataBase::~DataBase() {
    if (db) {
        sqlite3_close(db);
    }
}

void DataBase::Initialize() {

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
    executeQuery(createTableQuery);
}

void DataBase::InsertGameData(const std::string& playerName, uint32_t score, uint8_t level)
{
    const std::string insertQuery =
        "INSERT INTO GameData (playerName, score, level, roomCode) "
        "VALUES ('" + playerName + "', " + std::to_string(score) + ", " + std::to_string(level) + ", NULL ) "
        "ON CONFLICT(playerName) DO UPDATE SET "
        "score = " + std::to_string(score) + ", "
        "level = " + std::to_string(level) + "";
    executeQuery(insertQuery);
    updateLastConnected(playerName);
}

void DataBase::UpdateGameData(const std::string& playerName, uint32_t score)
{
    const std::string updateQuery = "UPDATE GameData SET score=" + std::to_string(score) + " WHERE playerName='" + playerName + "';";
    executeQuery(updateQuery);
}

void DataBase::DeleteGameData()
{
    const std::string deleteQuery = "DELETE FROM GameData;";
    executeQuery(deleteQuery);
}

bool DataBase::UserExists(const std::string& username) {
    const std::string query = "SELECT EXISTS(SELECT 1 FROM GameData WHERE playerName = ?);";
    sqlite3_stmt* stmt = nullptr;
    bool exists = false;

    try {
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
            return false;
        }

        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(stmt, 0) != 0;
        }
    }
    catch (...) {
        std::cerr << "Unexpected error during query execution.\n";
    }
    updateLastConnected(username);
    if (stmt) sqlite3_finalize(stmt);
    return exists;
}

void DataBase::AddUser(const std::string& username)
{
    const std::string insertQuery = "INSERT INTO GameData (playerName, score, level, roomCode) "
        "VALUES ('" + username + "', 0, 0, NULL);";
    executeQuery(insertQuery);
    updateLastConnected(username);
}

void DataBase::UpdateLevel(const std::string& playerName, int level)
{
    const std::string updateLevelQuery = R"(
        UPDATE GameData
        SET level = ?
        WHERE playerName = ?;
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, updateLevelQuery.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Eroare la pregătirea interogării: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, level);
    sqlite3_bind_text(stmt, 2, playerName.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Eroare la executarea interogării: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    //executeQuery(updateLevelQuery);
}


void DataBase::InsertRoomCode(const std::string& playerName, const std::string& roomCode)
{
    const std::string checkRoomQuery = "SELECT COUNT(*) FROM GameData WHERE roomCode = '" + roomCode + "';";
    sqlite3_stmt* stmt;
    int playerCount = 0;

    if (sqlite3_prepare_v2(db, checkRoomQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            playerCount = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);

    if (playerCount >= 4) {
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
    int rc = sqlite3_prepare_v2(db, addCodeQuery.c_str(), -1, &stmtAdd, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Eroare la pregătirea interogării: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmtAdd, 1, roomCode.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtAdd, 2, roomCode.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtAdd, 3, playerName.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmtAdd);
    if (rc != SQLITE_DONE) {
        std::cerr << "Eroare la executarea interogării: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmtAdd);
    updateLastConnected(playerName);
}




std::optional<std::string> DataBase::FindRoomByCode(const std::string& code)
{
    const std::string countQuery = "SELECT COUNT(*) FROM GameData WHERE roomCode = '" + code + "';";
    sqlite3_stmt* countStmt;
    int playerCount = 0;

    if (sqlite3_prepare_v2(db, countQuery.c_str(), -1, &countStmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(countStmt) == SQLITE_ROW) {
            playerCount = sqlite3_column_int(countStmt, 0);
        }
    }
    else {
        std::cerr << "Eroare la verificarea numărului de jucători: " << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(countStmt);

    if (playerCount >= 4) {
        std::cerr << "Camera este plină: " << code << '\n';
        return {};
    }

    // Continuă cu căutarea jucătorului
    const std::string findQuery = "SELECT playerName FROM GameData WHERE roomCode = '" + code + "';";
    sqlite3_stmt* stmt;
    std::optional<std::string> result;

    if (sqlite3_prepare_v2(db, findQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
    }
    else {
        std::cerr << "Eroare la căutarea codului: " << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(stmt);
    return result;
}


std::optional<std::string> DataBase::GetRecentPlayer() {
    const std::string getPlayerQuery = R"(
        SELECT playerName
        FROM GameData
        ORDER BY lastConnected DESC
        LIMIT 1;
    )";
    sqlite3_stmt* stmt;
    std::optional<std::string> result;

    if (sqlite3_prepare_v2(db, getPlayerQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
    }
    else {
        std::cerr << "Eroare la executarea interogării: " << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(stmt);
    return result;
}

void DataBase::updateLastConnected(const std::string& playerName) {

    const std::string updateQuery = R"(
        UPDATE GameData
        SET lastConnected = CURRENT_TIMESTAMP
        WHERE playerName = ')" + playerName + R"(';
    )";
    executeQuery(updateQuery);
}

sqlite3* DataBase::GetDatabaseConnection() const
{
    return db;
}

std::vector<std::string> DataBase::GetPlayersForRoom(const std::string& roomCode) {
    std::vector<std::string> players;
    const std::string query = "SELECT playerName FROM GameData WHERE roomCode = ?;";

    sqlite3_stmt* stmt = nullptr;

    // Pregătește interogarea SQL
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return players;
    }

    // Asociază parametrii interogării
    if (sqlite3_bind_text(stmt, 1, roomCode.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Error binding parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return players;
    }

    // Execută interogarea și extrage datele
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* playerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (playerName) {
            players.emplace_back(playerName);
        }
    }

    // Verifică eventuale erori
    if (sqlite3_errcode(db) != SQLITE_DONE && sqlite3_errcode(db) != SQLITE_OK) {
        std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
    }

    // Curăță resursele
    sqlite3_finalize(stmt);
    return players;
}

std::optional<std::tuple<std::string, int>> DataBase::GetPlayerDataById(int playerId) const {
    const std::string selectQuery = "SELECT playerName, score FROM GameData WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, playerId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int score = sqlite3_column_int(stmt, 1);

            sqlite3_finalize(stmt);

            return std::make_tuple(name, score);
        }
    }
    else {
        std::cerr << "Eroare la executarea interogării: " << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<std::tuple<std::string, int, int, std::string>> DataBase::GetGameData() const {

    const std::string selectQuery = "SELECT playerName, score, level, roomCode FROM GameData;";
    sqlite3_stmt* stmt;
    std::vector<std::tuple<std::string, int, int, std::string>> results;

    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int score = sqlite3_column_int(stmt, 1);
            int level = sqlite3_column_int(stmt, 2);
            std::string roomCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            results.emplace_back(name, score, level, roomCode);
        }
    }
    else {
        std::cerr << "Eroare la executarea interogării: " << sqlite3_errmsg(db) << '\n';
    }
    sqlite3_finalize(stmt);
    return results;
}

void DataBase::executeQuery(const std::string& query) {

    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Eroare la executarea interogării: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

std::ostream& operator<<(std::ostream& out, const DataBase& db) {

    auto data = db.GetGameData();

    if (data.empty()) {
        out << "Database is empty.\n";
        return out;
    }

    out << "Player Name\tScore\tLevel\tRoom Code\n";
    out << "------------------------------------------\n";

    for (const auto& row : data) {
        const std::string& playerName = std::get<0>(row);
        int score = std::get<1>(row);
        int level = std::get<2>(row);
        const std::string& roomCode = std::get<3>(row);

        out << playerName << "\t\t" << score << "\t" << level << "\t" << roomCode << "\n";
    }

    return out;
}