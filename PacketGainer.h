#ifndef PACKET_GAINER_H
#define PACKET_GAINER_H
#include <pcap.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/RawPacket.h>

namespace lima {

class PacketGainer {

    public:
        PacketGainer() : mDeviceName(nullptr), mHandler(nullptr),
                        mSubnetMask(0), mIp(0) {}
        PacketGainer(std::string device);
        // function to be called in pcap_loop
        static void procces(u_char * args, const struct pcap_pkthdr * header, const u_char * packet);
        void start(); // starts packet sniffing and sending them to packet analyzer
        void stop();
        ~PacketGainer();
    
    private:
        const char * mDeviceName;
        char mErrorBuff[PCAP_ERRBUF_SIZE];
        pcap_t * mHandler;
        char mFilterExpression[3] = "ip";
        struct bpf_program mCompiledPcap;
        bpf_u_int32 mSubnetMask;
        bpf_u_int32 mIp;

        bool mIsValid = false;
};

} // end lima namespace

#endif