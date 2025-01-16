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
    auto playerPositions = m_serverObject.GetPlayerPositions();

    bool playerFound = false;
    for (const auto& position : playerPositions) {
        if (std::stoi(position.second) == m_serverObject.GetUserId()) {
            player1Position = QPoint(position.first.m_y, position.first.m_x);
            qDebug() << "[INFO] Player initialized at position: (" << player1Position.x() << ", " << player1Position.y() << ")";
            playerFound = true;
            break;
        }
    }

    if (!playerFound) {
        qWarning() << "[WARNING] Player position not found. Ensure server returned correct data.";
        player1Position = QPoint(-1, -1); // Setăm o poziție invalidă dacă nu este găsită
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
    QPoint direction;
    QString dirString;

    switch (event->key()) {
    case Qt::Key_Up:
        direction = QPoint(0, -1);  // Mișcare în sus
        dirString = "up";
        break;
    case Qt::Key_Down:
        direction = QPoint(0, 1);  // Mișcare în jos
        dirString = "down";
        break;
    case Qt::Key_Left:
        direction = QPoint(-1, 0);  // Mișcare la stânga
        dirString = "left";
        break;
    case Qt::Key_Right:
        direction = QPoint(1, 0);  // Mișcare la dreapta
        dirString = "right";
        break;
    }

    if (event->key() == QKeySequence(m_upKey).toString().at(0).unicode())
    {
        direction = QPoint(0, -1);  // Mișcare în sus
        dirString = "up";
    }
    else if (event->key() == QKeySequence(m_downKey).toString().at(0).unicode())
    {
        direction = QPoint(0, 1);  // Mișcare în jos
        dirString = "down";
    }
    else if (event->key() == QKeySequence(m_leftKey).toString().at(0).unicode())
    {
        direction = QPoint(-1, 0);  // Mișcare la stânga
        dirString = "left";
    }
    else if (event->key() == QKeySequence(m_rightKey).toString().at(0).unicode())
    {
        direction = QPoint(1, 0);  // Mișcare la dreapta
        dirString = "right";
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }

    auto newPositionOpt = m_serverObject.SendMoveToServer(dirString.toStdString());
    if (newPositionOpt) {
        const Point& newPosition = *newPositionOpt;

        if (newPosition.m_x != player1Position.x() || newPosition.m_y != player1Position.y()) {
            player1Position = QPoint(newPosition.m_y, newPosition.m_x);
            qDebug() << "[INFO] Player moved to new position: (" << player1Position.x() << ", " << player1Position.y() << ")";
        }
        else {
            qDebug() << "[INFO] Server rejected move. Position unchanged.";
        }
    }
    else {
        qWarning() << "[WARNING] Move validation failed. Check server connection.";
    }
    update();
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

    if (player1Position.x() >= 0 && player1Position.y() >= 0) {
        pixmap.load("./player1.png");
        painter.drawPixmap(player1Position.x() * 36, player1Position.y() * 36, pixmap.scaled(36, 36));
    }

    pixmap = QPixmap("./base.jpg").scaled(36, 36);
    painter.drawPixmap(basePosition.second * 36, basePosition.first * 36, pixmap);

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
