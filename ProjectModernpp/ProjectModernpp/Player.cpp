#include "Player.h"
#include <cstdlib>

Player::Player(const std::string& name, const GameMap& grid)
    : m_name{ name }, m_points{ 0 }, m_score{ 0 } {
    m_positions.push_back({ Point(0, 0), false });                                 // corner letf-up
    m_positions.push_back({ Point(0, grid.GetWidth() - 1), false });               // corner right-up
    m_positions.push_back({ Point(grid.GetHeight() - 1, 0), false });             // corner left-downs
    m_positions.push_back({ Point(grid.GetHeight() - 1, grid.GetWidth() - 1), false }); // corner right-down
}

void Player::PlaceCharacter() {
    srand(time(NULL));
    bool found = false;

    do {
        size_t randomIndex = rand() % 4; // generate a random index between 0 and 3

        if (!m_positions[randomIndex].second) {
            m_position = m_positions[randomIndex].first;  // we set the position of the player
            m_positions[randomIndex].second = true;        //we set the position as marked
            found = true;
        }
    } while (!found);

}

void Player::MoveCharacter(const Point& direction, GameMap& grid) {
    Point newPos = m_position + direction;

    if (newPos.GetX() >= 0 && newPos.GetX() < grid.GetHeight() &&
        newPos.GetY() >= 0 && newPos.GetY() < grid.GetWidth() &&
        grid.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {

        m_position = newPos;
    }

    if (direction != Point(0, 0)) {
        m_shootDirection = direction;
    }
    m_direction = Point(0, 0);
}

const Point& Player::GetShootDirection() const {

    return m_shootDirection;
}

std::string Player::GetName() const
{
    return m_name;
}

uint32_t Player::GetPoints() const
{
    return m_points;
}

uint32_t Player::GetScore() const
{
    return m_score;
}

void Player::SetShootDirection(const Point& direction)
{
    m_shootDirection = direction;
}

void Player::SetPoints(uint32_t points)
{
    m_points += points;
}

void Player::SetScore()
{
    m_score += m_points;
}
