#pragma once
#include <string>
#include <unordered_set>

class Room {
public:
	std::string m_code;
	uint8_t m_capacity;
	std::unordered_set<std::string> m_players;

	Room(std::string code, uint8_t maxCapacity);

	bool IsFull() const;

	bool AddPlayer(const std::string& playerName);

	bool RemovePlayer(const std::string& playerName);

	bool HasPlayer(const std::string& playerName) const;
};

