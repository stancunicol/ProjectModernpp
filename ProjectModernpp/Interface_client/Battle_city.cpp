#include "Battle_city.h"

Battle_city::Battle_city(QWidget* parent)
	: QMainWindow(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);

	CustomTitleBar* titleBar = new CustomTitleBar(this);
	setMenuWidget(titleBar);  

	QPixmap pixmap1("./logo_tank2.jpg");
	titleBar->SetIcon(pixmap1);  
	titleBar->SetImage("./include_girls_logo.jpg");

	QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	resize(1200, 700);
	m_background = new QLabel(this);
	QPixmap pixmap("./Battle_city.jpg");
	if (pixmap.isNull())
	{
		qDebug() << "The image couldn't be loaded!";
	}
	m_background->setPixmap(pixmap);
	m_background->setScaledContents(true);
	m_background->setGeometry(this->rect());
	setCentralWidget(m_background);

	QVBoxLayout* layout = new QVBoxLayout(m_background);
	QHBoxLayout* otherLayout = new QHBoxLayout;

	m_playButton = new QPushButton("Play", this);
	m_connectButton = new QPushButton("Connect", this);
	m_controlsButton = new QPushButton("Controls", this);

	m_playButton->setFixedSize(320, 55);
	m_connectButton->setFixedSize(270, 40);
	m_controlsButton->setFixedSize(270, 40);

	QFont buttonFont("Arial", 12, QFont::Bold);
	QFont buttonFontPlay("Arial", 18, QFont::Bold);
	m_playButton->setFont(buttonFontPlay);
	m_connectButton->setFont(buttonFont);
	m_controlsButton->setFont(buttonFont);

	m_playButton->setStyleSheet(
		"QPushButton {"
		"background-color: rgba(50, 50, 50, 0.7);"
		"color: white;"
		"border: 1px solid black;"
		"border - radius: 5px;"
		"}"
		"QPushButton:hover {"
		"background-color: rgba(30, 30, 30, 0.7);"
		"color: rgba(255, 255, 0,0.7);"
		"}"
		"QPushButton:pressed {"
		"background-color: rgba(20, 20, 20, 0.7);"
		"color: rgba(0, 0, 0,0.5);"
		"}");

	m_connectButton->setStyleSheet(
		"QPushButton {"
		"background-color: rgba(50, 50, 50, 0.7);"
		"color: white;"
		"border: 1px solid black;"
		"border - radius: 5px;"
		"}"
		"QPushButton:hover {"
		"background-color: rgba(30, 30, 30, 0.7);"
		"color: rgba(255, 255, 0,0.7);"
		"}"
		"QPushButton:pressed {"
		"background-color: rgba(20, 20, 20, 0.7);"
		"color: rgba(0, 0, 0,0.5);"
		"}");

	m_controlsButton->setStyleSheet(
		"QPushButton {"
		"background-color: rgba(50, 50, 50, 0.7);"
		"color: white;"
		"border: 1px solid black;"
		"border - radius: 5px;"
		"}"
		"QPushButton:hover {"
		"background-color: rgba(30, 30, 30, 0.7);"
		"color: rgba(255, 255, 0,0.7);"
		"}"
		"QPushButton:pressed {"
		"background-color: rgba(20, 20, 20, 0.7);"
		"color: rgba(0, 0, 0,0.5);"
		"}");

	layout->addSpacerItem(new QSpacerItem(0, 200, QSizePolicy::Minimum, QSizePolicy::Fixed));
	layout->addWidget(m_playButton, 0, Qt::AlignCenter);

	otherLayout->addWidget(m_connectButton);
	otherLayout->addSpacerItem(new QSpacerItem(350, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

	otherLayout->addWidget(m_controlsButton);

	otherLayout->setAlignment(Qt::AlignBottom | Qt::AlignCenter);

	layout->addLayout(otherLayout);

	connect(m_playButton, &QPushButton::clicked, this, &Battle_city::OnPlayButtonClicked);
	connect(m_connectButton, &QPushButton::clicked, this, &Battle_city::OnConnectButtonClicked);
	connect(m_controlsButton, &QPushButton::clicked, this, &Battle_city::OnControlsButtonClicked);
}

Battle_city::~Battle_city()
{

}

void Battle_city::OnPlayButtonClicked()
{
	while (m_connectLogin == false)
	{
		LoginWindow* login = new LoginWindow(this);
		size_t result = login->exec();
		if (result == QDialog::Accepted)
		{
			m_connectLogin = true;
		}
		else
			QMessageBox::information(this, "Account", "You must login/register to continue");
	}
	LevelsWindow* level = new LevelsWindow(this);
	if (level->exec() == QDialog::Accepted)
	{
		level->close();
		PlayWindow* playWindow = new PlayWindow(this);
		if (playWindow->exec() == QDialog::Accepted)
		{
			playWindow->close();
			emit aboutToClose();
			Battle_city::close();
		}
	}
}

void Battle_city::OnConnectButtonClicked()
{
	while (m_connectLogin == false)
	{
		LoginWindow* login = new LoginWindow(this);
		int result = login->exec();
		if (result == QDialog::Accepted)
		{
			m_connectLogin = true;
		}
		else
			QMessageBox::information(this, "Account", "You must login/register to continue");
	}
	ConnectWindow* connectWindow = new ConnectWindow(this);
	if (connectWindow->exec() == QDialog::Accepted)
	{
		connectWindow->close();
		emit aboutToClose();
		Battle_city::close();
	}
}

void Battle_city::OnControlsButtonClicked()
{
	ControlsWindow* controls = new ControlsWindow(this);
	controls->exec();
}