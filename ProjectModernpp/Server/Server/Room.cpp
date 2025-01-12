#include "Room.h"
#include <iostream>

Room::Room(const std::string& code)
	: m_code{ code }, m_state{ RoomState::WAITING_FOR_PLAYERS }, m_lastActivityTime{ std::chrono::steady_clock::now() } {}

bool Room::IsFull() const
{
	return m_players.size() >= m_capacity;
}

bool Room::AddPlayer(const std::string& playerName)
{
	if (IsFull() || m_players.count(playerName))
		return false;
	m_players.insert(playerName);

	if (m_players.size() == m_capacity) {
		m_state = RoomState::IN_PROGRESS;
	}

	UpdateActivityTime();
	return true;
}

bool Room::RemovePlayer(const std::string& playerName)
{
	bool removed = m_players.erase(playerName) > 0;

	if (removed) {
		UpdateActivityTime();
	}

	if (m_players.empty()) {
		m_state = RoomState::INACTIVE;
	}

	return removed;
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

void Room::ClearPlayers() {
	m_players.clear();
	m_state = RoomState::INACTIVE;
	UpdateActivityTime();
}

void Room::SetState(RoomState state) {
	m_state = state;
}

Room::RoomState Room::GetState() const {
	return m_state;
}

void Room::RemoveRoom() {
	m_players.clear();
	m_state = RoomState::INACTIVE;
	std::cout << "Room " << m_code << " has been removed." << std::endl;
}

bool Room::IsInactive() const {
	return m_state == RoomState::INACTIVE;
}

void Room::UpdateActivityTime() {
	m_lastActivityTime = std::chrono::steady_clock::now();
}

bool Room::HasTimedOut() const {
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<double> diff = now - m_lastActivityTime;

	return diff.count() > 600;
}