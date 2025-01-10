#pragma once

#include <QDialog>
#include <QPushButton>//create buttons
#include <QVBoxLayout>//for vertical layout
#include <QMessageBox>
#include <QLineEdit>//to create a text input field
#include <QLabel>//to display images or text
#include <QPalette>
#include <QColor>
#include "ServerUtils.h"

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

protected:

    void OnLoginClicked(); // slot for managing the click on the login button
    void OnRegisterClicked(); // slot for managing the click on the registration button

private:
    QPushButton* m_loginButton = nullptr; // Login button
    QPushButton* m_registerButton = nullptr; // Registration button
    QPushButton* m_cancelButton = nullptr;// Cancel button
    QLineEdit* m_usernameLineEdit = nullptr; // Username field
    QLabel* m_messageLabel = nullptr; // Label for feedback messages

private:
    ServerUtils m_serverObject;
};
