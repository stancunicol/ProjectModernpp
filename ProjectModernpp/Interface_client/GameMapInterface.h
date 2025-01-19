#pragma once

#include <QMainWindow>
#include<QTimer>
#include<QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <iostream>
#include <QSettings>
#include "ServerUtils.h"

struct Bullet {
    Point position;  // Current position of the bullet
    Point direction; // Direction of movement
    bool active;     // Whether the bullet is still active or has been deactivated (e.g., due to a collision)
};

class GameMapInterface : public QMainWindow
{
public:
    GameMapInterface(QWidget* parent = nullptr);
    ~GameMapInterface();
    void loadControls();
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void updateOtherPlayers();
    void updateEnemies();
    void updatePlayerScores();
    void fireBullet();
    void updateMap();
    void updateBullets();
    void updateBombs();
private:
    ServerUtils m_serverObject;
    std::pair<int, int> basePosition;
    std::vector<Bomb> bombs;
    std::vector<Enemy> m_enemies;
    std::vector<std::vector<int>> matrix;
    uint16_t height, width;
    std::vector<std::pair<Point, bool>> playerPositions;
    QString m_upKey;
    QString m_downKey;
    QString m_leftKey;
    QString m_rightKey;
    QString m_fireKey;

    QPoint player1Position;

    std::vector<std::pair<QPoint, QString>> otherPlayers;
    std::vector<std::pair<QString, int>> playerScores;
    QPoint m_currentDirection = QPoint(0, 0);
    std::vector<Bullet> activeBullets;
};
