#include "GameMapInterface.h"
#include "ServerUtils.h"
#include<QTimer>
#include<QPainter>
#include <QKeyEvent>
#include <QDebug>

GameMapInterface::GameMapInterface(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Game Map");
    setFixedSize(600, 600);
    
    
    
    std::pair<int, int> basePosition = m_serverObject.GetBaseFromServer();
    qDebug() << "Base Position - X:" << basePosition.first << " Y:" << basePosition.second;

    std::vector<Bomb> bombs = m_serverObject.GetBombsFromServer();
    if (!bombs.empty()) {
        qDebug() << "Received " << bombs.size() << " bombs from server.";
        for (const auto& bomb : bombs) {
            qDebug() << "Bomb ID: " << bomb.id << " Position: (" << bomb.x << ", " << bomb.y << ")";
        }
    }
    else {
        qDebug() << "No bomb data available.";
    }
    std::vector<Enemy> enemies = m_serverObject.GetEnemiesFromServer();
    qDebug() << "Enemy Positions:";
    for (const auto& enemy : enemies) {
        qDebug() << "Enemy ID:" << enemy.id << "Position: (" << enemy.x << "," << enemy.y << ")";
    }
}

GameMapInterface::~GameMapInterface() {

}

void GameMapInterface::keyPressEvent(QKeyEvent* event)
{
    m_serverObject.GetMapFromServer();
    std::vector<std::vector<int>> matrix = m_serverObject.GetMap();
    std::cout << "Matrix:" << '\n';
    for (const auto& row : matrix)
    {
        for (const auto& cell : row)
        {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }


    if (event->key() == Qt::Key_W)
    {
        qDebug() << "W key pressed - Moving Up";
        m_serverObject.SendMoveToServer("up");
    }
    else if (event->key() == Qt::Key_S)
    {
        qDebug() << "S key pressed - Moving Down";
        m_serverObject.SendMoveToServer("down");
    }
    else if (event->key() == Qt::Key_A)
    {
        qDebug() << "A key pressed - Moving Left";
        m_serverObject.SendMoveToServer("left");
    }
    else if (event->key() == Qt::Key_D)
    {
        qDebug() << "D key pressed - Moving Right";
        m_serverObject.SendMoveToServer("right");
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

        
    

