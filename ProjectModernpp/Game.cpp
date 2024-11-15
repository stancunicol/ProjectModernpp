#include "Game.h"

void Game::movePlayer(char direction)
{
    // Get the current position of the player
    Point currentPos = player.GetPosition();
    int newX = currentPos.m_x;
    int newY = currentPos.m_y;

    // Determine the new coordinates based on the direction
    switch (direction) {
    case 'w': newX -= 1; break; // Up
    case 's': newX += 1; break; // Down
    case 'a': newY -= 1; break; // Left
    case 'd': newY += 1; break; // Right
    default: return; // Invalid direction, exit the function
    }

    // Check if the position is free
    if (map.isPositionFree(newX, newY)) {
        // Update the map: mark the current cell as empty
        map.GetMap()[currentPos.m_x][currentPos.m_y] = CellType::EMPTY;
        // Move the player to the new coordinates
        player.SetPosition(newX, newY);

        // Mark the new position as the player's
        map.GetMap()[newX][newY] = CellType::PLAYER;
    }

    // Display the updated map
    map.display();
}
