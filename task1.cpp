#include <pcap.h>
#include <sqlite3.h>
#include <iostream>
#include "framwork.h"
#include <netinet/ip.h> 

// ...

void processPacket(const u_char* packet, const pcap_pkthdr& packet_header, sqlite3* db) {
    
    const struct ip* ipHeader = (struct ip*)(packet);

    
    std::string srcIP = inet_ntoa(ipHeader->ip_src);
    std::string dstIP = inet_ntoa(ipHeader->ip_dst);

    // Insert 
    insert_data(db, srcIP, dstIP, "CallID");  // Replace CallID
}

int main() {
    // Initi
    sqlite3* db;
    if (!initialize_db(db)) {
        return 1; // Exit with  error 
    }

    std::string inputPcapFile = "sip-rtp-g729a.pcap";
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* handle = pcap_open_offline(inputPcapFile.c_str(), errbuf);
    if (!handle) {
        std::cerr << "Error opening pcap file: " << errbuf << std::endl;
        close_db(db);
        return 1;
    }

    pcap_pkthdr packet_header;
    const u_char* packet_data;

    while ((packet_data = pcap_next(handle, &packet_header))) {
        
        processPacket(packet_data, packet_header, db);
    }

    
    close_db(db);

    std::cout << "Packets processed and written to the database." << std::endl;
    return 0; 
