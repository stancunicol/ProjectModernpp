#pragma once
#include <string>
#include <unordered_set>

class Room {
private:
	std::string m_code;
	static const uint8_t m_capacity = 4;
	std::unordered_set<std::string> m_players;

public:
	Room(const std::string& code);

	bool IsFull() const;

	bool AddPlayer(const std::string& playerName);

	bool RemovePlayer(const std::string& playerName);

	bool HasPlayer(const std::string& playerName) const;

	const std::string GetCode() const;

	const std::unordered_set<std::string> GetPlayers() const;

	const uint8_t GetCapacity() const;
};

