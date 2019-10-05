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
        Lima() : mPacketSniffer("wlan0") {
            mCapturer = std::thread(PacketGainer::start, &mPacketSniffer, &mCapturedPackets, &mCapturedQueueMutex);
            mFlowWriter = std::thread();
        }

        ~Lima() {
            // wyslac sygnal do wątkow/dzieci zeby sie zakonczyly, za pomoca condvariable
        }

    private:
        std::thread mCapturer;
        std::thread mFlowWriter;

        PacketGainer mPacketSniffer;
        std::mutex mCapturedQueueMutex;
        std::queue<pcpp::Packet> mCapturedPackets;
        std::mutex mConMapMutex;
        std::unordered_map<std::string, std::list<ConnectionFlow>> mConnectionMap;

};

}

#endif