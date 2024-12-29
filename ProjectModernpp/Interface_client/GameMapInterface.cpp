#include "GameMapInterface.h"
#include "ServerUtils.h"
#include <QKeyEvent>
#include <QDebug>

GameMapInterface::GameMapInterface(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Game Map");
    setFixedSize(600, 600);
}

GameMapInterface::~GameMapInterface()
{

}


void GameMapInterface::keyPressEvent(QKeyEvent* event)
{
    std::pair<int, int> basePosition = GetBaseFromServer();
    qDebug() << "Base Position - X:" << basePosition.first << " Y:" << basePosition.second;

    std::vector<Bomb> bombs = GetBombsFromServer();
    if (!bombs.empty()) {
        qDebug() << "Received " << bombs.size() << " bombs from server.";
        for (const auto& bomb : bombs) {
            qDebug() << "Bomb ID: " << bomb.id << " Position: (" << bomb.x << ", " << bomb.y << ")";
        }
    }
    else {
        qDebug() << "No bomb data available.";
    }

    std::vector<Enemy> enemies = GetEnemiesFromServer();
    qDebug() << "Enemy Positions:";
    for (const auto& enemy : enemies) {
        qDebug() << "Enemy ID:" << enemy.id << "Position: (" << enemy.x << "," << enemy.y << ")";
    }

    if (event->key() == Qt::Key_W)
    {
        qDebug() << "W key pressed - Moving Up";
        SendMoveToServer(1, "up");
    }
    else if (event->key() == Qt::Key_S)
    {
        qDebug() << "S key pressed - Moving Down";
        SendMoveToServer(1, "down");
    }
    else if (event->key() == Qt::Key_A)
    {
        qDebug() << "A key pressed - Moving Left";
        SendMoveToServer(1, "left");
    }
    else if (event->key() == Qt::Key_D)
    {
        qDebug() << "D key pressed - Moving Right";
        SendMoveToServer(1, "right");
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

