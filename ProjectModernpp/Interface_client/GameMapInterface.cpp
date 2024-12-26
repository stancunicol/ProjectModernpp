#include "GameMapInterface.h"
#include "ServerUtils.h"
#include <QKeyEvent>
#include <QDebug>

GameMapInterface::GameMapInterface(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Game Map");
    setFixedSize(600, 600);
}

GameMapInterface::~GameMapInterface()
{
}

void GameMapInterface::keyPressEvent(QKeyEvent* event)
{
    // Check which key was pressed and send the corresponding direction to the server
    if (event->key() == Qt::Key_W)
    {
        qDebug() << "W key pressed - Moving Up";
        SendMoveToServer(1, "up");  // Sending the move command to the server
    }
    else if (event->key() == Qt::Key_S)
    {
        qDebug() << "S key pressed - Moving Down";
        SendMoveToServer(1, "down");
    }
    else if (event->key() == Qt::Key_A)
    {
        qDebug() << "A key pressed - Moving Left";
        SendMoveToServer(1, "left");
    }
    else if (event->key() == Qt::Key_D)
    {
        qDebug() << "D key pressed - Moving Right";
        SendMoveToServer(1, "right");
    }
    else
    {
        QMainWindow::keyPressEvent(event);  // Default key handling
    }
}
