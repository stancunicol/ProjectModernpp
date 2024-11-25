#include "Game.h"
#include "Enemy.h"
#include <Windows.h>

Game::Game(uint32_t width, uint32_t height, uint8_t level)
    : m_map(width, height, level), m_player("Player", m_map) {
}

Game::~Game() {
    for (auto* enemy : m_enemies)
        delete enemy;
}

void Game::InitializeGame() {
    srand(time(NULL));
    Base base(m_map);
    base.PlaceBase();
    
    for (int i = 0; i < 3; ++i) {
        m_enemies.push_back(new Enemy(*this, m_map));
        m_enemies.back()->PlaceCharacter();
    }
    //3 playeri ce nu se misca
    m_players.push_back(new Player("Johnny", m_map));
    m_players.back()->PlaceCharacter();
    m_players.push_back(new Player("John", m_map));
    m_players.back()->PlaceCharacter();
    m_players.push_back(new Player("David", m_map));
    m_players.back()->PlaceCharacter();
    m_players.push_back(new Player("Me", m_map));
    m_players.back()->PlaceCharacter();

    
}

const std::vector<Enemy*> Game::GetEnemies() const {
    return m_enemies;
}

void Game::MovePlayer(const Point& direction) {
    for (int i = 0; i < 3; ++i) {
        m_players[i]->MoveCharacter(Point(0, 0));
    }
    m_players[4]->MoveCharacter(direction);
}

void Game::Run() {
    while (true) {
        system("CLS"); // Curățăm consola

        // Actualizăm jocul
        if (_kbhit()) {
            char input = _getch();
            Point direction;
            switch (input) {
            case 'w': case 'W': direction = Point(-1, 0); break;
            case 'a': case 'A': direction = Point(0, -1); break;
            case 's': case 'S': direction = Point(1, 0); break;
            case 'd': case 'D': direction = Point(0, 1); break;
            default: break;
            }
            MovePlayer(direction); // Mutăm jucătorul
        }


        for (auto* enemy : m_enemies) {
            enemy->MoveRandom();
        }
        //UpdateBullets(m_bullets, m_map);

        // Renderizăm harta
        m_map.Display();

        Sleep(200);

    }

}

void Game::AddBullet(const Bullet& bullet) {
    m_bullets.emplace_back(bullet);
}

void Game::UpdateBullets(std::vector<Bullet>& bullets, GameMap& map) {
    for (auto& bullet : bullets) {
        if (bullet.IsActive())
            bullet.Move(map);
        else {
            Point position = bullet.GetPosition();
            if (position.GetX() >= 0 && position.GetX() < map.GetHeight() &&
                position.GetY() >= 0 && position.GetY() < map.GetWidth()) {
                if (map.GetMap()[position.GetX()][position.GetY()] == CellType::BULLET) {
                    map.GetMap()[position.GetX()][position.GetY()] = CellType::EMPTY;
                }
            }
        }
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& bullet) { return !bullet.IsActive(); }), bullets.end());
}
