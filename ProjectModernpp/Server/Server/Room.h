#pragma once
#include <string>
#include <unordered_set>
#include <chrono>

class Room {
private:
	std::string m_code;
	static const uint8_t m_capacity = 4;
	std::unordered_set<std::string> m_players;

	enum class RoomState { WAITING_FOR_PLAYERS, IN_PROGRESS, FINISHED, INACTIVE };
	RoomState m_state;

	std::chrono::steady_clock::time_point m_lastActivityTime;

public:
	Room(const std::string& code);

	bool IsFull() const;

	bool AddPlayer(const std::string& playerName);

	bool RemovePlayer(const std::string& playerName);

	bool HasPlayer(const std::string& playerName) const;

	const std::string GetCode() const;

	const std::unordered_set<std::string> GetPlayers() const;

	const uint8_t GetCapacity() const;

	void ClearPlayers();

	void SetState(RoomState state);

	RoomState GetState() const;

	void RemoveRoom();

	bool IsInactive() const;

	void UpdateActivityTime();

	bool HasTimedOut() const;
};

