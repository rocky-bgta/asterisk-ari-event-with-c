#include "utility.hpp"

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#else
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#endif

std::string getDefaultInterface() {
#if defined(_WIN32) || defined(_WIN64)
    char buffer[512];
    DWORD buffer_size = sizeof(buffer);
    if (GetBestInterface(0, (DWORD*)buffer) == NO_ERROR) {
        IP_ADAPTER_INFO adapter_info[16];
        DWORD buf_len = sizeof(adapter_info);
        DWORD status = GetAdaptersInfo(adapter_info, &buf_len);
        if (status == ERROR_SUCCESS) {
            PIP_ADAPTER_INFO pAdapterInfo = adapter_info;
            while (pAdapterInfo) {
                if (strcmp(pAdapterInfo->IpAddressList.IpAddress.String, "0.0.0.0") != 0) {
                    return std::string(pAdapterInfo->AdapterName);
                }
                pAdapterInfo = pAdapterInfo->Next;
            }
        }
    }
    return "";
#else
    struct ifaddrs* ifaddr, * ifa;
    int family;
    std::string interface;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return "";
    }

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr)
            continue;

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) { // IPv4
            // Here you might want to add additional logic to select the appropriate interface
            if (std::string(ifa->ifa_name) != "lo") { // Skip the loopback interface
                interface = ifa->ifa_name;
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
    return interface;
#endif
}
