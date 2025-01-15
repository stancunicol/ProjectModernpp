#include "GameMapInterface.h"
#include "ServerUtils.h"

GameMapInterface::GameMapInterface(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Game Map");
    setFocusPolicy(Qt::StrongFocus);

    m_serverObject.GetMapFromServer();
    matrix = m_serverObject.GetMap();
    height = matrix.size();
    width = matrix[0].size();

    std::cout << "Matrix:" << '\n';
    for (const auto& row : matrix)
    {
        for (const auto& cell : row)
        {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

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

    m_serverObject.GetPlayerPositionsFromServer();
    playerPositions = m_serverObject.GetPlayerPositions();
    std::cout << "Player Positions:\n";
    for (const auto& position : playerPositions) {
        auto point = position.first;
        bool isActive = position.second;

        std::cout << "Player at (" << static_cast<int>(point.m_x) << ", " << static_cast<int>(point.m_y)
            << ") is " << (isActive ? "active" : "inactive") << ".\n";
    }
    loadControls();
    update();
}

GameMapInterface::~GameMapInterface()
{

}

void GameMapInterface::loadControls()
{
    QSettings settings("MyCompany", "BattleCity");

    m_upKey = settings.value("UpKey", "W").toString();
    m_downKey = settings.value("DownKey", "S").toString();
    m_leftKey = settings.value("LeftKey", "A").toString();
    m_rightKey = settings.value("RightKey", "D").toString();
    m_fireKey = settings.value("FireKey", "Space").toString();

}

void GameMapInterface::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        qDebug() << "Up Arrow pressed!";
        m_serverObject.SendMoveToServer("up");
        break;
    case Qt::Key_Down:
        qDebug() << "Down Arrow pressed!";
        m_serverObject.SendMoveToServer("down");
        break;
    case Qt::Key_Left:
        qDebug() << "Left Arrow pressed!";
        m_serverObject.SendMoveToServer("left");
        break;
    case Qt::Key_Right:
        qDebug() << "Right Arrow pressed!";
        m_serverObject.SendMoveToServer("right");
        break;
    }

    if (event->key() == QKeySequence(m_upKey).toString().at(0).unicode())
    {
        qDebug() << m_upKey << " key pressed - Moving Up";
        m_serverObject.SendMoveToServer("up");
    }
    else if (event->key() == QKeySequence(m_downKey).toString().at(0).unicode())
    {
        qDebug() << m_downKey << " key pressed - Moving Down";
        m_serverObject.SendMoveToServer("down");
    }
    else if (event->key() == QKeySequence(m_leftKey).toString().at(0).unicode())
    {
        qDebug() << m_leftKey << " key pressed - Moving Left";
        m_serverObject.SendMoveToServer("left");
    }
    else if (event->key() == QKeySequence(m_rightKey).toString().at(0).unicode())
    {
        qDebug() << m_rightKey << " key pressed - Moving Right";
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
    for (int i = 0; i < height; i++)
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
