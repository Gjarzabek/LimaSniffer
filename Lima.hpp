#ifndef LIMA_HPP
#define LIMA_HPP

#include "PacketGainer.h"
#include "ConnectionFlowWriter.h"
#include <unordered_map>
#include <utility>
#include "ConnectionFlow.h"
#include <mutex>
#include <list>
#include <thread>
#include <queue>

namespace lima {

class Lima {

    public:
        // po utworzeniu watkow ten watek czeka na sygnal od MapWritera zeby odebrać od niego pelną mape Flowow
        // i przekazac do zapisu to db/maila
        Lima() : mPacketSniffer("wlan0"), mFlowWriter(mCapturedPackets, mCapturedQueueMutex) {
            mCapturer = std::thread(PacketGainer::start, &mPacketSniffer, &mCapturedPackets, &mCapturedQueueMutex);
            mConnectionMapWriter = std::thread(ConnectionFlowWriter::start, &mFlowWriter);
        }

        ~Lima() {
            // wyslac sygnal do wątkow/dzieci zeby sie zakonczyly, za pomoca condvariable
        }

    private:

        std::thread mCapturer;
        std::thread mConnectionMapWriter;

        PacketGainer mPacketSniffer;
        ConnectionFlowWriter mFlowWriter;

        std::mutex mCapturedQueueMutex;
        std::queue<pcpp::Packet> mCapturedPackets; // packetGainer stores packets here
        std::mutex mConMapMutex;
        // Connection Flow Writers takes packets from queue, makes ConnectionFlows from them and saves to ConnMap
        // (or mby make another queue for ConFlows to be writed to connmap)
        ConnectionFlowMap mConnectionMap;

};

}

#endif
