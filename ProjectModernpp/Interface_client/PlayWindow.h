#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPalette>
#include <QColor>
#include "ServerUtils.h"

class PlayWindow : public QDialog
{
    Q_OBJECT

public:
    PlayWindow(QWidget* parent = nullptr);
    ~PlayWindow();

protected:
    void OnGenerateClicked();
    void OnForwardClicked();

private:
    QLineEdit* m_generateCode = nullptr;
    QPushButton* m_generateButton = nullptr;
    QPushButton* m_forwardButton = nullptr;
    bool m_generate = false;

private:
    ServerUtils m_serverObject;
};