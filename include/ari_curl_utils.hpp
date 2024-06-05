//
// Created by Salahin on 6/3/2024.
//

#ifndef ARILISTENER_ARI_CURL_UTILS_HPP
#define ARILISTENER_ARI_CURL_UTILS_HPP
#include <string>

std::string create_bridge(const std::string& server_url, const std::string& user, const std::string& password);
std::string create_channel(const std::string& server_url, const std::string& endpoint, const std::string& extension, const std::string& context, int priority, const std::string& app, const std::string& user, const std::string& password);
std::string start_recording(const std::string& server_url, const std::string& channel_id, const std::string& filename, const std::string& username, const std::string& password);
void add_channel_to_bridge(const std::string& server_url, const std::string& bridge_id, const std::string& channel_id, const std::string& user, const std::string& password);
void dial_channel(const std::string& server_url, const std::string& channel_id, const std::string& user, const std::string& password);

#endif //ARILISTENER_ARI_CURL_UTILS_HPP
