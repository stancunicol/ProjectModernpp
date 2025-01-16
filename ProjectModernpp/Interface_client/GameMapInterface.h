#pragma once

#include <QMainWindow>
#include "ServerUtils.h"
#include <iostream>

class GameMapInterface : public QMainWindow
{
public:
    GameMapInterface(QWidget* parent = nullptr);
    ~GameMapInterface();
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
private:
    ServerUtils m_serverObject;
    std::pair<int, int> basePosition;
    std::vector<Bomb> bombs;
    std::vector<Enemy> enemies;
    std::vector<std::vector<int>> matrix;
    uint16_t height, width;
    std::vector<std::pair<Point, bool>> playerPositions;
};
