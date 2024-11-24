#pragma once

#include <QDialog>
#include <QPushButton>//create buttons
#include <QVBoxLayout>//for vertical layout

class LoginWindow : public QDialog
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

private:
	QPushButton* loginButton = nullptr; // Login button
	QPushButton* registerButton = nullptr; // Registration button
	QPushButton* cancelButton = nullptr;// Cancel button
};
