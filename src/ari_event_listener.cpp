#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/asio.hpp>
#include <json/json.h>
#include <iostream>
#include <functional>

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
        std::string target_extension = jsonData["args"][0].asString();
        std::cout << "Target extension: " << target_extension << std::endl;
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
