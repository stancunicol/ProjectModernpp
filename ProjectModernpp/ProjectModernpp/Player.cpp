#include "Player.h"
#include <cstdlib>

Player::Player(const std::string& name, GameMap& grid)
    : m_name(name), m_grid(grid) {
    m_positions.push_back({ Point(0, 0), false });                                 // Colțul stânga-sus
    m_positions.push_back({ Point(0, grid.GetWidth() - 1), false });               // Colțul dreapta-sus
    m_positions.push_back({ Point(grid.GetHeight() - 1, 0), false });             // Colțul stânga-jos
    m_positions.push_back({ Point(grid.GetHeight() - 1, grid.GetWidth() - 1), false }); // Colțul dreapta-jos
}

void Player::PlaceCharacter() {
    bool gasit = false;

    do {
        size_t randomIndex = rand() % 4; // Generăm un index aleatoriu între 0 și 3

        if (!m_positions[randomIndex].second) {
            m_position = m_positions[randomIndex].first;  // Setăm poziția jucătorului
            m_positions[randomIndex].second = true;        // Marcăm poziția ca ocupată
            m_grid.GetMap()[m_position.GetX()][m_position.GetY()] = CellType::PLAYER;
            gasit = true;
        }
    } while (!gasit);
}

void Player::MoveCharacter(const Point& direction) {
    Point newPos = m_position + direction;
    auto& map = m_grid.GetMap();

    if (newPos.GetX() >= 0 && newPos.GetX() < m_grid.GetHeight() &&
        newPos.GetY() >= 0 && newPos.GetY() < m_grid.GetWidth() &&
        map[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {

        map[m_position.GetX()][m_position.GetY()] = CellType::EMPTY; // Curățăm poziția anterioară
        m_position = newPos;
        map[newPos.GetX()][newPos.GetY()] = CellType::PLAYER; // Actualizăm noua poziție
        m_direction = direction;
    }
    else {
        m_direction = Point(0, 0); // Resetăm direcția dacă mutarea nu e validă
    }
}

void Player::Shot() {
    if (m_direction.GetX() == 0 && m_direction.GetY() == 0) {
        m_direction = Point(0, 1); // Spre dreapta, implicit
    }

    // Cream glonțul și îl adăugăm direct
    Bullet bullet(true, 1.0f, m_direction, m_position);

    // Adăugăm glonțul în hartă
    m_grid.AddBullet(bullet);
}

Bullet Player::GetBullet() {
    return m_bullet;
}
