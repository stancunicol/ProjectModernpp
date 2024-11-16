#include "GameMap.h"
#include "Enemy.h"

GameMap::GameMap()
    : m_width(0), m_height(0), m_level(0)
{
}

GameMap::GameMap(uint32_t width, uint32_t height, uint8_t level)
    : m_width(width),
    m_height(height),
    m_level(level)
{
    // Initialize the grid with empty cells
    m_grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    Initialize();  // Call the initialize function to set up the map based on difficulty
}

GameMap::~GameMap() {
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
}

void GameMap::Initialize() {

    srand(static_cast<unsigned>(time(0)));

    int breakableWallChance;
    int unbreakableWallChance;
    // Set wall chances based on difficulty level
    switch (m_level) {
    case 1: // easy
        breakableWallChance = 10;   // 10% chance for breakable walls
        unbreakableWallChance = 5;  // 5% chance for unbreakable walls
        break;
    case 2: // medium
        breakableWallChance = 20;   // 20% chance for breakable walls
        unbreakableWallChance = 10; // 10% chance for unbreakable walls
        break;
    case 3: // hard
        breakableWallChance = 30;   // 30% chance for breakable walls
        unbreakableWallChance = 15; // 15% chance for unbreakable walls
        break;
    default:
        break;
    }

    // Generate walls structured in clusters
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (m_grid[i][j] != CellType::EMPTY) continue; // Skip if the cell is already occupied

            int randVal = rand() % 100; // Generate a random number between 0 and 99
            if (randVal < breakableWallChance) {
                // Create a cluster for breakable walls
                CreateWallCluster(i, j, CellType::BREAKABLE_WALL);
            }
            else if (randVal < breakableWallChance + unbreakableWallChance) {
                // Create a cluster for unbreakable walls
                CreateWallCluster(i, j, CellType::UNBREAKABLE_WALL);
            }
            // Otherwise, leave the cell empty (default)
        }
    }
}

void GameMap::CreateWallCluster(int startX, int startY, CellType wallType) {
    int clusterSize = rand() % 3 + 1; // Cluster size: between 1 and 3 cells

    for (int i = 0; i < clusterSize; ++i) {
        for (int j = 0; j < clusterSize; ++j) {
            int x = startX + i;
            int y = startY + j;

            // Check if the new position is within bounds and if the cell is empty
            if (x < m_height && y < m_width && m_grid[x][y] == CellType::EMPTY) {
                m_grid[x][y] = wallType; // Set the cell type to the specified wall type
            }
        }
    }
}

// Function to check if a position is free
bool GameMap::IsPositionFree(int x, int y) {
    return x >= 0 && x < m_height && y >= 0 && y < m_width && m_grid[x][y] == CellType::EMPTY;
}

void GameMap::PlacePlayer(Player* p) {
    int startX, startY;
    do {
        // Generate random positions between 0 and m_height/m_width - 1
        startX = rand() % m_height;
        startY = rand() % m_width;
    } while (m_grid[startX][startY] != CellType::EMPTY); // Look for an EMPTY cell

    // Place the player at the selected position
    player = p;

    /*
    player->MoveCharacter();  // Dacă ai nevoie de această funcție pentru mișcare, las-o
    */
    uint8_t sX = static_cast<uint8_t>(startX);
    uint8_t sY = static_cast<uint8_t>(startY);
    player->SetPosition({sX, sY});
    // Mark the cell on the map as occupied by the player
    m_grid[sX][sY] = CellType::PLAYER;
}

// Function to display the map with the player
void GameMap::Display() const {
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            bool ok = false;
            if (player != nullptr && i == player->GetCoordonateX() && j == player->GetCoordonateY()) {
                std::cout << " P ";
                ok = true;
            }
            for (const auto& enemy : enemies) {
                if (i == enemy->GetCoordonateX() && j == enemy->GetCoordonateY()) {
                    std::cout << " I ";
                    ok = true;
                    break;
                }
            }
            if(!ok) 
            {
                switch (m_grid[i][j]) {
                case CellType::EMPTY: std::cout << " . "; break; // Empty cells represented by "."
                case CellType::BREAKABLE_WALL: std::cout << " * "; break; // Breakable walls represented by "*"
                case CellType::UNBREAKABLE_WALL: std::cout << " # "; break; // Unbreakable walls represented by "#"
                case CellType::PLAYER: std::cout << " P "; break; // Player
                }
            }
        }
        std::cout << '\n'; // New line after each row
    }
}

std::vector<std::vector<CellType>> GameMap::GetMap() const {
    return m_grid;
}

uint32_t GameMap::GetWidth() {
    return m_width;
}

uint32_t GameMap::GetHeight()
{
    return m_height;
}

void GameMap::AddEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

void GameMap::MoveEnemies(){
    for (Enemy* enemy : enemies)
        enemy->RandomMovement();
}
