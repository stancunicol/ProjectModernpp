#include <QApplication>
#include "Battle_city.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Battle_city game;
    game.show();
    return app.exec();
}