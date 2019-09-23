#ifndef PACKET_GAINER_H
#define PACKET_GAINER_H
#include <pcap.h>

namespace lima {

class PacketGainer {

    public:
        PacketGainer() : mDeviceName(nullptr), mHandler(nullptr),
                        mSubnetMask(0), mIp(0) {}
        PacketGainer(char *device);
        // function to be called in pcap_loop
        void procces(u_char * args, const struct pcap_pkthdr * header, const u_char * packet);
        void start(); // starts packet sniffing and sending them to packet analyzer
        void stop();
        ~PacketGainer();
    
    private:
        char * mDeviceName = NULL;
        char mErrorBuff[PCAP_ERRBUF_SIZE];
        pcap_t * mHandler;
        char * mFilterExpression = "ip";
        struct bpf_program mCompiledPcap;
        bpf_u_int32 mSubnetMask;
        bpf_u_int32 mIp;

        bool mIsValid = false;
};

} // end lima namespace

#endif