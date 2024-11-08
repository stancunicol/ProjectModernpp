#pragma once

#include <QMainWindow>//standard main window for GUI applications
#include <QLabel>//to display images or text
#include <QPushButton>//create buttons

class Battle_city : public QMainWindow
{
	Q_OBJECT

public:
	Battle_city(QWidget *parent = nullptr);
	~Battle_city();
private:
	QLabel* background = nullptr;//Background picure
	QPushButton* playButton = nullptr;//Button for Play
	QPushButton* connectButton = nullptr;//Button for Connect
	QPushButton* controlsButton = nullptr;//Button for Controls
};
