// rtp_capture.cpp
#include "rtp_capture.hpp"
#include <iostream>
#include <pcap.h>

#ifdef __linux__
#include <arpa/inet.h>
#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

// Ethernet header is always 14 bytes
#define SIZE_ETHERNET 14

// IP header structure
struct sniff_ip {
    uint8_t ip_vhl;  // version << 4 | header length >> 2
    uint8_t ip_tos;  // type of service
    uint16_t ip_len; // total length
    uint16_t ip_id;  // identification
    uint16_t ip_off; // fragment offset field
    uint8_t ip_ttl;  // time to live
    uint8_t ip_p;    // protocol
    uint16_t ip_sum; // checksum
    struct in_addr ip_src, ip_dst; // source and dest address
};

#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip) (((ip)->ip_vhl) >> 4)

// UDP header structure
struct sniff_udp {
    uint16_t sport;    // Source port
    uint16_t dport;    // Destination port
    uint16_t len;      // Length
    uint16_t crc;      // Checksum
};

// Function to process packets
void packetHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    const struct sniff_ip* ip;
    const struct sniff_udp* udp;
    const char* payload;

    int size_ip;
    int size_udp;

    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip) * 4;
    if (size_ip < 20) {
        std::cerr << "Invalid IP header length: " << size_ip << " bytes" << std::endl;
        return;
    }

    udp = (struct sniff_udp*)(packet + SIZE_ETHERNET + size_ip);
    size_udp = 8;  // UDP header length is always 8 bytes

    payload = (char*)(packet + SIZE_ETHERNET + size_ip + size_udp);

    // Print packet details
    std::cout << "Packet captured: length = " << pkthdr->len << std::endl;
    std::cout << "Source IP: " << inet_ntoa(ip->ip_src) << std::endl;
    std::cout << "Destination IP: " << inet_ntoa(ip->ip_dst) << std::endl;
    std::cout << "Source port: " << ntohs(udp->sport) << std::endl;
    std::cout << "Destination port: " << ntohs(udp->dport) << std::endl;
    std::cout << "Payload: " << payload << std::endl;

    // Further processing can be done here, such as extracting RTP payload
}

// Function to start capturing RTP packets
void startRtpCapture(const std::string& iface, const std::string& filter_exp) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle;

    // Open the session in promiscuous mode
    handle = pcap_open_live(iface.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "Couldn't open device " << iface << ": " << errbuf << std::endl;
        return;
    }

    struct bpf_program fp;
    bpf_u_int32 net = 0;  // This can be set to the correct netmask if known
    if (pcap_compile(handle, &fp, filter_exp.c_str(), 0, net) == -1) {
        std::cerr << "Couldn't parse filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        return;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        std::cerr << "Couldn't install filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        return;
    }

    // Capture packets
    pcap_loop(handle, 0, packetHandler, nullptr);

    pcap_close(handle);
}
