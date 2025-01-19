#pragma once

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QPalette>
#include <QColor>
#include "ServerUtils.h"

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();
    void OnLoginClicked(); 
    void OnRegisterClicked(); 

private:
    QPushButton* m_loginButton = nullptr;
    QPushButton* m_registerButton = nullptr; 
    QPushButton* m_cancelButton = nullptr;
    QLineEdit* m_usernameLineEdit = nullptr; 
    QLabel* m_messageLabel = nullptr; 
    ServerUtils m_serverObject;
};
