#pragma once
#include <pcap.h>
#include "pkttypes.h"
#include "others.h"

class PKT{
    private:
        ////// for pcap.h //////
        char* dev;
        pcap_t* pcap_handler;
        char errbuf[PCAP_ERRBUF_SIZE];
        int pcap_res;
        
        ////// packet pointer /////
        const uint8_t* pkt_ptr;
        ETHIPTCP* tcp;
        ETHIPUDP* udp;
        ETHIPICMP* icmp;
        
        ////// 패킷 속성 //////
        int pkttype;
        int pktsize;

        // 패킷 만들기 내부 함수 //
        void make_common_part(mac_t* target_mac, ip_t target_ip, ETHIPHDR* common);
        void make_tcp_packet(ETHIPTCP* tcp_ptr, int flagtype, int datalen);
        void make_udp_packet(ETHIPUDP* udp_ptr, int datalen);
        void make_icmp_packet(ETHIPICMP* icmp_ptr, int flagtype, int datalen);
    public:
        // initialize
        PKT(char* dev);
        ~PKT();

        // functions for setting, make, send packets
        void set_pcap();
        
        // target의 맥주소 받아올 함수
        void set_attackinfo(ip_t target_ip, mac_t* storage);
        
        // 패킷 만드는 함수
        void make_packet(mac_t* target_mac, uint32_t target_ip, int pkttype, int flagtype, int datalen);
        void send_packet();
};