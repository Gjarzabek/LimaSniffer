#include "PacketGainer.h"
#include "stdio.h"
#include <pcapplusplus/EthLayer.h>

namespace lima {

PacketGainer::PacketGainer(char * device) {
    mDeviceName = device;
    if (pcap_lookupnet(mDeviceName, &mIp, 
                       &mSubnetMask, mErrorBuff) == -1) {
        fprintf(stderr, "Coulnt get netmask for device: %s", mErrorBuff);
        return;
    }
    mHandler = pcap_open_live(mDeviceName, BUFSIZ, 1, 0, mErrorBuff);
    if (mHandler == nullptr) {
        fprintf(stderr, "Couldn't set up handler: %s", mErrorBuff);
    }
    if (pcap_datalink(mHandler) != DLT_EN10MB) {
		fprintf(stderr, "%s is not an Ethernet\n", mDeviceName);
        return;
	}

    if (pcap_compile(mHandler, &mCompiledPcap, mFilterExpression, 0, mIp) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", mFilterExpression, pcap_geterr(mHandler));
        return;
	}

	/* apply the compiled filter */
	if (pcap_setfilter(mHandler, &mCompiledPcap) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", mFilterExpression, pcap_geterr(mHandler));
        return;
	}
    mIsValid = true;
}

PacketGainer::~PacketGainer() {
    pcap_freecode(&mCompiledPcap);
	pcap_close(mHandler);
}

void PacketGainer::start() {
    pcap_loop(mHandler, -1, (pcap_handler)&PacketGainer::procces, nullptr);
}

void PacketGainer::stop() {
    pcap_breakloop(mHandler);
}


/*
    set dst and src ip
    set tcp/udp
    send payload with that data to analyze
*/
void procces(u_char * args, const struct pcap_pkthdr * header, const u_char * packet) {
    // get every layer
    // parsepacket
    
}

}