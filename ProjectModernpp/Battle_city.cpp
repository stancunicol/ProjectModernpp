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
	setCentralWidget(background);//Set the QLabel as the center widget
}

Battle_city::~Battle_city()
{}

void Battle_city::onPlayButtonClicked()
{
}

void Battle_city::onConnectButtonClicked()
{
}

void Battle_city::onControlsButtonClicked()
{
}
