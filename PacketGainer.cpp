#include "PacketGainer.h"
#include "stdio.h"

#include <iostream>
#include <pcapplusplus/EthLayer.h>

namespace lima {

PacketGainer::PacketGainer(std::string device): mDeviceName(device.c_str()) {
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
    pcap_loop(mHandler, -1, PacketGainer::procces, nullptr);
}

void PacketGainer::stop() {
    pcap_breakloop(mHandler);
}

void PacketGainer::procces(u_char * args, const struct pcap_pkthdr * header, const u_char * packet) {
    static uint64_t num = 0;
    if (packet == nullptr || !header) {
        std::cerr << "Pusty pakiet nr:" << num++ << std::endl;
        return;
    }
    /*std::cout << "Pakiet nr:" << num++ << std::endl;
    std::cout << "Len:" << header->len << std::endl;
    std::cout << "Caplen:" << header->caplen << std::endl;*/
    pcpp::RawPacket rawPacket((const uint8_t*)packet, header->len, header->ts, false);
    pcpp::Packet parsedPacket(&rawPacket);
    auto lastLayer = parsedPacket.getLastLayer();
    if (lastLayer && lastLayer->getProtocol() == 0x1000)
        std::cout << parsedPacket.toString() << std::endl;
    /*const pcpp::ether_header * ethernetHeader = reinterpret_cast<const pcpp::ether_header *>(packet);
    pcpp::MacAddress dst((const char *)ethernetHeader->dstMac);
    pcpp::MacAddress src((const char *)ethernetHeader->srcMac);
    std::cout << "dstMAC:" << dst.toString() << std::endl;
    std::cout << "srcMAC:" << src.toString() << std::endl;*/
}

}