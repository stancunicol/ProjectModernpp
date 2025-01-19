#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Login");
    setFixedSize(400, 300);

    QPalette palette;
    QColor customColor(20, 20, 20, 245);
    palette.setColor(QPalette::Window, customColor);
    setPalette(palette);
    setAutoFillBackground(true);

    QVBoxLayout* layout = new QVBoxLayout(this);
    m_usernameLineEdit = new QLineEdit(this);
    m_usernameLineEdit->setPlaceholderText("Username");
    m_usernameLineEdit->setFixedSize(400, 50);

    layout->addWidget(m_usernameLineEdit);
    layout->addSpacerItem(new QSpacerItem(0, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));

    m_loginButton = new QPushButton("Login", this);
    m_loginButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(0,255,0,0.7);" 
        "color: white;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0,128,0,0.7);" 
        "color: rgba(0, 0, 0,0.5);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0,77,0,0.7);" 
        "color: rgba(0, 0, 0,0.7);"
        "}");
    layout->addWidget(m_loginButton);

    m_registerButton = new QPushButton("Register", this);
    m_registerButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(0,255,0,0.7);" 
        "color: white;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0,128,0,0.7);" 
        "color: rgba(0, 0, 0,0.5);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0,77,0,0.7);"
        "color: rgba(0, 0, 0,0.7);"
        "}");
    layout->addWidget(m_registerButton);

    m_cancelButton = new QPushButton("Cancel", this);
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(255,127,127,0.7);" 
        "color: white;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255,0,0,0.7);" 
        "color: rgba(0, 0, 0,0.5);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(139,0,0,0.7);" 
        "color: rgba(0, 0, 0,0.7);"
        "}");
    layout->addWidget(m_cancelButton);
    layout->setAlignment(Qt::AlignCenter);

    m_messageLabel = new QLabel(this);
    layout->addWidget(m_messageLabel);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::OnLoginClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &LoginWindow::OnRegisterClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &LoginWindow::close);
}

LoginWindow::~LoginWindow()
{

}

void LoginWindow::OnLoginClicked() {
    QString username = m_usernameLineEdit->text();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter username.");
        return;
    }

    m_serverObject.SendUserRequestToServer(username.toStdString());

    try {
        if (m_serverObject.GetStatus() == "login") {
            QMessageBox::information(this, "Login Successful", "User " + username + " logged in successfully!");
            accept();
        }
        else if (m_serverObject.GetStatus() == "register") {
            QMessageBox::information(this, "Login Successful", "User " + username + " registered successfully!");
            accept();
        }
        else {
            QMessageBox::warning(this, "Login Failed", "Invalid username or server error.");
        }
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Login Failed", "Error processing server response: " + QString::fromStdString(e.what()));
    }

    m_usernameLineEdit->clear();
}

void LoginWindow::OnRegisterClicked() {
    QString username = m_usernameLineEdit->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Registration Failed", "Please enter username.");
        return;
    }

    m_serverObject.SendUserRequestToServer(username.toStdString());

    try {
        if (m_serverObject.GetStatus() == "register") {
            QMessageBox::information(this, "Registration Successful", "User " + username + " registered successfully!");
            accept();
        }
        else if (m_serverObject.GetStatus() == "login") {
            QMessageBox::information(this, "Registration Failed", "User " + username + " already exists. Please log in.");
        }
        else {
            QMessageBox::warning(this, "Registration Failed", "Failed to register user.");
        }
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Registration Failed", "Error processing server response: " + QString::fromStdString(e.what()));
    }

    m_usernameLineEdit->clear();
}
