#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget* parent)
	: QDialog(parent)
{
    setWindowTitle("Login");//title
    setFixedSize(400, 300);//400X300

    QVBoxLayout* layout = new QVBoxLayout(this);//create layout
    usernameLineEdit = new QLineEdit(this);//create username
    usernameLineEdit->setPlaceholderText("Username");//used to set a placeholder text in the username input field
    usernameLineEdit->setFixedSize(400, 50);//size

    layout->addWidget(usernameLineEdit);
    layout->addSpacerItem(new QSpacerItem(0, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));//spacer

    loginButton = new QPushButton("Login", this);//create login button
    loginButton->setStyleSheet(
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
    layout->addWidget(loginButton);

    registerButton = new QPushButton("Register", this);//create register button
    registerButton->setStyleSheet(
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
    layout->addWidget(registerButton);

    cancelButton = new QPushButton("Cancel", this);
    cancelButton->setStyleSheet(
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
    layout->addWidget(cancelButton);
    layout->setAlignment(Qt::AlignCenter);

    messageLabel = new QLabel(this);//label for messages
    layout->addWidget(messageLabel);

    //Connect signals to slots(function that conects a signal emitted by another object)
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(cancelButton, &QPushButton::clicked, this, &LoginWindow::close);
}

LoginWindow::~LoginWindow()
{

}

void LoginWindow::onLoginClicked()
{
	QString username = usernameLineEdit->text();//get your username and password

	if (username.isEmpty()) {
		QMessageBox::warning(this, "Login Failed", "Please enter username.");
		return;
	}
	else
	{
		QMessageBox::information(this, "Login Successful", "User " + username + " login successfully!");
		accept();
	}
	usernameLineEdit->clear();//reset fields after registration
}

void LoginWindow::onRegisterClicked()
{
	QString username = usernameLineEdit->text();//get your username and password

	if (username.isEmpty()) {
		QMessageBox::warning(this, "Registration Failed", "Please enter username.");
		return;
	}
	else
	{
		QMessageBox::information(this, "Registration Successful", "User " + username + " registered successfully!");
		accept();
	}
	usernameLineEdit->clear();//reset fields after registration
}

