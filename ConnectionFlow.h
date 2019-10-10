#ifndef CONNECTIONFLOW_H
#define CONNECTIONFLOW_H

#include <string>
#include <pcapplusplus/IpAddress.h>
#include <pcapplusplus/MacAddress.h>
#include <pcapplusplus/Packet.h>
#include <memory>
#include <chrono>
#include <bitset>

#include <DnsInfo.hpp>

namespace lima {

// all information about srcIp - dstIp Connection
class ConnectionFlow {

    friend ConnectionFLowHasher;

    private:

        std::unique_ptr<pcpp::IPAddress> mSrcIp;        
        std::unique_ptr<pcpp::IPAddress> mDstIp;
        std::string mSrcMAC;
        std::string mDstMAC;

        std::chrono::steady_clock::time_point mStartTimeStamp;
        std::chrono::steady_clock::time_point mLastTimeStamp;
        DnsInfo mDnsInfo;
        bool mIsFinished;
        bool mIsValid;

        struct PacketCounter {
            uint64_t fromSrcToDst = 0;
            uint64_t fromDstToSrc = 0;
        };

        PacketCounter mPacketCounter;

        void update(pcpp::Packet & packet); // when ipsrc /ipdst are already in flowmap i just need to update info
        void checkFinishConditions(pcpp::Packet & packet);

    public:
    
        ConnectionFlow() = default;
        ~ConnectionFlow() = default;

        ConnectionFlow(pcpp::Packet & pakcet);
        void updateCounter(const pcpp::IPAddress & src, const pcpp::IPAddress & dst);
};

// TODO: write hasher
struct ConnectionFLowHasher {
    
    size_t operator()(const ConnectionFlow & node) const {

    }

};

}

#endif