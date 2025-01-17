#pragma once
#include <unordered_map>
#include <iostream>
#include <string>
#include "Room.h"
#include "GameMap.h"

class RoomManager {
private:
	std::unordered_map <std::string, Room*> m_rooms;

public:
	RoomManager() = default;

	~RoomManager();

	bool CreateRoom(const std::string& code, std::unique_ptr<GameMap>&& map);

	bool RemoveRoom(const std::string& code);

	bool RoomExists(const std::string& code);

	Room* GetRoom(const std::string& code);

	void CleanupInactiveRooms();

	std::vector<std::string> GetActiveRooms() const;

	bool IsRoomAvailable(const std::string& code);
};