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
	playButton = new QPushButton("Play", this);//Play button
	connectButton = new QPushButton("Connect", this);//Connect button
	controlsButton = new QPushButton("Controls", this);//Controls button

	playButton->setFixedSize(270, 40);
	connectButton->setFixedSize(270, 40);
	controlsButton->setFixedSize(270, 40);

	QFont buttonFont("Arial", 12, QFont::Bold);
	QFont buttonFontPlay("Arial", 18, QFont::Bold);
	playButton->setFont(buttonFontPlay);
	connectButton->setFont(buttonFont);
	controlsButton->setFont(buttonFont);

	playButton->setStyleSheet("background-color: rgba(50, 50, 50, 0.7); color: white;border: 1px solid black;border - radius: 5px;");
	connectButton->setStyleSheet("background-color: rgba(50, 50, 50, 0.7);color: white;border: 1px solid black;border - radius: 5px;");
	controlsButton->setStyleSheet("background-color: rgba(50, 50, 50, 0.7);color: white;border: 1px solid black;border - radius: 5px;");
	layout->addWidget(playButton);
	layout->addWidget(connectButton);
	layout->addWidget(controlsButton);

	connect(playButton, &QPushButton::clicked, this, &Battle_city::onPlayButtonClicked);//connect the button to His use
	connect(connectButton, &QPushButton::clicked, this, &Battle_city::onConnectButtonClicked);
	connect(controlsButton, &QPushButton::clicked, this, &Battle_city::onControlsButtonClicked);
}

Battle_city::~Battle_city()
{}

void Battle_city::onPlayButtonClicked()
{
	QMessageBox::information(this, "Play", "The Play button has been pressed!");
}

void Battle_city::onConnectButtonClicked()
{
	QMessageBox::information(this, "Connect", "The connect button has been pressed!");
}

void Battle_city::onControlsButtonClicked()
{
	QMessageBox::information(this, "Controls", "The controls button has been pressed!");
}