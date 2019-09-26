#ifndef LIMA_HPP
#define LIMA_HPP

#include "PacketGainer.h"
#include <unordered_map>
#include <utility>
#include "Connection.h"
#include <mutex>
#include <list>
#include <thread>


namespace lima {

class Lima {
    public:
        Lima() : mPacketSniffer("wlx503eaab3182f") {
            mPacketSniffer.start();
        }   
        ~Lima() = default;
        void stop() {
            mPacketSniffer.stop();
        }
        // start 3 threads
        // stop 3 threads
        // app_config
    private:
        PacketGainer mPacketSniffer; // przekazuje warstwe aplikacji do packet_analyzera
        // key is string which is contcat of 2 Mac/ip Addresses[src,dst]
        std::mutex mConMapMutex;
        std::unordered_map<std::string, std::list<Connection>> mConnectionMap;
        // Packet analyzer
        // Packet exporter
        // container to store sniffedPacket's (self made data type for them)
        // container to store Data rdy to export
};      // mutex
        // condition variable

}

#endif