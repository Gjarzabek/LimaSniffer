#ifndef CONNECTIONFLOW_H
#define CONNECTIONFLOW_H

#include <string>
#include <pcapplusplus/IpAddress.h>
#include <pcapplusplus/MacAddress.h>
#include <pcapplusplus/Packet.h>
#include <memory>
#include <chrono>
#include <functional>

#include <DnsInfo.hpp>

namespace lima {

using IpPair = std::pair<std::string, std::string>;

// all information about srcIp - dstIp Connection
class ConnectionFlow {

    static constexpr uint8_t TIMEOUT = 15;

    friend struct ConnectionFLowHasher;

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

            PacketCounter(const PacketCounter & other) :
                fromDstToSrc(other.fromDstToSrc), fromSrcToDst(other.fromSrcToDst) {}
        };

        PacketCounter mPacketCounter;

        void updateCounter(const IpPair & SrcDstIp);
        
        void checkFinishConditions(pcpp::Packet & packet);

    public:
    
        ConnectionFlow() = default;
        ~ConnectionFlow() = default;

        ConnectionFlow(const ConnectionFlow & other);
        ConnectionFlow(pcpp::Packet & pakcet);

        const std::string & getSrcIp() {
            return mSrcIp->toString();
        }

        const std::string & getDstIp() {
            return mDstIp->toString();
        }

        void update(pcpp::Packet & packet, const IpPair & SrcDstIp); // when ipsrc /ipdst are already in flowmap i just need to update info
        bool isFinished() const {
            return mIsFinished;
        }
};

struct ConnectionFLowHasher {
    

    size_t operator()(const IpPair & ippair) const {
        size_t h1 = std::hash<std::string>{}(ippair.first);
        size_t h2 = std::hash<std::string>{}(ippair.second);
        return h1 + h2;
    }

};

using ConnectionFlowMap = std::unordered_map<IpPair, ConnectionFlow, ConnectionFLowHasher>;

}

#endif