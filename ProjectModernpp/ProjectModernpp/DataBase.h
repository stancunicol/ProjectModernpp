#pragma once
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <optional>


class DataBase{
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

	void UpdateLevel(const std::string& playerName, int level);

	void InsertRoomCode(const std::string& playerName, const std::string& roomCode);
	std::optional<std::string> FindRoomByCode(const std::string& code);

	std::optional<std::string> GetRecentPlayer();
	void updateLastConnected(const std::string& playerName);


	std::vector<std::tuple<std::string, int, int, std::string>> GetGameData() const;

	friend std::ostream& operator<<(std::ostream& out, const DataBase& db);
};

