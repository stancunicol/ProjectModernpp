#include "Player.h"
#include <cstdlib>

Player::Player(const std::string& name, GameMap& grid)
    : m_name(name), m_grid(grid) {
    m_positions.push_back({ Point(0, 0), false });                                 // corenr letf-up
    m_positions.push_back({ Point(0, grid.GetWidth() - 1), false });               // corner right-up
    m_positions.push_back({ Point(grid.GetHeight() - 1, 0), false });             // corner left-downs
    m_positions.push_back({ Point(grid.GetHeight() - 1, grid.GetWidth() - 1), false }); // corner right-down
}

void Player::PlaceCharacter() {
    bool gasit = false;

    do {
        size_t randomIndex = rand() % 4; // generate a random index between 0 and 3

        if (!m_positions[randomIndex].second) {
            m_position = m_positions[randomIndex].first;  // we set the position of the player
            m_positions[randomIndex].second = true;        //we set the position as marked
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

        map[m_position.GetX()][m_position.GetY()] = CellType::EMPTY; //we clean the previous position
        m_position = newPos;
        map[newPos.GetX()][newPos.GetY()] = CellType::PLAYER; //we update the new position
        m_direction = direction;
    }
    else {
        m_direction = Point(0, 0); //reset the direction if is not valid
    }
}

void Player::Shot() {
    if (m_direction.GetX() == 0 && m_direction.GetY() == 0) {
        m_direction = Point(0, 1); //to left, deafult
    }

    //we create the bullet and add it
    Bullet bullet(true, 1.0f, m_direction, m_position);

    //we add the bullet in the map
    m_grid.AddBullet(bullet);
}

Bullet Player::GetBullet() {
    return m_bullet;
}
