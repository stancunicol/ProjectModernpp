#pragma once

#include <QDialog>
#include <QPushButton>//create buttons
#include <QVBoxLayout>//for vertical layout
#include < QMessageBox>
#include <QLineEdit>//to create a text input field
#include <QLabel>//to display images or text


class LoginWindow : public QDialog
{
	Q_OBJECT
		 
public:
	LoginWindow(QWidget * parent = nullptr);
	~LoginWindow();

protected:
	void onLoginClicked(); // slot for managing the click on the login button
	void onRegisterClicked(); // slot for managing the click on the registration button
private:
	QPushButton* loginButton = nullptr; // Login button
	QPushButton* registerButton = nullptr; // Registration button
	QPushButton* cancelButton = nullptr;// Cancel button
	QLineEdit* usernameLineEdit = nullptr; // Username field
	QLabel* messageLabel = nullptr; // Label for feedback messages
};