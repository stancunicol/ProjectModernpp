#include "Player.h"
#include <fstream>

Player::Player(const std::string& name, GameMap& grid)
    : m_name(name), m_grid(grid) {
    m_positions.push_back({ Point(0, 0), false });                                 // Colțul stânga-sus
    m_positions.push_back({ Point(0, grid.GetWidth() - 1), false });               // Colțul dreapta-sus
    m_positions.push_back({ Point(grid.GetHeight() - 1, 0), false });             // Colțul stânga-jos
    m_positions.push_back({ Point(grid.GetHeight() - 1, grid.GetWidth() - 1), false }); // Colțul dreapta-jos
}

void Player::PlaceCharacter() {
    bool gasit = false;

    // Bucla care continuă până când găsim o poziție liberă
    do {
        size_t randomIndex = rand() % 4; // Generăm un index aleatoriu între 0 și 3

        // Verificăm dacă poziția aleasă nu este ocupată
        if (!m_positions[randomIndex].second) {
            m_position = m_positions[randomIndex].first;  // Setăm poziția jucătorului
            m_positions[randomIndex].second = true;        // Marcăm poziția ca ocupată

            // Plasăm jucătorul pe hartă
            m_grid.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::PLAYER;


            gasit = true;  // Am găsit o poziție liberă și am plasat jucătorul
        }
    } while (!gasit);
}

void Player::MoveCharacter(const Point& direction) {
    Point newPos = m_position + direction;
    auto& map = m_grid.GetMap();
    if (newPos.GetX() >= 0 && newPos.GetX() < m_grid.GetHeight() &&
        newPos.GetY() >= 0 && newPos.GetY() < m_grid.GetWidth() &&
        map[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {

        map[m_position.GetX()][m_position.GetY()] = CellType::EMPTY;

        m_position = newPos;

        map[newPos.GetX()][newPos.GetY()] = CellType::PLAYER;

        m_direction = direction;
    }
    else {
        m_direction = Point(0, 0);
    }
}