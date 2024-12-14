#include "Room.h"

Room::Room(std::string code, uint8_t maxCapacity)
	: m_code{ code }, m_capacity{ maxCapacity } {}

bool Room::IsFull() const
{
	return m_players.size() >= m_capacity;
}

bool Room::AddPlayer(const std::string& playerName)
{
	if (IsFull() || m_players.count(playerName))
		return false;
	m_players.insert(playerName);
	return true;
}

bool Room::RemovePlayer(const std::string& playerName)
{
	return m_players.erase(playerName) > 0;
}

bool Room::HasPlayer(const std::string& playerName) const
{
	return m_players.count(playerName) > 0;
}
