#include <ConnectionFlowWriter.h>
#include <functional>

namespace lima {

    void ConnectionFlowWriter::packetProcess() {
        while (!mProcessPackets.empty()) {
            auto packet = mProcessPackets.back();
            std::unique_ptr<IpPair> SrcDstIp;
            auto ipv4 = dynamic_cast<pcpp::IPv4Layer* >(packet.getLayerOfType(pcpp::ProtocolType::IPv4));
            
            if (ipv4) {
                SrcDstIp = std::make_unique<IpPair>(ipv4->getSrcIpAddress().toString(),
                                                    ipv4->getDstIpAddress().toString());
            }
            else {
                auto ipv6 = dynamic_cast<pcpp::IPv6Layer* >(packet.getLayerOfType(pcpp::ProtocolType::IPv6));
                SrcDstIp = std::make_unique<IpPair>(ipv6->getSrcIpAddress().toString(),
                                                    ipv6->getDstIpAddress().toString());
            }

            ConnectionFlow Flow = ConnectionFlow(packet);
            auto iter = mCurrentFlows.find(*SrcDstIp);
            
            if (iter == mCurrentFlows.end()) {
                mCurrentFlows.emplace(std::move(*SrcDstIp), std::move(Flow));
            }
            else {
                ConnectionFlow & updateFlow = mCurrentFlows[iter->first];
                updateFlow.update(packet);
                if (updateFlow.isFinished()) {
                    mFlowsToExport.emplace(std::move(*SrcDstIp), std::move(updateFlow));
                    mCurrentFlows.erase(iter);
                }
            }
            
            mProcessPackets.pop();
        }   
    }

    void ConnectionFlowWriter::start() {
        mWork = true;
        
        while (mWork) {
            std::unique_lock<std::mutex> mlock(mReceicePacketsMutex);
            mReceiveCondVar.wait(mlock, std::bind(ConnectionFlowWriter::isQueueReady, this));
            mProcessPackets = PacketQueue(std::move(mReceivePackets));
            packetProcess();
        }    
    }

    void ConnectionFlowWriter::stop() {
        mWork = false;
    }
}