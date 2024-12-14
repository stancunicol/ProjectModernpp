#include "Room.h"

Room::Room(const std::string& code)
	: m_code{ code } {}

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

const std::string Room::GetCode() const
{
	return m_code;
}

const std::unordered_set<std::string> Room::GetPlayers() const
{
	return m_players;
}

const uint8_t Room::GetCapacity() const
{
	return m_capacity;
}
