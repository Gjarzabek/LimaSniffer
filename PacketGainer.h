#ifndef PACKET_GAINER_H
#define PACKET_GAINER_H
#include <pcap.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/RawPacket.h>
#include <queue>
#include <mutex>
#include <memory>

namespace lima {

using PacketQueue = std::queue<pcpp::Packet>;


class PacketGainer {

    struct CallbackPackage {
        CallbackPackage(PacketQueue * qp, std::mutex * pqueueMutex):
            packetQueue(qp), packetqueueMutex(pqueueMutex)  {}

        std::unique_ptr<PacketQueue> packetQueue;
        std::unique_ptr<std::mutex> packetqueueMutex;
    };
    
    public:
        PacketGainer() : mDeviceName(nullptr), mHandler(nullptr),
                        mSubnetMask(0), mIp(0) {}
        PacketGainer(std::string device);

        static void procces(u_char * args, const struct pcap_pkthdr * header, const u_char * packet);
        void start(std::queue<pcpp::Packet> * mCapturedPackets, std::mutex * CaputredPacketsMutex);
        void stop();
        ~PacketGainer();
    
    private:
        const char * mDeviceName;
        char mErrorBuff[PCAP_ERRBUF_SIZE];
        pcap_t * mHandler;
        char mFilterExpression[3] = "ip"; // TODO: take care of other packets also.
        struct bpf_program mCompiledPcap;
        bpf_u_int32 mSubnetMask;
        bpf_u_int32 mIp;
};

} // end lima namespace

#endif