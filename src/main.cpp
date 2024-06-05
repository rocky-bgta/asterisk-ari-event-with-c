#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <functional>
#include <json/value.h>
#include <json/reader.h>

#include "constants.hpp"
#include "ari_curl_utils.hpp"

typedef websocketpp::client<websocketpp::config::asio_client> client;

void on_message(client* c, websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg) {
    std::cout << "Received message: " << msg->get_payload() << std::endl;

    // Parse the JSON message
    Json::Value jsonData;
    Json::Reader reader;
    if (!reader.parse(msg->get_payload(), jsonData)) {
        std::cerr << "Failed to parse JSON message" << std::endl;
        return;
    }

    // Process the JSON data (e.g., log event type)
    std::string event_type = jsonData["type"].asString();
    std::cout << "Event Type: " << event_type << std::endl;

    // Extract other necessary information here
    if (event_type == "StasisStart") {
        std::string channel_id = jsonData["channel"]["id"].asString();
        std::string caller_number = jsonData["channel"]["caller"]["number"].asString();
        std::cout << "Incoming call from: " << caller_number << " on channel ID: " << channel_id << std::endl;

        // Example: Extracting target extension
        std::string target_extension = jsonData["channel"]["dialplan"]["exten"].asString();
        std::cout << "Target extension: " << target_extension << std::endl;

        // Start recording the call
        std::string recording_response = start_recording(Constants::SERVER_URL, channel_id, "call_recording", Constants::ARI_USERNAME, Constants::ARI_PASSWORD);
        std::cout << recording_response << std::endl;

        // Create a bridge
        std::string bridge_id = create_bridge(Constants::SERVER_URL, Constants::ARI_USERNAME, Constants::ARI_PASSWORD);
        std::cout << "Bridge created with ID: " << bridge_id << std::endl;

        std::string SIPInfo = "SIP/"+target_extension;

        // Create a new channel to the target extension (inactive)
        std::string new_channel_id = create_channel(Constants::SERVER_URL, SIPInfo, target_extension,
                                                    Constants::CONTEXT ,1, Constants::STASIS_APP,
                                                    Constants::ARI_USERNAME, Constants::ARI_PASSWORD);

        std::cout << "New channel created with ID: " << new_channel_id << std::endl;

        std::string active_channel_id = channel_id;

        // Add the active channel to the bridge
        add_channel_to_bridge(Constants::SERVER_URL, bridge_id, active_channel_id, Constants::ARI_USERNAME, Constants::ARI_PASSWORD);
        std::cout << "Active channel " << active_channel_id << " added to bridge " << bridge_id << std::endl;

        // Add the new channel to the bridge
        add_channel_to_bridge(Constants::SERVER_URL, bridge_id, new_channel_id, Constants::ARI_USERNAME, Constants::ARI_PASSWORD);
        std::cout << "New channel " << new_channel_id << " added to bridge " << bridge_id << std::endl;

        // Start dialing the new channel
        dial_channel(Constants::SERVER_URL, new_channel_id, Constants::ARI_USERNAME, Constants::ARI_PASSWORD);
        std::cout << "Dialing new channel " << new_channel_id << std::endl;
    }
    else if (event_type == "StasisEnd") {
        std::string channel_id = jsonData["channel"]["id"].asString();
        std::cout << "Channel ID: " << channel_id << " has left the Stasis application" << std::endl;
    }
}

void on_open(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "Connection opened" << std::endl;
}

void on_close(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "Connection closed" << std::endl;
}

void on_fail(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "Connection failed" << std::endl;
}

void run_ari_listener(const std::string& uri) {
    client c;

    c.set_access_channels(websocketpp::log::alevel::all);
    c.clear_access_channels(websocketpp::log::alevel::frame_payload);

    c.init_asio();

    c.set_message_handler(std::bind(&on_message, &c, std::placeholders::_1, std::placeholders::_2));
    c.set_open_handler(std::bind(&on_open, &c, std::placeholders::_1));
    c.set_close_handler(std::bind(&on_close, &c, std::placeholders::_1));
    c.set_fail_handler(std::bind(&on_fail, &c, std::placeholders::_1));

    websocketpp::lib::error_code ec;
    client::connection_ptr con = c.get_connection(uri, ec);
    if (ec) {
        std::cout << "Could not create connection because: " << ec.message() << std::endl;
        return;
    }

    c.connect(con);
    c.run();
}

int main() {
    std::string uri = "ws://192.168.0.132:8088/ari/events?api_key=asterisk:secret&app=my-stasis-app";
    run_ari_listener(uri);
    return 0;
}
