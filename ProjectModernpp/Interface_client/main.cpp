#include <QApplication>
#include "Battle_city.h"
#include "GameMapInterface.h"
#include <QObject>

#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <thread>

using json = nlohmann::json;

size_t WriteCallBack(void* content, size_t size, size_t memb, void* user) 
{
    size_t totalSize = size * memb;
    ((std::string*)user)->append((char*)content, totalSize);
    return totalSize;
}

void FetchGameState() 
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) 
    {
        std::string readBuffer;

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/game");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) 
        {
            qDebug() << "CURL error: " << curl_easy_strerror(res);
        }
        else 
        {
            try 
            {
                json jsonResponse = json::parse(readBuffer);

                int level = jsonResponse.value("level", -1); 
                int rows = jsonResponse.value("rows", -1);
                int columns = jsonResponse.value("columns", -1);

                qDebug() << "Level:" << level;
                qDebug() << "Rows:" << rows;
                qDebug() << "Columns:" << columns;

                if (jsonResponse.contains("matrix")) 
                {
                    auto matrix = jsonResponse["matrix"];

                    qDebug() << "Matrix:";
                    for (const auto& row : matrix) 
                    {
                        QString rowString;
                        for (const auto& cell : row)
                        {
                            rowString += QString::number(cell.get<int>()) + " "; // Convertește valorile la întregi și le concatenează
                        }
                        rowString.chop(1);
                        rowString += "]";
                        qDebug() << rowString;
                    }
                }
                else 
                {
                    qDebug() << "Matrix not found in response.";
                }
            }
            catch (const json::parse_error& e) 
            {
                qDebug() << "JSON Parsing error: " << e.what();
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Battle_city* game = new Battle_city();
    GameMapInterface* gameMap = new GameMapInterface();
    
    std::thread fetchThread([]() 
        {
        FetchGameState();
        });

    fetchThread.join();

    game->show();

    QObject::connect(game, &Battle_city::aboutToClose, [=]()
        {
        gameMap->show();
        });

    return app.exec();
}