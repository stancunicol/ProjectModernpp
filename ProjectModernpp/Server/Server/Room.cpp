#include "Room.h"

Room::Room(const std::string& code, std::unique_ptr<GameMap> map)
	: m_code{ code },
	m_map{ std::move(map) },
	m_state{ RoomState::WAITING_FOR_PLAYERS },
	m_lastActivityTime{ std::chrono::steady_clock::now() } {
}

bool Room::IsFull() const
{
	return m_players.size() >= m_capacity;
}

bool Room::AddPlayer(const int& playerId)
{
	if (IsFull() || m_players.count(playerId))
		return false;
	m_players.insert(playerId);
	if (m_players.size() == m_capacity) {
		m_state = RoomState::IN_PROGRESS;
	}
	UpdateActivityTime();
	return true;
}

bool Room::RemovePlayer(const int& playerId)
{
	bool removed = m_players.erase(playerId) > 0;
	if (removed) {
		UpdateActivityTime();
	}
	if (m_players.empty()) {
		m_state = RoomState::INACTIVE;
	}
	return removed;
}

bool Room::HasPlayer(const int& playerId) const
{
	return m_players.count(playerId) > 0;
}

const std::string& Room::GetCode() const
{
	return m_code;
}

const std::unordered_set<int>& Room::GetPlayers() const
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

bool Room::IsInactive() const {
	return m_state == RoomState::INACTIVE;
}

void Room::UpdateActivityTime() {
	m_lastActivityTime = std::chrono::steady_clock::now();
}

bool Room::HasTimedOut() const {
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<double> diff = now - m_lastActivityTime;

	return diff.count() > ROOM_TIMEOUT;
}