#include "ConnectionFlow.h"
#include <pcapplusplus/TcpLayer.h>
#include <pcapplusplus/IPv4Layer.h>
#include <pcapplusplus/IPv6Layer.h>
#include <pcapplusplus/EthLayer.h>

namespace lima {

    ConnectionFlow::ConnectionFlow(const ConnectionFlow & other) : 
        mSrcIp(std::make_unique<pcpp::IPAddress>(other.mSrcIp)),
        mDstIp(std::make_unique<pcpp::IPAddress>(other.mDstIp)),
        mSrcMAC(other.mSrcMAC), mDstMAC(other.mDstMAC),
        mStartTimeStamp(other.mStartTimeStamp), mLastTimeStamp(other.mLastTimeStamp),
        mDnsInfo(other.mDnsInfo), mIsFinished(other.mIsFinished), mIsValid(other.mIsValid),
        mPacketCounter(other.mPacketCounter) {}
    
    
    ConnectionFlow::ConnectionFlow(pcpp::Packet & packet) : mStartTimeStamp(std::chrono::steady_clock::now()) {
        mIsValid = false;
        mIsFinished = false;

        auto firstLayer = packet.getFirstLayer();
        if (firstLayer == nullptr) {
            return;
        }

        if (firstLayer->getProtocol() == pcpp::ProtocolType::Ethernet) {
            auto etherLayer = dynamic_cast<pcpp::EthLayer *>(firstLayer);
            mDstMAC = etherLayer->getDestMac().toString();
            mSrcMAC = etherLayer->getSourceMac().toString();
        }
        else {
            return;
        }

        if (firstLayer->getNextLayer()->getProtocol() == pcpp::ProtocolType::IPv4) {
            auto ipv4layer = dynamic_cast<pcpp::IPv4Layer*>(firstLayer->getNextLayer());
            mDstIp = std::make_unique<pcpp::IPv4Address>((ipv4layer->getDstIpAddress()));
            mSrcIp = std::make_unique<pcpp::IPv4Address>((ipv4layer->getSrcIpAddress()));
        }
        else if (firstLayer->getNextLayer()->getProtocol() == pcpp::ProtocolType::IPv6) {
            auto ipv6layer = dynamic_cast<pcpp::IPv6Layer*>(firstLayer->getNextLayer());
            mDstIp = std::make_unique<pcpp::IPv6Address>((ipv6layer->getDstIpAddress()));
            mSrcIp = std::make_unique<pcpp::IPv6Address>((ipv6layer->getSrcIpAddress()));
        }
        else {
            return;
        }
        if (packet.getLastLayer()->getProtocol() == pcpp::ProtocolType::DNS)
            mDnsInfo = DnsInfo(dynamic_cast<pcpp::DnsLayer *>(packet.getLastLayer()));

        mIsValid = true;
    }

    //TODO: write update
    void ConnectionFlow::update(pcpp::Packet & packet, const IpPair & SrcDstIp) {
        checkFinishConditions(packet);
        mLastTimeStamp = std::chrono::steady_clock::now();
        updateCounter(SrcDstIp);
        mDnsInfo.update();
    }

    void ConnectionFlow::updateCounter(const IpPair & SrcDstIp) {
        mSrcIp->toString() == SrcDstIp.first ? ++(mPacketCounter.fromSrcToDst) : ++(mPacketCounter.fromDstToSrc);
    }

    void ConnectionFlow::checkFinishConditions(pcpp::Packet & packet) {
        pcpp::TcpLayer * TransportLayer = dynamic_cast<pcpp::TcpLayer*>(packet.getLayerOfType(pcpp::ProtocolType::TCP));
        pcpp::tcphdr * hdr = TransportLayer != nullptr ? TransportLayer->getTcpHeader : nullptr;
        if (hdr && (hdr->finFlag || hdr->rstFlag))
            mIsFinished = true;
        else if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - mLastTimeStamp).count() > 15)
            mIsFinished = true;
    }


}