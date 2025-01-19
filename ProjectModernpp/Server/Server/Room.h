#pragma once
#include <string>
#include <unordered_set>
#include <chrono>
#include <iostream>
#include "GameMap.h"

class Room 
{
private:
	std::string m_code;

	static const uint8_t m_capacity = 4;
	
	std::unordered_set<int> m_players;
	
	std::unique_ptr<GameMap> m_map;

	enum class RoomState { WAITING_FOR_PLAYERS, IN_PROGRESS, FINISHED, INACTIVE };
	
	RoomState m_state;

	std::chrono::steady_clock::time_point m_lastActivityTime;
	
	static constexpr double ROOM_TIMEOUT = 600.0;

public:
	Room(const std::string& code, std::unique_ptr<GameMap> map);

	bool IsFull() const;

	bool AddPlayer(const int& playerName);

	bool RemovePlayer(const int& playerName);

	bool HasPlayer(const int& playerName) const;

	const std::string& GetCode() const;

	const std::unordered_set<int>& GetPlayers() const;

	const uint8_t GetCapacity() const;

	void ClearPlayers();

	void SetState(RoomState state);

	RoomState GetState() const;

	bool IsInactive() const;

	void UpdateActivityTime();

	bool HasTimedOut() const;
};