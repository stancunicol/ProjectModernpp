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
    ServerUtils server;
    scoreUpdateTimer = new QTimer(this);

    
    for (int i = 1; i <= 4; i++)
    {
        std::string response = m_serverObject.GetPlayerDataByIdFromServer();

        if (!response.empty()) {
            try {
                auto jsonResponse = nlohmann::json::parse(response);
                if (jsonResponse.contains("name") && jsonResponse.contains("score")) {
                    std::string playerName = jsonResponse["name"];
                    int score = jsonResponse["score"];
                    playerNames.push_back(playerName);
                    playerScores.push_back(score);

                }
            }
            catch (const std::exception& e) {
                qDebug() << "Error parsing server response: " << e.what();
            }
        }
    }
    if (scoreUpdateTimer != nullptr) {
        connect(scoreUpdateTimer, &QTimer::timeout, this, &GameMapInterface::updateScores);
        scoreUpdateTimer->start(10000); 
    }
    else {
        qDebug() << "Error: scoreUpdateTimer is nullptr.";
    }
    /*std::pair<int, int> basePosition = m_serverObject.GetBaseFromServer();
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
    }*/
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
void GameMapInterface::paintEvent(QPaintEvent* event)
{

    QPainter painter(this);
    painter.setFont(QFont("Segoe Script", 8));

    int startX = 15; //initial position on the x axis
    int startY = 15; // initial position on the y axis
    int spacing = 90; // the distance between players

    
    for (int i = 0; i < 4; i++) {
        // draw the name
        painter.drawText(startX + i * spacing, startY, QString::fromStdString(playerNames[i]));

        // draw the score
        painter.drawText(startX + i * spacing, startY + 20, "Score: " + QString::number(playerScores[i]));
    }
}
void GameMapInterface::updateScores()
{
    ServerUtils server;
    qDebug() << "Updating scores...";

    for (int i = 0; i < static_cast<int>(playerScores.size()); ++i) {
        
        std::string response = server.GetPlayerDataByIdFromServer(); 

        if (!response.empty()) {
            try {
                auto jsonResponse = nlohmann::json::parse(response);

                if (jsonResponse.contains("score")) {
                    int newScore = jsonResponse["score"];

                    // update the score only if it has changed
                    if (playerScores[i] != newScore) {
                        playerScores[i] = newScore;
                        qDebug() << "Player" << i + 1 << "score updated to:" << newScore;
                        update(); 
                    }
                }
            }
            catch (const std::exception& e) {
                qDebug() << "Error parsing server response: " << e.what();
            }
        }
    }
}
