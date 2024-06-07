//#include <pcap.h>
//#include <iostream>
//#include <stdexcept>
//#include <winsock2.h>
//#include <iphlpapi.h>
//
//// Function to list and select network interfaces
//void listAndSelectNetworkInterfaces(pcap_if_t*& alldevs, pcap_if_t*& selectedDev) {
//    char errbuf[PCAP_ERRBUF_SIZE];
//    int i = 0;
//
//    // Retrieve the device list on the local machine
//    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
//        std::cerr << "Error in pcap_findalldevs: " << errbuf << std::endl;
//        throw std::runtime_error("Failed to find network devices");
//    }
//
//    // Print the list
//    for (pcap_if_t* d = alldevs; d != nullptr; d = d->next) {
//        std::cout << ++i << ". " << (d->name ? d->name : "Unnamed") << std::endl;
//        if (d->description) {
//            std::cout << "   Description: " << d->description << std::endl;
//        }
//        else {
//            std::cout << "   No description available" << std::endl;
//        }
//    }
//
//    if (i == 0) {
//        std::cerr << "No interfaces found! Make sure Npcap is installed." << std::endl;
//        pcap_freealldevs(alldevs);
//        throw std::runtime_error("No network interfaces found");
//    }
//
//    std::cout << "Enter the interface number (1-" << i << "): ";
//    int inum;
//    std::cin >> inum;
//
//    if (inum < 1 || inum > i) {
//        std::cerr << "Interface number out of range." << std::endl;
//        pcap_freealldevs(alldevs);
//        throw std::runtime_error("Selected interface number out of range");
//    }
//
//    // Select the desired interface
//    for (selectedDev = alldevs, i = 0; i < inum - 1; selectedDev = selectedDev->next, i++);
//}
//
//// Packet handler function
//void packetHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
//    std::cout << "Packet captured with length: " << pkthdr->len << std::endl;
//}
//
//int main() {
//    pcap_if_t* alldevs = nullptr; // Network devices list
//    pcap_if_t* selectedDev = nullptr; // Selected device
//    char errbuf[PCAP_ERRBUF_SIZE];
//
//    try {
//        // List and select network interfaces
//        listAndSelectNetworkInterfaces(alldevs, selectedDev);
//
//        std::cout << "Selected network interface: " << (selectedDev->name ? selectedDev->name : "Unnamed") << std::endl;
//
//        // Open the device for capturing
//        pcap_t* adhandle = pcap_open_live(selectedDev->name, BUFSIZ, 1, 1000, errbuf);
//        if (adhandle == nullptr) {
//            std::cerr << "Couldn't open device: " << selectedDev->name << " - " << errbuf << std::endl;
//            pcap_freealldevs(alldevs);
//            return 1;
//        }
//
//        // Retrieve the netmask of the device
//        bpf_u_int32 net;
//        bpf_u_int32 mask;
//        if (pcap_lookupnet(selectedDev->name, &net, &mask, errbuf) == -1) {
//            std::cerr << "Couldn't get netmask for device: " << selectedDev->name << " - " << errbuf << std::endl;
//            net = 0;
//            mask = 0;
//        }
//
//        // Compile and apply the filter
//        struct bpf_program fp;
//        const char* filter_exp = "udp"; // Filter expression to capture only UDP packets
//        if (pcap_compile(adhandle, &fp, filter_exp, 0, net) == -1) {
//            std::cerr << "Couldn't parse filter " << filter_exp << " - " << pcap_geterr(adhandle) << std::endl;
//            pcap_freealldevs(alldevs);
//            pcap_close(adhandle);
//            return 1;
//        }
//        if (pcap_setfilter(adhandle, &fp) == -1) {
//            std::cerr << "Couldn't install filter " << filter_exp << " - " << pcap_geterr(adhandle) << std::endl;
//            pcap_freealldevs(alldevs);
//            pcap_close(adhandle);
//            return 1;
//        }
//
//        std::cout << "Listening on " << selectedDev->name << "..." << std::endl;
//
//        // Start packet capture loop
//        pcap_loop(adhandle, 10, packetHandler, nullptr);
//
//        // Cleanup
//        pcap_freecode(&fp);
//        pcap_close(adhandle);
//        pcap_freealldevs(alldevs);
//    }
//    catch (const std::runtime_error& e) {
//        std::cerr << e.what() << std::endl;
//        if (alldevs) {
//            pcap_freealldevs(alldevs);
//        }
//        return 1;
//    }
//
//    return 0;
//}
