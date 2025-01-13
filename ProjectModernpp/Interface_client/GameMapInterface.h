#pragma once

#include <QMainWindow>
#include "ServerUtils.h"
#include <iostream>

class GameMapInterface : public QMainWindow
{
    Q_OBJECT

public:
    GameMapInterface(QWidget* parent = nullptr);
    ~GameMapInterface();
    void keyPressEvent(QKeyEvent* event);
private slots:
    void updateScores(); // Slot for update the scores
private:
    ServerUtils m_serverObject;
    std::vector<std::string> playerNames;
    std::vector<int> playerScores;
    QTimer* scoreUpdateTimer; // Timer
protected:
    void paintEvent(QPaintEvent* event) override;

};
