#ifndef DNSINFO_HPP
#define DNSINFO_HPP

#include <pcapplusplus/DnsLayerEnums.h>
#include <pcapplusplus/DnsLayer.h>
#include <ctime>
#include <cstdlib>

/* class to store info DnsConversations for signle Connectionflow
 Current info extracted from DNS protocol
 a) Querries:
    - domain name
    - type
    - class
*/

namespace lima {

    class DnsInfo {
    
    private:

        struct InfoNode {
            InfoNode() : mDomainName("") {}

            InfoNode(const std::string & Domain, pcpp::DnsClass dnsclass, pcpp::DnsType type) :
                mDomainName(Domain), mDnsClass(dnsclass), mDnsType(type) {}

            std::string mDomainName;
            pcpp::DnsClass mDnsClass;
            pcpp::DnsType mDnsType;

            bool operator==(const InfoNode & other) const {
                return mDomainName == other.mDomainName && mDnsClass == other.mDnsClass && mDnsType == other.mDnsType;
            }
        };

        struct InfoNodeHasher {
            
            size_t operator()(const InfoNode & node) const {
                return (node.mDomainName.size() * (uint8_t)node.mDomainName[0] + node.mDnsType + node.mDnsClass) / 
                            ((uint8_t)node.mDomainName[0] + (uint8_t)node.mDomainName[node.mDomainName.size() - 1]);

            }

        };

        // hashTable to store infonodes and count the repetitions
        std::unordered_map<InfoNode, uint64_t, InfoNodeHasher> mDnsMap;
    
    public:

        DnsInfo(pcpp::DnsLayer * dnslayer) {
            auto firstquerry = dnslayer->getFirstQuery();
            while (firstquerry) {
                InfoNode toinsert = InfoNode(firstquerry->getName(), firstquerry->getDnsClass(), firstquerry->getDnsType());
                auto iter = mDnsMap.find(toinsert);
                if (iter == mDnsMap.end())
                    mDnsMap.emplace(toinsert, 0);
                else 
                    ++(iter->second);
                firstquerry = dnslayer->getNextQuery(firstquerry);
            }
        }

        DnsInfo() = default;

        ~DnsInfo() = default;

        void update(pcpp::DnsLayer * dnslayer);

    };
}

#endif