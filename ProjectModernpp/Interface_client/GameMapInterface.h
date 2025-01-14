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

private:
    ServerUtils m_serverObject;
};
