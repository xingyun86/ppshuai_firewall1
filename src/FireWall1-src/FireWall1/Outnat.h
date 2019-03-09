
#define  ETH_ALEN    6
//#define __LITTLE_ENDIAN_BITFIELD

typedef unsigned char __u8;
typedef unsigned short __u16;
typedef unsigned int __u32;
/* INDENT OFF */
typedef struct _ETH_HEADER
{
    unsigned char   h_dest[ETH_ALEN];
    unsigned char   h_source[ETH_ALEN];
    unsigned short  h_proto;
}               ETH_HEADER;

typedef struct _IP_HEADER
{
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	        ihl:4,
		            version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
	__u8	        version:4,
  		            ihl:4;
#endif
    __u8            tos;
    __u16           tot_len;
    __u16           id;
    __u16           frag_off;
    __u8            ttl;
    __u8            protocol;
    __u16           check;
    __u32           saddr;
    __u32           daddr;
}               IP_HEADER;

typedef struct _TCP_HEADER
{
    __u16           source;
    __u16           dest;
    __u32           seq;
    __u32           ack_seq;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u16	        res1:4,
		            doff:4,
		            fin:1,
		            syn:1,
		            rst:1,
		            psh:1,
		            ack:1,
		            urg:1,
		            res2:2;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u16	        doff:4,
		            res1:4,
		            res2:2,
		            urg:1,
		            ack:1,
		            psh:1,
		            rst:1,
		            syn:1,
		            fin:1;
#endif	
    __u16           window;
    __u16           check;
    __u16           urg_ptr;
}               TCP_HEADER;

typedef struct _UDP_HEADER
{
    __u16           source;
    __u16           dest;
	__u16           udpleng;
    __u16           check;
}               UDP_HEADER;


typedef struct _ETHER_PACKET
{
    ETH_HEADER      ETH;
    IP_HEADER       IP;    
} ETHER_PACKET;


struct  NAT_DATA{
	USHORT  us_DevInstance;
	USHORT  b_Direction;      
	ULONG   ul_BufferLength;
	UCHAR*  uc_DataBuffer;
};
/*
struct STATICTABLEITEM{
	BYTE     type;                  //=1:tcp;=2:udp;3:icmp 
    ULONG    ulIpadress;              
	USHORT   usPort;                  
};

struct STATICTABLE{
	int   count;
    STATICTABLEITEM*  item;
};
*/
struct  NATTABLE_ITEM{
	unsigned char     bUse;                //使用标志
	unsigned char     us_TimeOut;          //
	USHORT   us_InterPort;        //使用的内部端口
	USHORT   us_OutPort;          //使用的转换后的外部端口
	USHORT   uInSuffixParent;     //in hashID冲突项 
	USHORT   uInSuffixNext;       //in hashID冲突项 
	USHORT   uOutSuffixParent;    //out hashID冲突项 
	USHORT   uOutSuffixNext;      //out hashID冲突项 
	ULONG    ul_InterIPAddress;   //使用的内部地址
    ULONG    ul_OutIPAddress;     //与其通讯的外部地址
	ULONG    ul_GlobalIPAddress; 
};
struct STATICTCPTABLE{
	ULONG    ulIpadress;              
	USHORT   usPort;	
};

struct STATICUDPTABLE{
	ULONG    ulIpadress;              
	USHORT   usPort;   
};


USHORT  GetCksum(USHORT* buffer,int size);
//BOOL  NatForOutPacket(BYTE* EthBuffer,ULONG packetlength,USHORT Adapternumber);
//BOOL  NatForInPacket(BYTE* EthBuffer,ULONG packetlength,USHORT Adapternumber);
BOOL  GetTCPCksum(BYTE* buffer,int leng);
BOOL  GetUDPCksum(BYTE* buffer,int leng);
BOOL  GetIPCksum(BYTE* buffer,int leng);
void NatCfg();
inline NATTABLE_ITEM* NatSearch(BYTE* EthBuffer,BOOL bDirection, USHORT type);
USHORT  NATGetIndexOther(ULONG ipaddress1,ULONG ipaddress2,USHORT sport);
BOOL  NATableTimeInspec();
BOOL  NATableTimeInspecUDP();
BOOL NatPacket(BYTE* total_EthBuffer);
inline USHORT swaps(USHORT port);
void NatDelItem();
void NatBeginThread(PVOID temp);
NATTABLE_ITEM*  NatTableAddItem(BYTE* EthBuffer,BOOL bDirection, USHORT type);
void ClearNatUseResoure();
inline BOOL NATSearchStatic(BYTE* EthBuffer,BOOL Direction,USHORT type);
/*for test*/
//BOOL  NatForInIcmpPacket(BYTE* EthBuffer,ULONG packetlength,USHORT Adapternumber);
//BOOL  NatForOutIcmpPacket(BYTE* EthBuffer,ULONG packetlength,USHORT Adapternumber);

