#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ControlsWindow : public QDialog
{
	Q_OBJECT

public:
	ControlsWindow(QWidget *parent = nullptr);
	~ControlsWindow();

private:
	QLabel* controlLabel;
	QPushButton* closeButton;
};
