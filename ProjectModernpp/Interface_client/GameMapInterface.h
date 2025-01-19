#pragma once

#include <QMainWindow>
#include<QTimer>
#include<QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <iostream>
#include <QSettings>
#include <QMessageBox>
#include <unordered_set>
#include "ServerUtils.h"

struct Bullet {
    Point position;  
    Point direction; 
    bool active; 

    Bullet(const Point& pos, const Point& dir, bool isActive)
        : position(pos), direction(dir), active(isActive) {}

    bool operator==(const Bullet& other) const {
        return position == other.position && direction == other.direction && active == other.active;
    }
};

class GameMapInterface : public QMainWindow
{
public:
    GameMapInterface(QWidget* parent = nullptr);
    ~GameMapInterface();
    void LoadControls();
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void UpdateOtherPlayers();
    void UpdateEnemies();
    void UpdatePlayerScores();
    void FireBullet();
    void UpdateMap();
    void UpdateBullets();
    void UpdateBombs();
    void CheckBaseState();
    void UpdateEnemiesBullets();
private:
    ServerUtils m_serverObject;
    std::pair<int, int> m_basePosition;
    std::vector<Bomb> m_bombs;
    std::vector<Enemy> m_enemies;
    std::vector<std::vector<int>> m_matrix;
    uint16_t m_height, m_width;
    std::vector<std::pair<Point, bool>> m_playerPositions;
    QString m_upKey;
    QString m_downKey;
    QString m_leftKey;
    QString m_rightKey;
    QString m_fireKey;
    QPoint m_player1Position;
    std::vector<std::pair<QPoint, QString>> m_otherPlayers;
    std::vector<std::pair<QString, int>> m_playerScores;
    QPoint m_currentDirection = QPoint(0, 0);
    QPoint m_shootDirection = QPoint(0, 0);
    std::vector<Bullet> m_activeBullets;
    bool m_baseInitialized = false;
};