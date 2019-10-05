#ifndef CONNECTIONFLOW_H
#define CONNECTIONFLOW_H

#include <string>
#include <pcapplusplus/IpAddress.h>
#include <memory>
#include <chrono>

#include <DnsInfo.hpp>

namespace lima {

// all information about srcIp - dstIp Connection
class ConnectionFlow {

    struct DnsInfoNode {
        DnsInfo mDnsInfo;
    };

    private:
        std::unique_ptr<pcpp::IPAddress> mSrcIp;        
        std::unique_ptr<pcpp::IPAddress> mDstIp;        
        std::chrono::steady_clock::time_point mStartTimeStamp;
        std::chrono::steady_clock::time_point mLastTimeStamp;
    public:
        ConnectionFlow() = default;
        ~ConnectionFlow() = default;
};

}

#endif