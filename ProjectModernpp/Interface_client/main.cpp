#include <QApplication>
#include "Battle_city.h"
#include "GameMapInterface.h"
#include <QObject>

#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <thread>
#include <vector>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Battle_city* game = new Battle_city();

    std::vector<std::vector<int>> matrix;

    game->show();

    QObject::connect(game, &Battle_city::aboutToClose, [=]()
        {
        GameMapInterface* gameMap = new GameMapInterface();
        gameMap->show();
        });

    return app.exec();
}