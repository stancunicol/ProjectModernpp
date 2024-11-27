#include "Game.h"
#include "Enemy.h"
#include <Windows.h>

Game::Game(uint32_t width, uint32_t height, uint8_t level) //constructor
    : m_map(width, height, level), m_player("Player", m_map) {
}

Game::~Game() { //destructor (mainly for the enemies)
    for (auto* enemy : m_enemies)
        delete enemy;
}

void Game::InitializeGame() { //here, we initialize the game
    srand(time(NULL)); //makes use of the computer's internal clock to control the choice of the seed
    Base base(m_map); //we initialize the base
    base.PlaceBase(); //and place it on the map
    
    for (int i = 0; i < 3; ++i) { //we create the enemies and we put them on the map
        m_enemies.push_back(new Enemy(*this, m_map));
        m_enemies.back()->PlaceCharacter();
    }
    //3 players that don't move
    m_players.push_back(new Player("Johnny", m_map));
    m_players.back()->PlaceCharacter();
    m_players.push_back(new Player("John", m_map));
    m_players.back()->PlaceCharacter();
    m_players.push_back(new Player("David", m_map));
    m_players.back()->PlaceCharacter();

    m_player.PlaceCharacter(); //we place the players on the map
}

const std::vector<Enemy*> Game::GetEnemies() const { //returns the enemies
    return m_enemies;
}

void Game::MovePlayer(const Point& direction) { //it moves the player in the mentioned direction
    for (int i = 0; i < 3; ++i) {
        m_players[i]->MoveCharacter(Point(0, 0));
    }
    m_player.MoveCharacter(direction);
}

void Game::Run() { //running the game
    while (true) {
        system("CLS"); //we clean the console

        //updating the game
        if (_kbhit()) {
            char input = _getch(); //we "read" the character and we move the player based on it
            Point direction;
            switch (input) {
            case 'w': case 'W': direction = Point(-1, 0); break;
            case 'a': case 'A': direction = Point(0, -1); break;
            case 's': case 'S': direction = Point(1, 0); break;
            case 'd': case 'D': direction = Point(0, 1); break;
            default: break;
            }
            MovePlayer(direction); //we move the player
        }

        m_player.Shot(); //the player shoots
        UpdateBullets(m_bullets, m_map); //we update the bullets on the map
        for (auto* enemy : m_enemies) {
            enemy->MoveRandom(); //we move each enemy random on the map
        }
        UpdateBullets(m_bullets, m_map); //we update the bullets on the map

        //we renderizăm the map
        m_map.Display();

        Sleep(1000);

    }

}

void Game::AddBullet(const Bullet& bullet) { //we add the bullet to the vector of bullets
    m_bullets.emplace_back(bullet);
}

void Game::UpdateBullets(std::vector<Bullet>& bullets, GameMap& map) { //we update the bullets on the map
    for (auto& bullet : m_bullets) {
        if (!bullet.IsActive()) continue; //if the bullet is active

        //we calculate the new position of the bullet
        Point newPos = bullet.GetPosition() + bullet.GetDirection();

        //we check if it correctly placed
        if (newPos.GetX() < 0 || newPos.GetX() >= map.GetHeight() ||
            newPos.GetY() < 0 || newPos.GetY() >= map.GetWidth()) {
            //we clean the previous position of the bullet
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY;

            bullet.SetActive(false); //we desactivate the bullet
            continue;
        }

        //we check for collisions
        switch (map.GetMap()[newPos.GetX()][newPos.GetY()]) { //if the new/current position of the bullet is the same with
        case CellType::PLAYER:
            //if a player was hit
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY; //we delete the bullet
            bullet.SetActive(false); //and desactivate it
            break;

        case CellType::BREAKABLE_WALL:
            //if a breakable wall was hit
            map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::EMPTY; //we delete the wall
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY; //we delete the bullet
            bullet.SetActive(false); //we desactivate it
            break;

        case CellType::UNBREAKABLE_WALL:
            //if an unbreakable wall was hit
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY; //we delete the bullet
            bullet.SetActive(false); //we desactivate it
            break;

        case CellType::BULLET: //if two bullets meet
        case CellType::ENEMY: //if a bullet hits an enemy
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY; //we delete the bullet
            bullet.SetActive(false); //we desactivate it
            break;

        default:
            //we move the bullet
            map.GetMap()[bullet.GetPosition().GetX()][bullet.GetPosition().GetY()] = CellType::EMPTY; //we clean the previous position
            bullet.SetPosition(newPos); //we move it
            map.GetMap()[newPos.GetX()][newPos.GetY()] = CellType::BULLET; //and place it on the map
            break;
        }

        //we eliminate the inactive bullets from the vector
        m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullet& b) { return !b.IsActive(); }), m_bullets.end());

    }
}
