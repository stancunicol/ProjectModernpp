#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QDebug>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <QHBoxLayout>
#include "LoginWindow.h"
#include "PlayWindow.h"
#include "ControlsWindow.h"
#include "ConnectWindow.h"
#include "LevelsWindow.h"
#include "CustomToolBar.h"

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

signals:
	void aboutToClose();

private:
	QLabel* m_background = nullptr;
	QPushButton* m_playButton = nullptr;
	QPushButton* m_connectButton = nullptr;
	QPushButton* m_controlsButton = nullptr;
	bool m_connectLogin = false;
};
