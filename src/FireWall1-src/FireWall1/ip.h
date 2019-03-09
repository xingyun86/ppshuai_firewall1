
#if !defined(FIREWALLIP)
#define FIREWALLIP

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//网络包包头结构定义 
#include "StdAfx.h"
#include "ipfilter.h"

#define ETHER_PROTO_IP 0x0800
#define ETHER_PROTO_ARP 0x0806
//#define ETHER_PROTO_REVARP 0x
#define ETHER_HEAD_LEN 14
#define IFT_ETHER 105//gj
#define IP_VER 4
#define IP_HEAD_LEN 5
#define IP_SERV_TYPE 0
#define IP_PROTO_TCP 6
#define IP_PROTO_UDP 17
#define IP_PROTO_ICMP 1
#define IP_PROTO_IGMP 2
#define IP_PAD 0
#define IP_MF 0x4000
#define IP_DF 0x2000
#define IP_FLAG_MASK 0x1fff
#define IP_HEAD_BYTE_LEN 20

#define TCP_HEAD_LEN 5
#define TCP_URG 0x20
#define TCP_ACK 0x10
#define TCP_PSH 0x08
#define TCP_RST 0x04
#define TCP_SYN 0x02
#define TCP_FIN 0x01
#define TCP_HEAD_BYTE_LEN 20

#define UDP_HEAD_LEN 8

#define PSEUDO_HEAD_LEN 12

#define ICMP_HEAD_LEN 4
#define IPF_PROCESS_PKT 100//gj
#define  IPF_DROP_PKT    101//gj
#define  IPF_DROP_AND_ANSWER_PKT  102//gj
#define IPF_FORWARD_PKT  104//gj
struct Ether_Addr {
	unsigned char AddrByte[6];
};

struct Ether_Header{
   struct Ether_Addr ether_dhost;
   struct Ether_Addr ether_shost;
   unsigned short ether_type;
};

struct IP_Addr{
	unsigned char AddrByte[4];
};
/*
struct ip{
	unsigned char VerHlen;
	unsigned char Type;
	unsigned short TtlLen;
	unsigned short Id;
	unsigned short Flgoff;
	unsigned char TTL;
	unsigned char Proto;
	unsigned short ChkSum;
	in_addr_t ip_src;
	in_addr_t ip_dst;
};
*/

typedef struct ip{
	BYTE  ip_hl:4;//gj
	BYTE  ip_v:4;//gj
//	BYTE VerHlen;//版本和头标长
	BYTE Type;//服务类型
	WORD TtlLen;//总长
	WORD Id;//标识
	WORD Flgoff;//标志和片偏移
	BYTE TTL;
	BYTE Prot;//高层协议;
	WORD ChkSum;//头校验和
	in_addr_t ip_src;
	in_addr_t ip_dst;
}ip;


struct tcphdr{
	WORD th_sport;
	WORD th_dport;
	DWORD th_seq;
	DWORD th_ack;
	BYTE /*Hlen*/th_off;//首部长度
	BYTE th_flags;//控制标志
	WORD WndSize;//窗口大小
	WORD th_sum;
	WORD UrgPtr;//紧急指针
};

struct icmp{
	BYTE icmp_type;
	BYTE icmp_code;//代码：项对于Type的具体信息
	WORD icmp_cksum;
	DWORD icmp_void;//id&seq:应答与请求相匹配
	WORD *icmp_data;//gj ADD
};

struct udphdr{
	WORD uh_sport;
	WORD uh_dport;
	WORD Len;//长度（包括报文头和数据）
	WORD ChkSum;
};

struct PseudoHead {
	struct IP_Addr SourIP;
	struct IP_Addr DestIP;
	BYTE Pad;
	BYTE Proto;
	WORD Len;
};

struct ARPPacket {
	 WORD Type;
	 WORD Proto;
	 BYTE HardAddrLen;
	 BYTE AddrLen;
	 WORD Oper;
	 struct Ether_Addr SourEther;
	 struct IP_Addr SourIP;
	 struct Ether_Addr DestEther;
	 struct IP_Addr DestIP;
};

struct pkt_struct{
		struct ip i;
		struct tcphdr t;
	};
#endif

