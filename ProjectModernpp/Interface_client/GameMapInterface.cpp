#include "GameMapInterface.h"

GameMapInterface::GameMapInterface(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("Game Map");
	setFixedSize(600, 600);
}

GameMapInterface::~GameMapInterface()
{

}
