#include "GameMapInterface.h"

struct Enemy;
struct Point;
struct BulletResponse;

GameMapInterface::GameMapInterface(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Game Map");
    setFocusPolicy(Qt::StrongFocus);

    const int scorePanelWidth = 200;

    QTimer* scoreUpdateTimer = new QTimer(this);
    QTimer* fetchTimerPlayer = new QTimer(this);
    QTimer* fetchTimerEnemy = new QTimer(this);
    connect(fetchTimerEnemy, &QTimer::timeout, this, &GameMapInterface::UpdateEnemies);
    connect(scoreUpdateTimer, &QTimer::timeout, this, &GameMapInterface::UpdatePlayerScores);
    fetchTimerEnemy->start(4000);
    scoreUpdateTimer->start(1000);

    QTimer* bulletUpdateTimer = new QTimer(this);
    connect(bulletUpdateTimer, &QTimer::timeout, this, &GameMapInterface::UpdateBullets);
    bulletUpdateTimer->start(100);
    QTimer* bombUpdateTimer = new QTimer(this);
    connect(bombUpdateTimer, &QTimer::timeout, this, &GameMapInterface::UpdateBombs);
    bombUpdateTimer->start(1000);

    QTimer* baseCheckTimer = new QTimer(this);
    connect(baseCheckTimer, &QTimer::timeout, this, &GameMapInterface::CheckBaseState);
    baseCheckTimer->start(1000);

    /*QTimer* bulletsUpdateTimer = new QTimer(this);
    connect(bulletsUpdateTimer, &QTimer::timeout, this, &GameMapInterface::UpdateEnemiesBullets);
    bulletsUpdateTimer->start(5000);*/

    m_serverObject.GetMapFromServer();
    m_matrix = m_serverObject.GetMap();
    m_height = m_matrix.size();
    m_width = m_matrix[0].size();

    std::cout << "Matrix:" << '\n';
    for (const auto& row : m_matrix)
    {
        for (const auto& cell : row)
        {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    resize(36 * m_width + scorePanelWidth, 36 * m_height);

    m_basePosition = m_serverObject.GetBaseFromServer();
    qDebug() << "Base Position - X:" << m_basePosition.second << " Y:" << m_basePosition.first;
    m_baseInitialized = true;

    m_bombs = m_serverObject.GetBombsFromServer();
    if (!m_bombs.empty())
    {
        qDebug() << "Received " << m_bombs.size() << " bombs from server.";
        for (const auto& bomb : m_bombs)
        {
            qDebug() << "Bomb ID: " << bomb.id << " Position: (" << bomb.y << ", " << bomb.x << ")";
        }
    }
    else
    {
        qDebug() << "No bomb data available.";
    }

    m_serverObject.GetPlayerPositionsFromServer();
    auto playerPositions = m_serverObject.GetPlayerPositions();

    bool playerFound = false;
    for (const auto& position : playerPositions)
    {
        if (std::stoi(position.second) == m_serverObject.GetUserId())
        {
            m_player1Position = QPoint(position.first.m_y, position.first.m_x);
            qDebug() << "[INFO] Player initialized at position: (" << m_player1Position.y() << ", " << m_player1Position.x() << ")";
            playerFound = true;
            break;
        }
    }

    if (!playerFound)
    {
        qWarning() << "[WARNING] Player position not found. Ensure server returned correct data.";
        m_player1Position = QPoint(-1, -1);
    }

    bool baseState = m_serverObject.GetBaseState();
    if (baseState) {
        qDebug() << "[INFO] Base is intact.";
    }
    else {
        qDebug() << "[INFO] Base is destroyed.";
    }

    LoadControls();
    update();
}

GameMapInterface::~GameMapInterface()
{

}

void GameMapInterface::LoadControls()
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
        direction = QPoint(0, -1);
        dirString = "up";
        m_shootDirection = QPoint(-1, 0);
        break;
    case Qt::Key_Down:
        direction = QPoint(0, 1);
        dirString = "down";
        m_shootDirection = QPoint(1, 0);
        break;
    case Qt::Key_Left:
        direction = QPoint(-1, 0);
        dirString = "left";
        m_shootDirection = QPoint(0, -1);
        break;
    case Qt::Key_Right:
        direction = QPoint(1, 0);
        dirString = "right";
        m_shootDirection = QPoint(0, 1);
        break;
    case Qt::Key_Space:
        FireBullet();
        return;
    }

    if (event->key() == QKeySequence(m_upKey).toString().at(0).unicode())
    {
        direction = QPoint(0, -1);
        dirString = "up";
        m_shootDirection = QPoint(-1, 0);
    }
    else if (event->key() == QKeySequence(m_downKey).toString().at(0).unicode())
    {
        direction = QPoint(0, 1);
        dirString = "down";
        m_shootDirection = QPoint(1, 0);
    }
    else if (event->key() == QKeySequence(m_leftKey).toString().at(0).unicode())
    {
        direction = QPoint(-1, 0);
        dirString = "left";
        m_shootDirection = QPoint(0, -1);
    }
    else if (event->key() == QKeySequence(m_rightKey).toString().at(0).unicode())
    {
        direction = QPoint(1, 0);
        dirString = "right";
        m_shootDirection = QPoint(0, 1);
    }
    else if (event->key() == QKeySequence(m_fireKey).toString().at(0).unicode())
    {
        FireBullet();
        return;
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
    m_currentDirection = direction;
    qDebug() << "[INFO] Firing bullet from position:" << m_player1Position << "in direction:" << m_currentDirection;

    if (!dirString.isEmpty())
    {
        auto newPositionOpt = m_serverObject.SendMoveToServer(dirString.toStdString());
        if (newPositionOpt)
        {
            const Point& newPosition = *newPositionOpt;

            if (newPosition.m_x != m_player1Position.x() || newPosition.m_y != m_player1Position.y())
            {
                m_player1Position = QPoint(newPosition.m_y, newPosition.m_x);
                qDebug() << "[INFO] Player moved to new position: (X:" << m_player1Position.y() << ", Y:" << m_player1Position.x() << ")";
            }
            else
            {
                qDebug() << "[INFO] Server rejected move. Position unchanged.";
            }
        }
        else
        {
            qWarning() << "[WARNING] Move validation failed. Check server connection.";
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
    for (int i = 0; i < m_height; i++)
        for (int j = 0; j < m_width; j++)
        {
            switch (m_matrix[i][j])
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

    if (m_player1Position.x() >= 0 && m_player1Position.y() >= 0)
    {
        pixmap.load("./player1.png");
        painter.drawPixmap(m_player1Position.x() * 36, m_player1Position.y() * 36, pixmap.scaled(36, 36));

        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        int yOffsetText = (m_player1Position.y() == 0) ? 36 : -5;
        painter.drawText(m_player1Position.x() * 36, m_player1Position.y() * 36 + yOffsetText, "You");

        painter.setPen(Qt::blue);
        painter.drawText(m_player1Position.x() * 36 + 1, m_player1Position.y() * 36 + yOffsetText + 1, "You");
    }

    std::vector<QString> playerImages = {
    "./player2.png",
    "./player3.png",
    "./player4.png"
    };

    for (size_t i = 0; i < m_otherPlayers.size(); ++i)
    {
        const auto& [position, username] = m_otherPlayers[i];

        QPixmap playerPixmap;

        if (i < playerImages.size())
        {
            playerPixmap.load(playerImages[i]);
        }
        painter.drawPixmap(position.x() * 36, position.y() * 36, playerPixmap.scaled(36, 36));
    }

    pixmap = QPixmap("./base.jpg").scaled(36, 36);
    painter.drawPixmap(m_basePosition.second * 36, m_basePosition.first * 36, pixmap);

    for (const auto& enemy : m_enemies)
    {
        switch (enemy.id)
        {
        case 0:
            pixmap = QPixmap("./enemy1.png").scaled(36, 36);
            break;
        case 1:
            pixmap = QPixmap("./enemy2.png").scaled(36, 36);
            break;
        case 2:
            pixmap = QPixmap("./enemy3.png").scaled(36, 36);
            break;
        case 3:
            pixmap = QPixmap("./enemy4.png").scaled(36, 36);
            break;
        }
        painter.drawPixmap(enemy.y * 36, enemy.x * 36, pixmap);

        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 10, QFont::Bold));

        int yOffsetText = (enemy.x == 0) ? 36 : -5;
        painter.drawText(enemy.y * 36, enemy.x * 36 + yOffsetText, "Enemy");

        painter.setPen(Qt::red);
        painter.drawText(enemy.y * 36 + 1, enemy.x * 36 + yOffsetText + 1, "Enemy");
    }

    for (const auto& bullet : m_activeBullets)
    {
        if (bullet.active)
        {
            painter.setBrush(Qt::red);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(bullet.position.m_y * 36 + 12, bullet.position.m_x * 36 + 12, 12, 12);
        }
    }

    for (const auto& bomb : m_bombs) {
        if (bomb.x == -1 && bomb.y == -1) {
            pixmap.load("./bomb.png");
            painter.drawPixmap(bomb.x * 36, bomb.y * 36, pixmap.scaled(36, 36));
        }
    }

    int scorePanelStartX = 36 * m_width;
    QRect scorePanelRect(scorePanelStartX, 0, 200, 36 * m_height);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(scorePanelRect);

    QPixmap scorePanelBackground("./score_panel_bg.jpg");
    painter.drawPixmap(scorePanelRect, scorePanelBackground.scaled(scorePanelRect.size()));

    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(scorePanelRect, Qt::AlignTop | Qt::AlignHCenter, "Player Scores");

    int totalHeight = scorePanelRect.height();
    int sectionPadding = 10;
    int yOffset = sectionPadding;

    int remainingHeight = totalHeight - (2 * sectionPadding);
    int scoreSectionHeight = remainingHeight * 0.75;
    int controlsSectionHeight = remainingHeight * 0.25;

    int scoreCount = m_playerScores.size();
    int scoreLineHeight = scoreCount > 0 ? scoreSectionHeight / scoreCount : scoreSectionHeight;

    int controlCount = 5;
    int controlLineHeight = controlsSectionHeight / controlCount;

    painter.setFont(QFont("Arial", 12, QFont::Bold));
    for (const auto& [name, score] : m_playerScores)
    {
        QString scoreText = name + "\nScor: " + QString::number(score);
        QRect textRect(scorePanelStartX, yOffset, 200, scoreLineHeight);

        painter.drawText(textRect, Qt::AlignCenter, scoreText);

        yOffset += scoreLineHeight;
    }

    yOffset += sectionPadding;

    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(scorePanelStartX + 10, yOffset, "Controls:");
    yOffset += controlLineHeight;

    painter.drawText(scorePanelStartX + 10, yOffset, "Move Up: " + m_upKey + " / ↑");
    yOffset += controlLineHeight;
    painter.drawText(scorePanelStartX + 10, yOffset, "Move Down: " + m_downKey + " / ↓");
    yOffset += controlLineHeight;
    painter.drawText(scorePanelStartX + 10, yOffset, "Move Left: " + m_leftKey + " / ←");
    yOffset += controlLineHeight;
    painter.drawText(scorePanelStartX + 10, yOffset, "Move Right: " + m_rightKey + " / →");
    yOffset += controlLineHeight;
    painter.drawText(scorePanelStartX + 10, yOffset, "Fire: " + m_fireKey + " / Space");
}

void GameMapInterface::UpdateOtherPlayers()
{
    m_serverObject.FetchPlayerStates();

    m_otherPlayers.clear(); 

    uint8_t currentUserId = m_serverObject.GetUserId();
    std::string url = "http://localhost:8080/playerState?playerId=" + std::to_string(currentUserId);
    std::string response = m_serverObject.GetServerData(url);

    if (response.empty())
    {
        qWarning() << "Failed to fetch player states.";
        return;
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);
        if (jsonResponse.contains("players") && jsonResponse["players"].is_array())
        {
            for (const auto& player : jsonResponse["players"]) {
                int x = player["position"]["x"];
                int y = player["position"]["y"];
                QString username = QString::fromStdString(player["username"]);
                m_otherPlayers.emplace_back(QPoint(y, x), username);
            }
        }
    }
    catch (const std::exception& e)
    {
        qWarning() << "Error parsing player state response: " << e.what();
    }

    update();
}

void GameMapInterface::UpdateEnemies()
{
    std::vector<Enemy> fetchedEnemies;
    m_serverObject.FetchEnemyStates(fetchedEnemies);

    for (const auto& enemy : fetchedEnemies)
    {
        qDebug() << "Enemy ID:" << enemy.id << " Position: (" << enemy.x << ", " << enemy.y << ")";
        if (m_serverObject.GetMap()[enemy.x][enemy.y] == 1 || m_serverObject.GetMap()[enemy.x][enemy.y] == 2)
            qDebug() << "Invalid position.\n";
    }

    m_enemies = std::move(fetchedEnemies);
    update();
}

void GameMapInterface::UpdatePlayerScores()
{
    auto scores = m_serverObject.GetPlayerDataByIdFromServer();
    m_playerScores.clear();
    for (const auto& [name, score] : scores)
    {
        m_playerScores.emplace_back(QString::fromStdString(name), score);
    }
}

void GameMapInterface::FireBullet()
{
    Point bulletPosition(m_player1Position.y(), m_player1Position.x());
    Point bulletDirection(m_currentDirection.y(), m_currentDirection.x());

    auto result = m_serverObject.FireBullet({ m_player1Position.y(), m_player1Position.x() }, { m_shootDirection.x(), m_shootDirection.y() });
    if (result.success)
    {
        qDebug() << "[INFO] Bullet fired!";
        Bullet newBullet = { bulletPosition, bulletDirection, true };
        m_activeBullets.push_back(newBullet);
        if (result.collision)
        {
            qDebug() << "[INFO] Bullet hit something!";
        }
        else
        {
            qDebug() << "[INFO] Bullet missed.";
        }
    }
    else {
        qWarning() << "[WARNING] Bullet firing failed.";
    }
}

void GameMapInterface::UpdateMap()
{
    m_serverObject.GetMapFromServer();
    m_matrix = m_serverObject.GetMap();
    repaint();
}

void GameMapInterface::UpdateBullets()
{
    for (auto& bullet : m_activeBullets)
    {
        if (!bullet.active)
        {
            continue;
        }

        bullet.position.m_x += bullet.direction.m_x;
        bullet.position.m_y += bullet.direction.m_y;

        if (m_matrix[bullet.position.m_x][bullet.position.m_y] == 2)
        {
            qDebug() << "[INFO] Bullet hit a wall!";
            bullet.active = false;
        }
        if (m_matrix[bullet.position.m_x][bullet.position.m_y] == 1)
        {
            qDebug() << "[INFO] Bullet hit a wall!";
            bullet.active = false;
            UpdateMap();
        }
        for (const auto& enemy : m_enemies)
        {
            if (bullet.position.m_x == enemy.x && bullet.position.m_y == enemy.y)
            {
                qDebug() << "[INFO] Bullet hit an enemy!";
                bullet.active = false;
            }
        }
        if (bullet.position.m_x < 0 || bullet.position.m_x >= m_height ||
            bullet.position.m_y < 0 || bullet.position.m_y >= m_width)
        {
            bullet.active = false;
        }
    }

    m_activeBullets.erase(std::remove_if(m_activeBullets.begin(), m_activeBullets.end(),
        [](const Bullet& bullet) { return !bullet.active; }),
        m_activeBullets.end());

    update();
}

void GameMapInterface::UpdateBombs() {
    auto updatedBombs = m_serverObject.GetBombsFromServer();

    for (const auto& bomb : updatedBombs) {
        if (bomb.x == -1 && bomb.y == -1) {

            qDebug() << "[INFO] Bomb with ID:" << bomb.id << "has exploded!";
        }
        else {
            qDebug() << "[INFO] Bomb ID:" << bomb.id << " Position: (" << bomb.y << ", " << bomb.x << ")";
        }
    }

    m_bombs = std::move(updatedBombs);
    update();
}

void GameMapInterface::CheckBaseState()
{
    if (!m_baseInitialized) {
        qDebug() << "[INFO] Base not initialized yet.";
        return;
    }

    bool baseExists = m_serverObject.GetBaseState();

    if (!baseExists) {
        qDebug() << "[ALERT] Base has been destroyed!";
        m_basePosition.first = -1;
        m_basePosition.second = -1;
        QString winner;
        int maxScore = -1;

        for (const auto& [name, score] : m_playerScores)
        {
            if (score > maxScore)
            {
                maxScore = score;
                winner = name;
            }
        }

        if (!winner.isEmpty())
        {
            QMessageBox::information(this, "Game Over", "The winner is " + winner + " with " + QString::number(maxScore) + " points!");
        }
        QCoreApplication::quit();
    }
    else {
        qDebug() << "[INFO] Base is safe.";
    }
}

void GameMapInterface::UpdateEnemiesBullets() {
    std::string url = "http://localhost:8080/getBullets?playerId=" + std::to_string(m_serverObject.GetUserId());
    std::string response = m_serverObject.GetServerData(url);

    if (response.empty()) {
        qWarning() << "[ERROR] Failed to fetch bullets. Server returned an empty response.";
        return;
    }

    try {
        auto jsonResponse = nlohmann::json::parse(response);

        if (jsonResponse.contains("bullets") && jsonResponse["bullets"].is_array()) {
            std::unordered_set<std::string> uniqueBulletIds;
            std::vector<Bullet> newBullets;

            for (const auto& bullet : jsonResponse["bullets"]) {
                if (!bullet.contains("position") || !bullet.contains("direction")) {
                    qWarning() << "[WARNING] Invalid bullet structure received. Skipping.";
                    continue;
                }

                int x = bullet["position"]["x"];
                int y = bullet["position"]["y"];
                int dx = bullet["direction"]["x"];
                int dy = bullet["direction"]["y"];

                if (y >= 0 && y < m_width && x >= 0 && x < m_height && (dx != 0 || dy != 0)) {
                    Bullet newBullet(Point(x, y), Point(dx, dy), true);

                    std::string bulletKey = std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(dx) + "_" + std::to_string(dy);
                    if (uniqueBulletIds.find(bulletKey) == uniqueBulletIds.end()) {
                        uniqueBulletIds.insert(bulletKey);
                        newBullets.emplace_back(newBullet);
                    }
                }
                else {
                    qWarning() << "[WARNING] Bullet out of bounds or invalid direction at position (" << x << ", " << y << "). Ignored.";
                }
            }

            if (newBullets != m_activeBullets) {
                m_activeBullets = std::move(newBullets);
                qDebug() << "[INFO] Bullets updated. Total active bullets: " << m_activeBullets.size();
            }
        }
        else {
            qWarning() << "[ERROR] Invalid bullets data received. 'bullets' field missing or not an array.";
        }
    }
    catch (const std::exception& e) {
        qWarning() << "[ERROR] Exception while parsing bullets data: " << e.what();
    }

    m_activeBullets.erase(std::remove_if(m_activeBullets.begin(), m_activeBullets.end(),
        [this](const Bullet& bullet) {
            return !bullet.active || bullet.position.m_x < 0 || bullet.position.m_x >= m_height ||
                bullet.position.m_y < 0 || bullet.position.m_y >= m_width;
        }),
        m_activeBullets.end());

    update();
}
