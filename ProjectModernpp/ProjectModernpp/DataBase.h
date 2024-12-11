#pragma once
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>


class DataBase {
private:
	sqlite3* db;

	void executeQuery(const std::string& query);
public:
	DataBase(const std::string& dbName);
	~DataBase();

	void Initialize();

	void InsertGameData(const std::string& playerName, uint32_t score, uint8_t level);
	void UpdateGameData(const std::string& playerName, uint32_t score);
	void DeleteGameData();

	bool UserExists(const std::string& username);
	void AddUser(const std::string& username);

	std::vector<std::tuple<std::string, int, int>> GetGameData() const;

	friend std::ostream& operator<<(std::ostream& out, const DataBase& db);
};

