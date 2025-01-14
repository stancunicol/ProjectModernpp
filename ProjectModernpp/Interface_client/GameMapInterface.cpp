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

    m_serverObject.GetMapFromServer();
    matrix = m_serverObject.GetMap();
    height = matrix.size();
    width = matrix[0].size();

    resize(36 * width, 36 * height);

    basePosition = m_serverObject.GetBaseFromServer();
    qDebug() << "Base Position - X:" << basePosition.first << " Y:" << basePosition.second;

    bombs = m_serverObject.GetBombsFromServer();
    if (!bombs.empty()) {
        qDebug() << "Received " << bombs.size() << " bombs from server.";
        for (const auto& bomb : bombs) {
            qDebug() << "Bomb ID: " << bomb.id << " Position: (" << bomb.x << ", " << bomb.y << ")";
        }
    }
    else {
        qDebug() << "No bomb data available.";
    }

    enemies = m_serverObject.GetEnemiesFromServer();
    qDebug() << "Enemy Positions:";
    for (const auto& enemy : enemies) {
        qDebug() << "Enemy ID:" << enemy.id << "Position: (" << enemy.x << "," << enemy.y << ")";
    }
    update();
}

GameMapInterface::~GameMapInterface() 
{

}

void GameMapInterface::keyPressEvent(QKeyEvent* event)
{
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

void GameMapInterface::paintEvent(QPaintEvent* event)
{
    QPixmap pixmap;
    QPainter painter(this);
    for(int i=0;i<height;i++)
        for (int j = 0; j < width; j++)
        {
            switch (matrix[i][j])
            {
            case 0:
                pixmap = QPixmap("./path.jpg").scaled(36, 36);
                break;
            case 1:
                pixmap = QPixmap("./bush.jpg").scaled(36, 36);
                break;
            case 2:
                pixmap = QPixmap("./wall.jpg").scaled(36, 36);
                break;
            }
            painter.drawPixmap(j * 36, i * 36, pixmap);
        }
    pixmap = QPixmap("./base.jpg").scaled(36, 36);
    painter.drawPixmap(basePosition.second * 36, basePosition.first * 36, pixmap);
    pixmap = QPixmap("./player1.png").scaled(36, 36);
    painter.drawPixmap(0, 0, pixmap);
    for (auto enemy : enemies)
    {
        if (enemy.id == 0)
        {
            pixmap = QPixmap("./player2.png").scaled(36, 36);
            painter.drawPixmap(enemy.y * 36, enemy.x * 36, pixmap);
        }
        else
            if (enemy.id == 1)
            {
                pixmap = QPixmap("./player3.png").scaled(36, 36);
                painter.drawPixmap(enemy.y * 36, enemy.x * 36, pixmap);
            }
            else
                if (enemy.id == 2)
                {
                    pixmap = QPixmap("./player4.png").scaled(36, 36);
                    painter.drawPixmap(enemy.y * 36, enemy.x * 36, pixmap);
                }
    }
}
