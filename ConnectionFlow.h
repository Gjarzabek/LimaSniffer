#ifndef CONNECTIONFLOW_H
#define CONNECTIONFLOW_H

#include <string>
#include <pcapplusplus/IpAddress.h>
#include <memory>
#include <chrono>
//#include <DnsInfo.hpp>

namespace lima {

// all information about srcIp - dstIp Connection
class ConnectionFlow {
    private:
        std::unique_ptr<pcpp::IPAddress> mSrcIp;        
        std::unique_ptr<pcpp::IPAddress> mDstIp;        
        std::chrono::steady_clock::time_point mTimeStamp;
        //DnsInfo mDnsInfo; // storing domain name's - from DNS
        // PageVisitHistory -- info from HTTP session
    public:
        ConnectionFlow() = default;
        ~ConnectionFlow() = default;
};

}

#endif