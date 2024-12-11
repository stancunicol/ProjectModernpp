#pragma once

#include <string>
#include <QDebug>
#include <curl/curl.h>
#include <string>

void SendLevelToServer(int level);
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response);
std::string GetServerData(const std::string& url);
