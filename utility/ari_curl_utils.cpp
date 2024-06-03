//
// Created by Salahin on 6/3/2024.
//
#include <curl/curl.h>
#include <json/json.h>
#include <iostream>
#include <sstream>
//#include "ari_curl_utils.hpp"

// Helper function to perform HTTP POST request
std::string http_post(const std::string& url, const std::string& data, const std::string& user, const std::string& password) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_USERNAME, user.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

        // Set up the response capture
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void* contents, size_t size, size_t nmemb, std::string* s) -> size_t {
            size_t totalSize = size * nmemb;
            s->append(static_cast<char*>(contents), totalSize);
            return totalSize;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return response;
}

std::string create_bridge(const std::string& server_url, const std::string& user, const std::string& password) {
    std::string url = server_url + "/ari/bridges";
    std::string data = "{\"type\":\"mixing\"}";
    std::string response = http_post(url, data, user, password);


    Json::Value jsonData;
    std::istringstream(response) >> jsonData;
    return jsonData["id"].asString();
}

std::string create_channel(const std::string& server_url, const std::string& endpoint, const std::string& extension, const std::string& context, int priority, const std::string& app, const std::string& user, const std::string& password) {
    std::string url = server_url + "/ari/channels/create";
    Json::Value jsonData;
    jsonData["endpoint"] = endpoint;
    jsonData["extension"] = extension;
    jsonData["context"] = context;
    jsonData["priority"] = priority;
    jsonData["app"] = app;

    Json::FastWriter fastWriter;
    std::string data = fastWriter.write(jsonData);
    std::string response = http_post(url, data, user, password);

    std::istringstream(response) >> jsonData;
    return jsonData["id"].asString();
}

void add_channel_to_bridge(const std::string& server_url, const std::string& bridge_id, const std::string& channel_id, const std::string& user, const std::string& password) {
    std::string url = server_url + "/ari/bridges/" + bridge_id + "/addChannel";
    Json::Value jsonData;
    jsonData["channel"] = channel_id;

    Json::FastWriter fastWriter;
    std::string data = fastWriter.write(jsonData);
    http_post(url, data, user, password);
}

void dial_channel(const std::string& server_url, const std::string& channel_id, const std::string& user, const std::string& password) {
    std::string url = server_url + "/ari/channels/" + channel_id + "/dial";
    http_post(url, "", user, password);
}
