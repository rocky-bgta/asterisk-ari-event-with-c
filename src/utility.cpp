#include <websocketpp/config/asio_no_tls_client.hpp>
#include <iostream>
#include <functional>
#include <json/value.h>
#include <tins/tins.h>
#include <locale>
#include <codecvt>
#include "utility.hpp"

using namespace Tins;



//#if defined(_WIN32) || defined(_WIN64)
//#include <winsock2.h>
//#include <iphlpapi.h>
//#pragma comment(lib, "iphlpapi.lib")
//#else
//#include <ifaddrs.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <cstring>
//#endif

//std::string getDefaultInterface() {
//#if defined(_WIN32) || defined(_WIN64)
//    char buffer[512];
//    DWORD buffer_size = sizeof(buffer);
//    if (GetBestInterface(0, (DWORD*)buffer) == NO_ERROR) {
//        IP_ADAPTER_INFO adapter_info[16];
//        DWORD buf_len = sizeof(adapter_info);
//        DWORD status = GetAdaptersInfo(adapter_info, &buf_len);
//        if (status == ERROR_SUCCESS) {
//            PIP_ADAPTER_INFO pAdapterInfo = adapter_info;
//            while (pAdapterInfo) {
//                if (strcmp(pAdapterInfo->IpAddressList.IpAddress.String, "0.0.0.0") != 0) {
//                    return std::string(pAdapterInfo->AdapterName);
//                }
//                pAdapterInfo = pAdapterInfo->Next;
//            }
//        }
//    }
//    return "";
//#else
//    struct ifaddrs* ifaddr, * ifa;
//    int family;
//    std::string interface;
//
//    if (getifaddrs(&ifaddr) == -1) {
//        perror("getifaddrs");
//        return "";
//    }
//
//    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
//        if (ifa->ifa_addr == nullptr)
//            continue;
//
//        family = ifa->ifa_addr->sa_family;
//
//        if (family == AF_INET) { // IPv4
//            // Here you might want to add additional logic to select the appropriate interface
//            if (std::string(ifa->ifa_name) != "lo") { // Skip the loopback interface
//                interface = ifa->ifa_name;
//                break;
//            }
//        }
//    }
//
//    freeifaddrs(ifaddr);
//    return interface;
//#endif
//}

std::string wstring_to_string(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}




std::string getDefaultInterface() {
    try {
        // Detect available network interfaces
        std::vector<NetworkInterface> interfaces = NetworkInterface::all();
        if (interfaces.empty()) {
            std::cerr << "No network interfaces found!" << std::endl;
            return "";
        }

        // Print available interfaces and select the first one
        std::cout << "Available interfaces:" << std::endl;
        for (const auto& iface : interfaces) {
            std::cout << iface.name() << ": " << wstring_to_string(iface.friendly_name()) << std::endl;
        }

        //Default interface of my computer
        NetworkInterface iface = interfaces.front();
        std::cout << "Selected interface: " << iface.name() << std::endl;

        return iface.name();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return "";
}
