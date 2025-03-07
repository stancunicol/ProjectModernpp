#pragma once
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <optional>
#include <mutex>

class DataBase 
{
private:
	sqlite3* m_db;

	std::mutex m_dbMutex;

	void executeQuery(const std::string& query);

public:
	DataBase(const std::string& dbName);

	~DataBase();

	void Initialize();

	void UpdateGameData(const std::string& playerName, uint16_t score);

	void DeleteGameData();


	bool UserExists(const std::string& username);
	
	void AddUser(const std::string& username);
	
	int GetUserId(const std::string& username);
	

	void UpdateLevel(const std::string& playerName, uint8_t level);
	
	void UpdateDataByRoomCode(uint8_t playerId, const std::string& code);


	void InsertRoomCode(const std::string& playerName, const std::string& roomCode);
	
	std::optional<std::string> FindRoomByCode(const std::string& code);

	
	std::optional<std::string> GetRecentPlayer();
	
	void UpdateLastConnected(const std::string& playerName);


	std::vector<std::string> GetPlayersFromRoom(const std::string& roomCode);
	
	std::vector<uint8_t> GetPlayersFromRoom(uint8_t playerId);


	std::optional<std::tuple<std::string, uint8_t>> GetPlayerDataById(uint8_t playerId) const;
	
	std::vector<std::tuple<std::string, uint16_t, uint8_t, std::string>> GetGameData() const;


	void DeleteRoom(const std::string& roomCode);
	
	void UpdateRoomData(const std::string& roomCode, const std::string& newMapData);


	void RemovePlayerFromRoom(int playerId);

	void AddRoom(const std::string& roomCode);
};
