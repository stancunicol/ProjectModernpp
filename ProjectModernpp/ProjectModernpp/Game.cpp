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

    m_player.PlaceCharacter();
}

const std::vector<Enemy*> Game::GetEnemies() const {
    return m_enemies;
}

void Game::MovePlayer(const Point& direction) {
    for (int i = 0; i < 3; ++i) {
        m_players[i]->MoveCharacter(Point(0, 0));
    }
    m_player.MoveCharacter(direction);
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

        m_player.Shot();
        UpdateBullets(m_bullets, m_map);
        for (auto* enemy : m_enemies) {
            enemy->MoveRandom();
        }
        UpdateBullets(m_bullets, m_map);

        // Renderizăm harta
        m_map.Display();

        Sleep(1000);

    }

}

void Game::AddBullet(const Bullet& bullet) {
    m_bullets.emplace_back(bullet);
}

void Game::UpdateBullets(std::vector<Bullet>& bullets, GameMap& map) {
    for (auto& bullet : m_bullets) {
        if (!bullet.IsActive()) continue;

        // Calculăm noua poziție
        Point newPos = bullet.GetPosition() + bullet.GetDirection();

        // Verificăm limitele hărții
        if (newPos.GetX() < 0 || newPos.GetX() >= map.GetHeight() ||
            newPos.GetY() < 0 || newPos.GetY() >= map.GetWidth()) {
            // Curățăm poziția anterioară a glonțului
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;

            bullet.SetActive(false); // Glonțul devine inactiv
            continue;
        }

        // Gestionăm coliziuni
        switch (map.GetMap()[newPos.GetX()][newPos.GetY()]) {
        case CellType::PLAYER:
            // Jucător lovit
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;
            bullet.SetActive(false);
            break;

        case CellType::BREAKABLE_WALL:
            map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::EMPTY;
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;
            bullet.SetActive(false);
            break;

        case CellType::UNBREAKABLE_WALL:
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;
            bullet.SetActive(false);
            break;

        case CellType::BULLET: // Dacă se întâlnesc două gloanțe
        case CellType::ENEMY: // Poți decide cum să tratezi coliziunea
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;
            bullet.SetActive(false);
            break;

        default:
            // Mutăm glonțul
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;
            bullet.SetPosition(newPos);
            map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::BULLET;
            break;
        }

        // Eliminăm gloanțele inactive
        m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullet& b) { return !b.IsActive(); }), m_bullets.end());

    }
}
