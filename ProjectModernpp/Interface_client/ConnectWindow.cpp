﻿#include "ConnectWindow.h"

ConnectWindow::ConnectWindow(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Join Room");
    setFixedSize(300, 200);

    QPalette palette;
    QColor customColor(20, 20, 20, 245);
    palette.setColor(QPalette::Window, customColor);
    palette.setColor(QPalette::WindowText, Qt::white);
    setPalette(palette);
    setAutoFillBackground(true);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("Insert code:", this);
    QFont font;
    font.setPointSize(14);
    font.setBold(true);

    label->setFont(font);
    layout->addWidget(label, 0, Qt::AlignCenter);

    m_insertCode = new QLineEdit(this);
    layout->addWidget(m_insertCode);
    m_insertCode->setAlignment(Qt::AlignCenter);
    m_insertCode->setStyleSheet("color: lightBlue;background-color: darkBlue;font-size: 20px;border: 2px solid lightBlue");
    m_insertCode->setFixedSize(240, 50);
    layout->setAlignment(Qt::AlignCenter);

    m_startButton = new QPushButton("START GAME", this);
    m_startButton->setStyleSheet("QPushButton {"
        "background-color: rgba(158, 204, 158, 1);"
        "color: black;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(124, 160, 124, 0.7);"
        "color: rgba(255, 255, 255, 0.5);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(86, 123, 86, 0.9);"
        "color: rgba(255, 255, 255, 1);"
        "}");
    layout->addWidget(m_startButton);
    connect(m_startButton, &QPushButton::clicked, this, &ConnectWindow::OnConnectClicked);
}

void ConnectWindow::OnConnectClicked()
{
    QString roomCode = m_insertCode->text();

    if (roomCode.isEmpty())
    {
        QMessageBox::warning(this, "Error", "The code field is empty!");
        return;
    }

    bool isCodeValid = m_serverObject.CheckServerCode(roomCode.toStdString(), m_serverObject.GetUserId());

    if (isCodeValid) 
    {
        qDebug() << "Player successfully joined the room.";
    }
    else 
    {
        qDebug() << "Failed to join the room.";
    }
    if (isCodeValid)
    {
        QMessageBox::information(this, "Success", "Room code is valid. Joining room...");
        accept();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Invalid room code!");
    }
}

ConnectWindow::~ConnectWindow()
{

}
