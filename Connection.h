#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <pcapplusplus/IpAddress.h>
#include <memory>
#include <chrono>

namespace lima {

// all information about srcIp - dstIp Connection
class Connection {
    private:
        std::unique_ptr<pcpp::IPAddress> mSrcIp;        
        std::unique_ptr<pcpp::IPAddress> mDstIp;        
        std::string mDomain; // storing domain name's - from DNS
        std::chrono::steady_clock::time_point mTimeStamp;
        // PageVisitHistory -- info from HTTP session
    public:
        Connection();
        ~Connection();
};

}

#endif