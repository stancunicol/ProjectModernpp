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

	loginButton = new QPushButton("Login", this);//create login button
	registerButton = new QPushButton("Register", this);//create register button
	cancelButton = new QPushButton("Cancel", this);

	layout->addWidget(loginButton);
	layout->addWidget(registerButton);
	layout->addWidget(cancelButton);
	layout->setAlignment(Qt::AlignCenter);

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
	QMessageBox::information(this, "Login", "The Login button has been pressed!");
}

void LoginWindow::onRegisterClicked()
{
	QMessageBox::information(this, "Register", "The Register button has been pressed!");
}

