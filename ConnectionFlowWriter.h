#ifndef CONNECTION_FLOW_WRITER_H
#define CONNECTION_FLOW_WRITER_H

#include "ConnectionFlow.h"
#include <pcap.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/RawPacket.h>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <memory>



namespace lima {

using PacketQueue = std::queue<pcpp::Packet>;

class ConnectionFlowWriter {
    
    static constexpr uint16_t MAX_MAP_SIZE = 10000;

    public:
        ConnectionFlowWriter(PacketQueue & capturedPackets, std::mutex & queueMutex) :
            mReceivePackets(capturedPackets), mReceicePacketsMutex(queueMutex) {}

        ~ConnectionFlowWriter();

        void start();
        void stop();

        bool isQueueReady() const {
            return mReceivePackets.size() > 0;
        }


    private:

        void packetProcess();

        PacketQueue & mReceivePackets; // here PacketGainer Writes Packets
        std::mutex & mReceicePacketsMutex;

        std::condition_variable mReceiveCondVar;
        // dla zwiększenia wydajności - podzielic kolejkę na N czesci
        // i przetwarzac rownolegle za pomoca N watkow 
        PacketQueue mProcessPackets; // here are Packets to process , moved from mReceivePackets

        ConnectionFlowMap mCurrentFlows;
        ConnectionFlowMap mFlowsToExport;

        bool mWork;
};

}

#endif
