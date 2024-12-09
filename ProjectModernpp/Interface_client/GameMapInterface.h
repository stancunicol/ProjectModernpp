#pragma once

#include <QMainWindow>

class GameMapInterface : public QMainWindow
{
	Q_OBJECT

public:
	GameMapInterface(QWidget *parent = nullptr);
	~GameMapInterface();

private:
};
