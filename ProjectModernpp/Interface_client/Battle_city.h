#pragma once

#include <QMainWindow>//standard main window for GUI applications
#include <QLabel>//to display images or text
#include <QPushButton>//create buttons
#include <QPixmap>//to manage and manipulate images
#include <QDebug>//debug in console
#include <QVBoxLayout>//for vertical layout
#include <QMessageBox>//to create dialogs that can inform the user or reguest a decision
#include <QFont>//font buttons
#include <QHBoxLayout>//for horizontal layout
#include "LoginWindow.h"
#include "PlayWindow.h"
#include "ControlsWindow.h"
#include "ConnectWindow.h"
#include "LevelsWindow.h"

class Battle_city : public QMainWindow
{
	Q_OBJECT

public:
	Battle_city(QWidget * parent = nullptr);
	~Battle_city();

protected:
	void OnPlayButtonClicked();
	void OnConnectButtonClicked();
	void OnControlsButtonClicked();

private:
	QLabel* m_background = nullptr;//Background picture
	QPushButton* m_playButton = nullptr;//Button for Play
	QPushButton* m_connectButton = nullptr;//Button for Connect
	QPushButton* m_controlsButton = nullptr;//Button for Controls
	bool m_connectLogin = false;
};
