#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle("Login");//title
	setFixedSize(400, 300);//400X300
	QVBoxLayout* layout = new QVBoxLayout(this);//create layout
	loginButton = new QPushButton("Login", this);//create login button
	registerButton = new QPushButton("Register", this);//create register button
	cancelButton = new QPushButton("Cancel", this);

	layout->addWidget(loginButton);
	layout->addWidget(registerButton);
	layout->addWidget(cancelButton);
	layout->setAlignment(Qt::AlignCenter);
}

LoginWindow::~LoginWindow()
{

}
