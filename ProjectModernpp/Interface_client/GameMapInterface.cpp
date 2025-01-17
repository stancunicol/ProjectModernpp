#include "GameMapInterface.h"
#include "ServerUtils.h"

GameMapInterface::GameMapInterface(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Game Map");
    setFocusPolicy(Qt::StrongFocus);

    QTimer* fetchTimerPlayer = new QTimer(this);
    QTimer* fetchTimerEnemy = new QTimer(this);
    connect(fetchTimerPlayer, &QTimer::timeout, this, &GameMapInterface::updateOtherPlayers);
    connect(fetchTimerEnemy, &QTimer::timeout, this, &GameMapInterface::updateEnemies);
    fetchTimerPlayer->start(1000);
    fetchTimerEnemy->start(1000);

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
        player1Position = QPoint(-1, -1);
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

    for (const auto& [position, username] : otherPlayers) {
        pixmap.load("./player1.png");
        painter.drawPixmap(position.x() * 36, position.y() * 36, pixmap.scaled(36, 36));
    }

    pixmap = QPixmap("./base.jpg").scaled(36, 36);
    painter.drawPixmap(basePosition.second * 36, basePosition.first * 36, pixmap);

    for (auto enemy : enemies)
    {
        switch (enemy.id)
        {
        case 0:
            pixmap = QPixmap("./enemy1.png").scaled(36, 36);
            painter.drawPixmap(enemy.y * 36, enemy.x * 36, pixmap);
            break;
        case 1:
            pixmap = QPixmap("./enemy2.png").scaled(36, 36);
            painter.drawPixmap(enemy.y * 36, enemy.x * 36, pixmap);
            break;
        case 2:
            pixmap = QPixmap("./enemy3.png").scaled(36, 36);
            painter.drawPixmap(enemy.y * 36, enemy.x * 36, pixmap);
            break;
        case 3:
            pixmap = QPixmap("./enemy4.png").scaled(36, 36);
            painter.drawPixmap(enemy.y * 36, enemy.x * 36, pixmap);
            break;
        }
    }
}

void GameMapInterface::updateOtherPlayers() {
    m_serverObject.FetchPlayerStates();

    otherPlayers.clear(); // Clear existing data

    // Process the fetched data
    uint8_t currentUserId = m_serverObject.GetUserId();
    std::string url = "http://localhost:8080/playerState?playerId=" + std::to_string(currentUserId);
    std::string response = m_serverObject.GetServerData(url);

    if (response.empty()) {
        qWarning() << "Failed to fetch player states.";
        return;
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);
        if (jsonResponse.contains("players") && jsonResponse["players"].is_array()) {
            for (const auto& player : jsonResponse["players"]) {
                int x = player["position"]["x"];
                int y = player["position"]["y"];
                QString username = QString::fromStdString(player["username"]);
                otherPlayers.emplace_back(QPoint(y, x), username);
            }
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Error parsing player state response: " << e.what();
    }

    update();
}

void GameMapInterface::updateEnemies() {
    m_serverObject.FetchEnemyStates();  // Exemplu de funcție pentru a obține datele despre inamici

    enemies.clear(); // Clear existing enemies

    std::string url = "http://localhost:8080/getEnemiesState";
    std::string response = m_serverObject.GetServerData(url);

    if (response.empty()) {
        qWarning() << "Failed to fetch enemy states.";
        return;
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);

        if (jsonResponse.contains("enemies") && jsonResponse["enemies"].is_array()) {
            for (const auto& enemy : jsonResponse["enemies"]) {
                int x = enemy["position"]["x"];
                int y = enemy["position"]["y"];
                int id = enemy["id"];
                enemies.push_back({ x, y, id });
            }
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Error parsing enemy state response: " << e.what();
    }
    update();
}
