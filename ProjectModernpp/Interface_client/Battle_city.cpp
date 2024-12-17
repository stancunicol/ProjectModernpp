#include "Battle_city.h"
#include "CustomToolBar.h"

Battle_city::Battle_city(QWidget* parent)
	: QMainWindow(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);//hide the default tool bar

	// create a custom title bar
	CustomTitleBar* titleBar = new CustomTitleBar(this);
	setMenuWidget(titleBar);  // add the bar as main widget of the window

	QPixmap pixmap1("./logo_tank2.jpg");
	titleBar->setIcon(pixmap1);  // set the icon
	titleBar->setImage("./include_girls_logo13.jpg");// set the logo image

	// create a widget for the rest of the window
	QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	resize(1200, 700);//window 1500X775
	m_background = new QLabel(this);
	QPixmap pixmap("./Battle_city.jpg");
	if (pixmap.isNull())
	{
		qDebug() << "The image couldn't be loaded!";
	}
	m_background->setPixmap(pixmap);//Set the image as a QPixmap for the QLabel
	m_background->setScaledContents(true);//Scale the image to occupy the entire widget
	m_background->setGeometry(this->rect());//Set the geometry of the QLabel to occupy the entire window
	setCentralWidget(m_background);//Set the QLabel as the centre widget

	QVBoxLayout* layout = new QVBoxLayout(m_background);//vertical layout
	QHBoxLayout* otherLayout = new QHBoxLayout;// horizontal layout

	m_playButton = new QPushButton("Play", this);//play button
	m_connectButton = new QPushButton("Connect", this);//connect button
	m_controlsButton = new QPushButton("Controls", this);//controls button

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
		"color: rgba(255, 255, 0,0.7);"//yellow
		"}"
		"QPushButton:pressed {"
		"background-color: rgba(20, 20, 20, 0.7);"
		"color: rgba(0, 0, 0,0.5);"//black
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
		"color: rgba(255, 255, 0,0.7);"//yellow
		"}"
		"QPushButton:pressed {"
		"background-color: rgba(20, 20, 20, 0.7);"
		"color: rgba(0, 0, 0,0.5);"//black
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
		"color: rgba(255, 255, 0,0.7);"//yellow
		"}"
		"QPushButton:pressed {"
		"background-color: rgba(20, 20, 20, 0.7);"
		"color: rgba(0, 0, 0,0.5);"//black
		"}");

	layout->addSpacerItem(new QSpacerItem(0, 200, QSizePolicy::Minimum, QSizePolicy::Fixed));//spacer
	/*   2: The width of the spacer in pixels

		 0: The height of the spacer in pixels

		 QSizePolicy::Minimum : This is the size policy on the width direction. Minimum indicates that the spacer will not try to expand, but will only occupy the specified width (which is 0 in this case).

		 QSizePolicy::Expanding: The vertical/horizontal size policy, which indicates that this spacer will occupy as much vertical space as possible.

		 *QSizePolicy::Fixed : This is the size policy in the direction of height/width. Fixed indicates that the height of the spacer will remain constant (10 pixels) and will not adapt to the size of the layout
	*/
	layout->addWidget(m_playButton, 0, Qt::AlignCenter);//0-place at the beggining of the layout, aligned in the center of te page

	otherLayout->addWidget(m_connectButton);
	otherLayout->addSpacerItem(new QSpacerItem(350, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));//spacer

	otherLayout->addWidget(m_controlsButton);

	otherLayout->setAlignment(Qt::AlignBottom | Qt::AlignCenter);// Center the buttons

	layout->addLayout(otherLayout);//combine the other layout in the main one

	connect(m_playButton, &QPushButton::clicked, this, &Battle_city::OnPlayButtonClicked);//connect the button to His use
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
		LoginWindow* login = new LoginWindow(this);//associates the login window with the current window
		size_t result = login->exec();//opens the login window as a modal dialog, meaning the user cannot interact with the parent window until the login window is closed
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