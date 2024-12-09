#include "Game.h"
#include <Windows.h>

Game::Game(uint32_t width, uint32_t height, uint8_t level)
    : m_map(width, height, level) {}

Game::~Game() {}

void Game::InitializeGame() { //here, we initialize the game
    srand(time(NULL)); // Seed pentru generarea pozițiilor aleatoare

    Base base(m_map);
    base.PlaceBase();

    m_entityManager.AddPlayer(Player("Player1", m_map));

    // TODO: Multiplayer; playerii se spameaza pe aceeasi pozitie

    //m_entityManager.AddPlayer(Player("Player2", m_map));
    //m_entityManager.AddPlayer(Player("Player3", m_map));
    //m_entityManager.AddPlayer(Player("Player4", m_map));

    for (int i = 0; i < 3; ++i) {
        Enemy enemy(m_map);
        enemy.PlaceCharacter(m_map);
        m_entityManager.AddEnemy(enemy);
    }

    int bombsToPlace = m_map.GetLevel();
    while (bombsToPlace > 0) {
        Point randomPos(rand() % m_map.GetHeight(), rand() % m_map.GetWidth());

        if (m_map.GetMap()[randomPos.GetX()][randomPos.GetY()] == CellType::UNBREAKABLE_WALL) {
            float randomActivationTime = static_cast<float>(rand() % 10 + 5); // 10-15 s
            m_entityManager.AddBomb(Bomb(randomPos, randomActivationTime));
            bombsToPlace--;
        }
    }
}

void Game::Run() {
    static float elapsedTime = 0.0f;
    const float shootInterval = 0.3f;
    static float enemyShootTimer = 0.0f;

    while (true) {
        system("CLS");

        if (_kbhit()) {
            char input = _getch();
            Point direction;
            switch (input) {
            case 'w': case 'W': direction = Point(-1, 0); break;
            case 'a': case 'A': direction = Point(0, -1); break;
            case 's': case 'S': direction = Point(1, 0); break;
            case 'd': case 'D': direction = Point(0, 1); break;
            case ' ':
                m_entityManager.PlayerShoot(m_map);
                break;
            default: break;
            }
            m_entityManager.GetPlayersMutable()[0].MoveCharacter(direction, m_map);

            //m_entityManager.GetPlayersMutable()[1].MoveCharacter(Point(0, 0), m_map);
            //m_entityManager.GetPlayersMutable()[2].MoveCharacter(Point(0, 0), m_map);
            //m_entityManager.GetPlayersMutable()[3].MoveCharacter(Point(0, 0), m_map);
        }

        // Actualizarea timerului pentru intervalul de tragere al inamicilor
        elapsedTime += 0.15f;

        if (elapsedTime >= shootInterval) {
            m_entityManager.UpdateEntities(m_map, elapsedTime);
            elapsedTime = 0.0f;
        }

        m_map.Display();

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

crow::json::wvalue Game::TranformInJson() {
    crow::json::wvalue jsonMap;
    for (size_t i = 0; i < m_map.GetMap().size(); i++) {
        for (size_t j = 0; j < m_map.GetMap()[i].size(); j++) {
            jsonMap["matrix"][i][j] = static_cast<int>(m_map.GetMap()[i][j]);
        }

    }
    jsonMap["level"] = m_map.GetLevel();
    jsonMap["rows"] = m_map.GetHeight();
    jsonMap["columns"] = m_map.GetWidth();

    return jsonMap;
}

crow::json::wvalue Game::GetGameStateAsJson() {
    return TranformInJson();
}
