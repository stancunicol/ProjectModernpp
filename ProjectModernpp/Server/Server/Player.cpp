#include "Player.h"
#include <cstdlib>
#include <string>
#include <random>

std::shared_ptr<std::array<std::pair<Point, bool>, 4>> Player::m_positions = nullptr;

Player::Player() : m_name("DefaultPlayer"), m_points(0), m_score(0), m_moveDirection(0, 0), m_shootDirection(0, 0), m_isActive(true) { }

Player::Player(const std::string& name, const GameMap& grid)
    : m_name{ name }, m_points{ 0 }, m_score{ 0 }, m_shootDirection(0, 0), m_moveDirection(0, 0)
{
    if (!m_positions) {
        ResetPositions(grid);
    }
    PlaceCharacter();
    std::cout << "[INFO] Player created: " << name << " at position ("
        << m_position.GetX() << ", " << m_position.GetY() << ").\n";
}


void Player::PlaceCharacter() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, m_positions->size() - 1);

    bool found = false;

    do {
        size_t randomIndex = dis(gen);

        if (!(*m_positions)[randomIndex].second) {
            m_position = (*m_positions)[randomIndex].first;
            (*m_positions)[randomIndex].second = true;
            found = true;
            //std::cout << "[DEBUG] Player placed at position: (" << std::to_string(m_position.GetX()) << ", " << std::to_string(m_position.GetY()) << ").\n";
        }
    } while (!found);
}


void Player::MoveCharacter(const Point& direction, GameMap& grid) {
    Point newPos = m_position + direction;

    if (newPos.GetX() >= 0 && newPos.GetX() < grid.GetHeight() &&
        newPos.GetY() >= 0 && newPos.GetY() < grid.GetWidth() &&
        grid.GetMap()[newPos.GetX()][newPos.GetY()] == CellType::EMPTY) {

        m_position = newPos;
        //std::cout << "[DEBUG] Player moved to position: (" << std::to_string(newPos.GetX()) << ", " << std::to_string(newPos.GetY()) << ").\n";
    }

    if (direction != Point(0, 0)) {
        m_shootDirection = direction;
    }
    m_direction = Point(0, 0);
}

const Point& Player::GetShootDirection() const {

    return m_shootDirection;
}

void Player::ResetPositions(const GameMap& grid)
{
    //std::cout << "[DEBUG] Resetting player positions..." << std::endl;
    m_positions.reset();

    m_positions = std::make_shared<std::array<std::pair<Point, bool>, 4>>();
    (*m_positions)[0] = { Point(0, 0), false };
    (*m_positions)[1] = { Point(0, grid.GetWidth() - 1), false };
    (*m_positions)[2] = { Point(grid.GetHeight() - 1, 0), false };
    (*m_positions)[3] = { Point(grid.GetHeight() - 1, grid.GetWidth() - 1), false };
}

std::string Player::GetName() const
{
    return m_name;
}

uint16_t Player::GetPoints() const
{
    return m_points;
}

uint16_t Player::GetScore() const
{
    return m_score;
}

void Player::SetShootDirection(const Point& direction)
{
    m_shootDirection = direction;
}

void Player::SetPoints(uint16_t points)
{
    m_points += points;
}

void Player::SetScore(int8_t points)
{
    m_score += points;
}
