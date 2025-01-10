#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Login");//title
    setFixedSize(400, 300);//400X300

    QPalette palette;
    QColor customColor(20, 20, 20, 245);
    palette.setColor(QPalette::Window, customColor);
    setPalette(palette);
    setAutoFillBackground(true);

    QVBoxLayout* layout = new QVBoxLayout(this);//create layout
    m_usernameLineEdit = new QLineEdit(this);//create username
    m_usernameLineEdit->setPlaceholderText("Username");//used to set a placeholder text in the username input field
    m_usernameLineEdit->setFixedSize(400, 50);//size

    layout->addWidget(m_usernameLineEdit);
    layout->addSpacerItem(new QSpacerItem(0, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));//spacer

    m_loginButton = new QPushButton("Login", this);//create login button
    m_loginButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(0,255,0,0.7);" // green
        "color: white;"
        "border: none;"
        "padding: 10px;"// the inner space of a widget between the widget's content and it's bounds
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0,128,0,0.7);" // dark green
        "color: rgba(0, 0, 0,0.5);"//black
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0,77,0,0.7);" // even darker green
        "color: rgba(0, 0, 0,0.7);"//black
        "}");
    layout->addWidget(m_loginButton);

    m_registerButton = new QPushButton("Register", this);//create register button
    m_registerButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(0,255,0,0.7);" // green
        "color: white;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0,128,0,0.7);" // dark green
        "color: rgba(0, 0, 0,0.5);"//black
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0,77,0,0.7);" // even darker green
        "color: rgba(0, 0, 0,0.7);"//black
        "}");
    layout->addWidget(m_registerButton);

    m_cancelButton = new QPushButton("Cancel", this);
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(255,127,127,0.7);" // red
        "color: white;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255,0,0,0.7);" // dark red
        "color: rgba(0, 0, 0,0.5);"//black
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(139,0,0,0.7);" // even darker
        "color: rgba(0, 0, 0,0.7);"//black
        "}");
    layout->addWidget(m_cancelButton);
    layout->setAlignment(Qt::AlignCenter);

    m_messageLabel = new QLabel(this);//label for messages
    layout->addWidget(m_messageLabel);

    //Connect signals to slots(function that conects a signal emitted by another object)
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
