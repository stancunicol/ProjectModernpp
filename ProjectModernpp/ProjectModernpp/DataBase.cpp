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
            level INTEGER
        );
    )";
    executeQuery(createTableQuery);
}

void DataBase::InsertGameData(const std::string& playerName, uint32_t score, uint8_t level)
{
    const std::string insertQuery =
        "INSERT INTO GameData (playerName, score, level) "
        "VALUES ('" + playerName + "', " + std::to_string(score) + ", " + std::to_string(level) + ") "
        "ON CONFLICT(playerName) DO UPDATE SET "
        "score = 0, "
        "level = " + std::to_string(level) + ";";
    executeQuery(insertQuery);
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

bool DataBase::UserExists(const std::string& username)
{
    const std::string checkQuery = "SELECT COUNT(*) FROM GameData WHERE playerName = '" + username + "';";
    sqlite3_stmt* stmt;
    bool exists = false;

    if (sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(stmt, 0) > 0;
        }
    }
    else {
        std::cerr << "Eroare la executarea interogării: " << sqlite3_errmsg(db) << '\n';
    }
    sqlite3_finalize(stmt);
    return exists;
}

void DataBase::AddUser(const std::string& username)
{
    const std::string insertQuery = "INSERT INTO GameData (playerName, score, level) "
        "VALUES ('" + username + "', 0, 0);";
    executeQuery(insertQuery);
}


std::vector<std::tuple<std::string, int, int>> DataBase::GetGameData() const {

    const std::string selectQuery = "SELECT playerName, score, level FROM GameData;";
    sqlite3_stmt* stmt;
    std::vector<std::tuple<std::string, int, int>> results;

    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int score = sqlite3_column_int(stmt, 1);
            int level = sqlite3_column_int(stmt, 2);
            results.emplace_back(name, score, level);
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

    out << "Player Name\tScore\tLevel\n";
    out << "----------------------------------\n";

    for (const auto& row : data) {
        const std::string& playerName = std::get<0>(row);
        int score = std::get<1>(row);
        int level = std::get<2>(row);

        out << playerName << "\t\t" << score << "\t" << level << "\n";
    }

    return out;
}