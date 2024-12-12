#include "ServerUtils.h"

void SendLevelToServer(int level) 
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) 
    {
        std::string url = "http://localhost:8080/submitLevel";
        std::string jsonPayload = "{\"level\": " + std::to_string(level) + "}";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) 
        {
            qDebug() << "Failed to send level: " << curl_easy_strerror(res);
        }
        else 
        {
            qDebug() << "Level sent successfully: " << level;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void SendRegisterToServer(const std::string& username) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        std::string url = "http://localhost:8080/register"; // Endpoint-ul serverului pentru înregistrare
        std::string jsonPayload = "{\"username\": \"" + username + "\"}";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            qDebug() << "Failed to send register data: " << curl_easy_strerror(res);
        }
        else {
            qDebug() << "Register data sent successfully for user: " << QString::fromStdString(username);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

// Funcție pentru recepționarea unui răspuns de la server
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response)
{
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

std::string GetServerData(const std::string& url) {

    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) 
        {
            qDebug() << "Failed to fetch data from server: " << curl_easy_strerror(res);
        }
        else
        {
            qDebug() << "Data fetched successfully from server: " << QString::fromStdString(response);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return response;
}

void PostServerData(const std::string& url, const std::string& jsonPayload)
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            qDebug() << "Failed to post data to server: " << curl_easy_strerror(res);
        }
        else
        {
            qDebug() << "Data posted successfully to server: " << QString::fromStdString(jsonPayload);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

}
