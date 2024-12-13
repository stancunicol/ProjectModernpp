#pragma once

#include <string>
#include <QDebug>
#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>

void SendLevelToServer(int level);
void SendRegisterToServer(const std::string& username);
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response);
std::string GetServerData(const std::string& url);
void PostServerData(const std::string& url, const std::string& jsonPayload);
bool CheckServerCode(const std::string& url);
std::string GetPlayerDataByIdFromServer(int playerId);
