#include "Game.h"
#include <Windows.h>

Game::Game()
    : m_map(), m_entityManager(m_map), m_setLevel(false) {
}


Game::~Game() {}

void Game::InitializeGame() { //here, we initialize the game
    srand(time(NULL)); // Seed pentru generarea pozițiilor aleatoare

    m_entityManager.PlaceBase(m_map);

    m_entityManager.AddPlayer(Player("Player1", m_map));

    // TODO: Multiplayer; playerii se spameaza pe aceeasi pozitie, posibila problema: srand

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

    // Așteaptă până când nivelul este setat
    {
        std::unique_lock<std::mutex> lock(mutex);
        condition.wait(lock, [this]() { return m_setLevel; });
    }

    while (m_entityManager.GetBase().GetLife()) {
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
    EndGame(m_entityManager.GetWinner());
}

void Game::EndGame(const std::string& winner)
{
    for (size_t i = 0; i < m_entityManager.GetPlayers().size(); i++) {
        std::cout << m_entityManager.GetPlayers()[i].GetName() << " score: " 
            << static_cast<int>(m_entityManager.GetPlayers()[i].GetScore()) << '\n';\
    }
    std::cout << "The game is over! " << winner << " WON!\n";
}

void Game::SetLevel(int newLevel) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        m_map.Reset(newLevel);
        m_setLevel = true;
    }
    // Reinitializam jocul
    InitializeGame();

    condition.notify_all(); // Notifică bucla principală că poate continua
    std::cout << "Game level updated to: " << newLevel << '\n';
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
