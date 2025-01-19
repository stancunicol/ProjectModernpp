#include "RoomManager.h"

RoomManager::~RoomManager() 
{
    for (auto& [code, room] : m_rooms) 
    {
        delete room;
    }
    m_rooms.clear();
}

bool RoomManager::CreateRoom(const std::string& code, std::unique_ptr<GameMap>&& map) 
{
    if (RoomExists(code)) 
    {
        std::cerr << "Room with code " << code << "already exists.\n";
        return false;
    }
    m_rooms[code] = new Room(code, std::move(map));
    return true;
}

bool RoomManager::RemoveRoom(const std::string& code) 
{
    auto it = m_rooms.find(code);
    if (it == m_rooms.end()) 
    {
        std::cerr << "Room with code " << code << " does not exists.\n";
        return false;
    }
    delete it->second;
    m_rooms.erase(it);

    return true;
}

bool RoomManager::RoomExists(const std::string& code) 
{
    return m_rooms.find(code) != m_rooms.end();
}

Room* RoomManager::GetRoom(const std::string& code) 
{
    auto it = m_rooms.find(code);
    if (it == m_rooms.end()) 
    {
        return nullptr;
    }
    return it->second;
}

void RoomManager::CleanupInactiveRooms() 
{
    for (auto it = m_rooms.begin(); it != m_rooms.end(); ) 
    {
        if (it->second->HasTimedOut()) 
        {
            delete it->second;
            it = m_rooms.erase(it);
        }
        else it++;
    }
}

std::vector<std::string> RoomManager::GetActiveRooms() const 
{
    std::vector<std::string> activeRooms;
    for (const auto& [code, room] : m_rooms) 
    {
        if (!room->IsInactive())
            activeRooms.emplace_back(code);
    }
    return activeRooms;
}

bool RoomManager::IsRoomAvailable(const std::string& code) 
{
    auto it = m_rooms.find(code);
    if (it == m_rooms.end())
        return false;

    auto& room = it->second;
    return room->GetPlayers().size() < room->GetCapacity();
}
