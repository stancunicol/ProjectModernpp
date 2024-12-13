#include "Player.h"
#include <cstdlib>
#include <random>

Player::Player(const std::string& name, const GameMap& grid)
    : m_name{ name }, m_points{ 0 }, m_score{ 0 } {
    m_positions.push_back({ Point(0, 0), false });                                 // corner letf-up
    m_positions.push_back({ Point(0, grid.GetWidth() - 1), false });               // corner right-up
    m_positions.push_back({ Point(grid.GetHeight() - 1, 0), false });             // corner left-downs
    m_positions.push_back({ Point(grid.GetHeight() - 1, grid.GetWidth() - 1), false }); // corner right-down
}

void Player::PlaceCharacter() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);

    bool found = false;

    do {
        size_t randomIndex = dis(gen);
        Point potentialPosition = m_positions[randomIndex].first;

        if (m_occupiedPositions.find(potentialPosition) == m_occupiedPositions.end()) {
            m_position = potentialPosition;
            m_occupiedPositions.insert(m_position);
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
