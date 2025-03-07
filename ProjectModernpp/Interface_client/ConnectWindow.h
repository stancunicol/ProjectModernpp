﻿#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox> 
#include <QPalette>
#include <QColor>
#include "ServerUtils.h"

class ConnectWindow : public QDialog
{
    Q_OBJECT

public:
    ConnectWindow(QWidget* parent = nullptr);
    ~ConnectWindow();
    void OnConnectClicked();

private:
    QLineEdit* m_insertCode = nullptr;
    QPushButton* m_startButton = nullptr;
    QLabel* m_messageLabel = nullptr;
    ServerUtils m_serverObject;
};


