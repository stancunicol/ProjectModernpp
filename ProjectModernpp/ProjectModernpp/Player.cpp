#include "Player.h"
#include <cstdlib>
#include <random>

std::shared_ptr<std::array<std::pair<Point, bool>, 4>> Player::m_positions = nullptr;

Player::Player(const std::string& name, const GameMap& grid)
    : m_name{ name }, m_points{ 0 }, m_score{ 0 } {
    if (!m_positions) {
        m_positions = std::make_shared<std::array<std::pair<Point, bool>, 4>>();
        (*m_positions)[0] = { Point(0, 0), false };
        (*m_positions)[1] = { Point(0, grid.GetWidth() - 1), false };
        (*m_positions)[2] = { Point(grid.GetHeight() - 1, 0), false };
        (*m_positions)[3] = { Point(grid.GetHeight() - 1, grid.GetWidth() - 1), false };
    }
}

void Player::PlaceCharacter() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);

    bool found = false;

    do {
        size_t randomIndex = dis(gen);

        if (!(*m_positions)[randomIndex].second) {
            m_position = (*m_positions)[randomIndex].first;

            (*m_positions)[randomIndex].second = true;

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
