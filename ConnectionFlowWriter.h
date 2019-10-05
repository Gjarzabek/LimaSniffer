#ifndef CONNECTION_FLOW_WRITER_H
#define CONNECTION_FLOW_WRITER_H

#include "ConnectionFlow.h"
#include <pcap.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/RawPacket.h>
#include <queue>
#include <mutex>
#include <memory>

namespace lima {

using PacketQueue = std::queue<pcpp::Packet>;

class ConnectionFlowWriter {
    
    public:
        ConnectionFlowWriter();
        ~ConnectionFlowWriter();

        void start();
        void stop();

    private:

};

}

#endif
