#include "Battle_city.h"

Battle_city::Battle_city(QWidget* parent)
	: QMainWindow(parent)
{
	setWindowTitle("Battle City");//title
	resize(1200, 700);//window 1500X775
	background = new QLabel(this);
	QPixmap pixmap("./Battle_city.jpg");
	if (pixmap.isNull())
	{
		qDebug() << "The image couldn't be loaded!";
	}
	background->setPixmap(pixmap);//Set the image as a QPixmap for the QLabel
	background->setScaledContents(true);//Scale the image to occupy the entire widget
	background->setGeometry(this->rect());//Set the geometry of the QLabel to occupy the entire window
	setCentralWidget(background);//Set the QLabel as the centre widget

	QVBoxLayout* layout = new QVBoxLayout(background);//vertical layout
	QHBoxLayout* otherLayout = new QHBoxLayout;// horizontal layout

	playButton = new QPushButton("Play", this);//play button
	connectButton = new QPushButton("Connect", this);//connect button
	controlsButton = new QPushButton("Controls", this);//controls button

	playButton->setFixedSize(320, 55);
	connectButton->setFixedSize(270, 40);
	controlsButton->setFixedSize(270, 40);

	QFont buttonFont("Arial", 12, QFont::Bold);
	QFont buttonFontPlay("Arial", 18, QFont::Bold);
	playButton->setFont(buttonFontPlay);
	connectButton->setFont(buttonFont);
	controlsButton->setFont(buttonFont);

	playButton->setStyleSheet(
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

	connectButton->setStyleSheet(
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

	controlsButton->setStyleSheet(
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
	layout->addWidget(playButton, 0, Qt::AlignCenter);//0-place at the beggining of the layout, aligned in the center of te page

	otherLayout->addWidget(connectButton);
	otherLayout->addSpacerItem(new QSpacerItem(350, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));//spacer

	otherLayout->addWidget(controlsButton);

	otherLayout->setAlignment(Qt::AlignBottom | Qt::AlignCenter);// Center the buttons

	layout->addLayout(otherLayout);//combine the other layout in the main one

	connect(playButton, &QPushButton::clicked, this, &Battle_city::OnPlayButtonClicked);//connect the button to His use
	connect(connectButton, &QPushButton::clicked, this, &Battle_city::OnConnectButtonClicked);
	connect(controlsButton, &QPushButton::clicked, this, &Battle_city::OnControlsButtonClicked);
}

Battle_city::~Battle_city()
{

}

void Battle_city::OnPlayButtonClicked()
{
	LoginWindow* login = new LoginWindow(this);//associates the login window with the current window
	int result = login->exec();//opens the login window as a modal dialog, meaning the user cannot interact with the parent window until the login window is closed
	if (result == QDialog::Accepted)
	{
		PlayWindow* playWindow = new PlayWindow(this);
		playWindow->exec();
	}
}

void Battle_city::OnConnectButtonClicked()
{
	LoginWindow* login = new LoginWindow(this);
	int result = login->exec();
}

void Battle_city::OnControlsButtonClicked()
{
	ControlsWindow* controls = new ControlsWindow(this);
	controls->exec();
}