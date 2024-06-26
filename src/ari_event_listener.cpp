//#include <websocketpp/config/asio_no_tls_client.hpp>
//#include <websocketpp/client.hpp>
//#include <iostream>
//#include <functional>
//#include <json/value.h>
//#include <json/reader.h>
//#include <pcap.h>
//
//// Function prototype
//void startRtpCapture(const std::string& iface, const std::string& filter_exp);
//std::string getDefaultInterface();
//
//typedef websocketpp::client<websocketpp::config::asio_client> client;
//
//void on_message(client* c, websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg) {
//    std::cout << "Received message: " << msg->get_payload() << std::endl;
//
//    // Parse the JSON message
//    Json::Value jsonData;
//    Json::Reader reader;
//    if (!reader.parse(msg->get_payload(), jsonData)) {
//        std::cerr << "Failed to parse JSON message" << std::endl;
//        return;
//    }
//
//    // Process the JSON data (e.g., log event type)
//    std::string event_type = jsonData["type"].asString();
//    std::cout << "Event Type: " << event_type << std::endl;
//
//    // Extract other necessary information here
//    if (event_type == "StasisStart") {
//        std::string channel_id = jsonData["channel"]["id"].asString();
//        std::string caller_number = jsonData["channel"]["caller"]["number"].asString();
//        std::cout << "Incoming call from: " << caller_number << " on channel ID: " << channel_id << std::endl;
//
//        // Example: Extracting target extension
//        std::string target_extension = jsonData["channel"]["dialplan"]["exten"].asString();
//        std::cout << "Target extension: " << target_extension << std::endl;
//
//        // Assuming we get IP and port details from another part of the message
//        std::string src_ip = jsonData["channel"]["connected"]["address"].asString();
//        int src_port = jsonData["channel"]["connected"]["port"].asInt();
//
//        std::string filter_exp = "udp and src host " + src_ip + " and src port " + std::to_string(src_port);
//        std::cout << "Filter Expression: " << filter_exp << std::endl;
//
//        // Determine the default network interface
//        std::string iface = getDefaultInterface();
//        if (iface.empty()) {
//            std::cerr << "Could not determine default network interface" << std::endl;
//            return;
//        }
//
//        // Start capturing RTP packets
//        startRtpCapture(iface, filter_exp);
//    }
//    else if (event_type == "StasisEnd") {
//        std::string channel_id = jsonData["channel"]["id"].asString();
//        std::cout << "Channel ID: " << channel_id << " has left the Stasis application" << std::endl;
//    }
//}
//
//void on_open(client* c, websocketpp::connection_hdl hdl) {
//    std::cout << "Connection opened" << std::endl;
//}
//
//void on_close(client* c, websocketpp::connection_hdl hdl) {
//    std::cout << "Connection closed" << std::endl;
//}
//
//void on_fail(client* c, websocketpp::connection_hdl hdl) {
//    std::cout << "Connection failed" << std::endl;
//}
//
//void run_ari_listener(const std::string& uri) {
//    client c;
//
//    c.set_access_channels(websocketpp::log::alevel::all);
//    c.clear_access_channels(websocketpp::log::alevel::frame_payload);
//
//    c.init_asio();
//
//    c.set_message_handler(std::bind(&on_message, &c, std::placeholders::_1, std::placeholders::_2));
//    c.set_open_handler(std::bind(&on_open, &c, std::placeholders::_1));
//    c.set_close_handler(std::bind(&on_close, &c, std::placeholders::_1));
//    c.set_fail_handler(std::bind(&on_fail, &c, std::placeholders::_1));
//
//    websocketpp::lib::error_code ec;
//    client::connection_ptr con = c.get_connection(uri, ec);
//    if (ec) {
//        std::cout << "Could not create connection because: " << ec.message() << std::endl;
//        return;
//    }
//
//    c.connect(con);
//    c.run();
//}
//
//int main() {
//    std::string uri = "ws://192.168.0.132:8088/ari/events?api_key=asterisk:secret&app=my-stasis-app";
//    run_ari_listener(uri);
//    return 0;
//}
