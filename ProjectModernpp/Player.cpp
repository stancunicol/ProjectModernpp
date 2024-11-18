﻿#include "Player.h"

Player::Player(const std::string& name, GameMap& grid)
	: m_name(name), m_grid(grid) {}

void Player::PlaceCharacter(){
    uint16_t startX, startY;
    do {
        startX = rand() % m_grid.GetHeight();
        startY = rand() % m_grid.GetWidth();
    } while (m_grid.GetMap()[startX][startY] != CellType::EMPTY); // Verifică dacă locul este liber

    m_position = Point(startX, startY);
    m_grid.GetMap()[startX][startY] = CellType::PLAYER; // Plasează jucătorul pe hartă
}

void Player::MoveCharacter(const Point& direction){
    Point newPos = m_position + direction;
    if (newPos.GetX() >= 0 && newPos.GetX() < m_grid.GetHeight() &&
        newPos.GetY() >= 0 && newPos.GetY() < m_grid.GetWidth() &&
        m_grid.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {
        // Golire poziție veche
        auto& map = m_grid.GetMap();
        map[m_position.GetX() - direction.GetX()][m_position.GetY() - direction.GetY()] = CellType::EMPTY;

        // Setează noua poziție
        m_position = newPos;
        map[newPos.GetX()][newPos.GetY()] = CellType::PLAYER; // Plasează jucătorul pe noua poziție
    }
}