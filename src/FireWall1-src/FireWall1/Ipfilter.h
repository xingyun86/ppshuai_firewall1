

/*ip.h*/
#if !defined(FIREWALLFILTER)
#define FIREWALLFILTER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "ip.h"
#include "Struct.h"
 
//常量
enum { NO=0, YES } ;
enum { DISCARD_PKT=0, ALLOW_PKT, DISCARD_AND_ANSWER_PKT };
enum { ACTIVATE_NEW=0, CLEAR_ALL, INIT_NEW, MEM_REPORT };
enum { READ=0, WRITE, MANAGE };
enum { LOG_INFO = 0, LOG_WARNING, LOG_NOTICE };
enum { T_DST_IN=0, T_DST_OUT, T_SRC_IN, T_SRC_OUT,
       U_DST_IN, U_DST_OUT, U_SRC_IN, U_SRC_OUT,
       I_TYP_IN, I_TYP_OUT, T_RST_IN, T_RST_OUT,
       T_ACCTBL, T_REJTBL,T_INDM,T_OUTDM};
enum { DEFAULT_ACCESS=0 };
enum { NORMAL=0, INVERT };
enum { FLAG_MULTICAST=0, FLAG_NONIP, FLAG_OTHERIP, FLAG_SUSPECTOFFSET,
       FLAG_FRAGMENTEDICMP, FLAG_ATTACKICMP, MAX_FLAG };
//最前面的五个不能改变
enum { DB_DROP_PKT=0, DB_INSIDE_IF=1, DB_OUTSIDE_IF=2, DB_BOTH_IF=3,
       DB_PROCESS_PKT=4, DB_FORWARD_PKT, DB_DROP_AND_ANSWER_PKT };

#define LIST_SIZE		32
#define LIST_TABLE_SIZE		(LIST_SIZE * sizeof(PortListEntry))

//ip包头偏移掩码

#define SW_IP_OFFMASK		0xFF1F	

//系统常量

#define DEFAULT_SYSL_MASK	0		/* 0x1FFFF 全部记入日志*/
//#define DEFAULT_SYSL_FACILITY	0 //LOG_LOCAL//0

enum {IN_INF=0,OUT_INF=1,DM_INF=2};
enum {
	SYSL_IN_CLASSD,//multcast addr,srcaddr,protoclno,srcport,dstport
	SYSL_OUT_CLASSD,//multcast addr,srcaddr,protoclno,srcport,dstport
	SYSL_IN_PORT,// protocolNo, srcAddr and dstAddr,srcport,dstport
	SYSL_OUT_PORT,		/* protocolNo, srcAddr and dstAddr,srcport,dstport */
	SYSL_IN_LENGTH,		/* TCP/UDP-Header too short:  protocolNo, srcAddr and dstAddr,srcport,dstport */
	SYSL_OUT_LENGTH,	/* TCP/UDP-Header too short: protocolNo, srcAddr and dstAddr,srcport,dstport */

	SYSL_IN_TYPE,		/*  srcAddr, dstAddr, and icmp type */
	SYSL_OUT_TYPE,		/*  srcAddr, dstAddr, and icmp type */

	SYSL_IN_REJECT,		/*  protocolNo, srcAddr and dstAddr */
	SYSL_OUT_ACCEPT,	/*  protocolNo, srcAddr and dstAddr */
	SYSL_IN_PROT,		/*  nonip: protocolNo, srcAddr and dstAddr */
	SYSL_OUT_PROT,		/*  nonip: protocolNo, srcAddr and dstAddr */
	SYSL_IN_OFFSET,		/* suspect fragment offset:protocolNo, srcAddr and dstAddr */
	SYSL_OUT_OFFSET,	/* suspect fragment offset:protocolNo, srcAddr and dstAddr */
	SYSL_IN_FRAG,		/* fragmented packet:protocolNo, srcAddr and dstAddr*/
	SYSL_OUT_FRAG,		/* fragmented packet:protocolNo, srcAddr and dstAddr */

	SYSL_IN_DOS,		/* denial of service attack packet in:dos类型，srcaddr,dstaddr */
	SYSL_OUT_DOS,		/* denial of service attack packet out:dos类型，srcaddr,dstaddr */
    SYSL_OUT_ROUT,
    SYSL_IN_ICMP,
    SYSL_OUT_ICMP,
    SYSL_IN_IGMP,
    SYSL_OUT_IGMP,
};

// DOS攻击
 
enum dos_type {
	DOS_SMURF=0, DOS_PONG};

//错误信息
enum error_msg{
	NOO_ERROR,
	ERROR_OUT_OF_RANGE,	
	ERROR_UNKNOWN_GROUP,
	ERROR_INVALID_NETWORK,
	ERROR_TABLE_FULL,
	ERROR_NET_NOT_FOUND,
	ERROR_NO_MEMORY,
	ERROR_INVALID_OP, 
	ERROR_IF_DOWN,
	ERROR_GET_IP,
	ERROR_INVALID_IF,
	ERROR_SAME_IF,
	ERROR_DIFF_IF_TYPE,
	ERROR_BAD_IF_TYPE,
	ERROR_INSIDE_DOWN, 
	ERROR_OUTSIDE_DOWN,
	ERROR_TWO_IP,
	ERROR_NO_IP,
	ERROR_INVALID_MODE,	
	ERROR_NOT_INITILIZED,
	ERROR_RUNNING,
	ERROR_NOT_RUNNING,
	MAX_ERROR_NUM
};

#define DIOCINIT	1
#define DIOCSTART   2
#define DIOCSTOP	3
#define DIOCGFLAGS	4
#define DIOCSFLAGS	5
#define DIOCGSTATS	6
#define DIOCCSTATS	7
#define DIOCGBRIDGE	8
#define DIOCCBRIDGE	9
#define DIOCGCLASS	10
#define DIOCSCLASS	11
#define  DIOCSGROUP  12
#define DIOCGNETWORK	13 /* 12 no longer used */
#define DIOCSNETWORK	14
#define DIOCMNETWORK	15
#define DIOCGOVERRIDE	16
#define DIOCGREJECT	18
#define DIOCSREJECT	19
#define DIOCGACCEPT	20
#define DIOCSACCEPT	21
#define DIOCGLOGF	22
#define DIOCSLOGF	23
#define DIOCGLOGM	24
#define DIOCSLOGM	25
#define DIOCGVER	26
#define DIOCGTCPRESET	27
#define DIOCSTCPRESET	28
#define DIOCGMGTIF	29
#define DIOCSMGTIF	30
#define DIOCGMMODE	31
#define DIOCSMMODE	32
#define DIOCS_INDM  35
#define DIOCS_OUTDM  36

/*==============================================================*/
/*                           数据结构                           */
/*==============================================================*/
/*
typedef union _in_addr {
//	ULONG s_addr;
	struct {
		BYTE s_b[4];
	} s_un_b;
//	struct {
//		unsigned short s_w[2];
//	} s_un_w;
ULONG ss_addr;
#define s_b s_un_b.s_b
//#define s_w s_un_w.s_w
} in_addr_t;
*/

typedef struct _FilterConfig {
	ULONG  	logMask;
//#define CFG_CLR_SIZE	(sizeof(FilterConfig) - sizeof(ULONG))
//#define CFG_CLR_START	in_number
	USHORT    in_number;//adapter num     
	USHORT    out_number;//adapter num     
	USHORT    dm_number;//adapter num  
	USHORT    media_type;//gj
	ULONG	    in_ip;
	ULONG	    out_ip;
	ULONG	    dm_ip;
	ULONG     in_mask;
	ULONG     out_mask;
	ULONG     dm_mask;
	BOOL      discardIcmp;
	BOOL	  discardMulticast;
	BOOL		discardNonIp;
	BOOL		discardOtherIp;
	BOOL		discardSuspectOffset;
	BOOL		discardFragmentedICMP;
	BOOL		discardAttackICMP;
	BOOL     discardRouteIp;
} FilterConfig;
//流量统计信息
typedef struct _Statistics {
	unsigned short		running;
	SYSTEMTIME   starttime;
	ULONG	p_insideFiltered;//过滤掉的内网包计数
	ULONG  p_dmFiltered;
	ULONG	p_outsideFiltered;//过滤掉的外网包计数
	ULONG	p_insideRx;//内网卡接收到的包总数:成功的+过滤掉的;
	ULONG  p_dmRx;
	ULONG	p_outsideRx;
	ULONG	p_insideTx;//成功的进站包数
	ULONG  p_dmTx;
	ULONG	p_outsideTx;//成功的出站包数
	ULONG	b_insideFiltered;//过滤掉的内网包总长（bytes）
	ULONG   b_dmFiltered;
	ULONG	b_outsideFiltered;
	ULONG	b_insideRx;
	ULONG  b_dmRx;
	ULONG	b_outsideRx;
	ULONG	b_insideTx;
	ULONG  b_dmTx;
	ULONG	b_outsideTx;
    ULONG  f_outsideOtherIp;
	ULONG   f_insideOtherIp;
	ULONG	f_insideMcast;
	ULONG	f_outsideMcast;
	ULONG	f_insideTcpPort;
	ULONG	f_insideUdpPort;
	ULONG	f_insideIcmpType;
	ULONG	f_outsideIcmpType;

	ULONG	f_outsideRejectTable;
	ULONG	f_outsideSuspectOffset;
	ULONG	f_outsideIcmpFrag;
	ULONG	f_outsideSmurfDos;
	ULONG	f_outsidePongDos;

	ULONG	f_insideAcceptTable;
	ULONG	f_insideSuspectOffset;
	ULONG	f_insideIcmpFrag;
	ULONG	f_insideSmurfDos;
	ULONG	f_insidePongDos;

	ULONG	insideTCPreset;
	ULONG	outsideTCPreset;
} Statistics;

typedef struct _SyslogMessageEntry {
	const char *message;
	unsigned char priority;
} SyslogMessageEntry;

//表的结构
typedef struct {
        unsigned short c[256];
} addrTreeLeaf;


typedef struct _addrTreeNode {
        union {
                addrTreeLeaf *pd;
        } u[256];
        unsigned char flg[256];
} addrTreeNode;
typedef struct _OutaddrTreeNode {
        union {
                struct _OutaddrTreeNode *p;
                addrTreeLeaf *pd;
                unsigned short c;
        } u[256];
        unsigned char flg[256];
} OutaddrTreeNode;
typedef union {
        OutaddrTreeNode *p;
        addrTreeLeaf *pd;
        unsigned short c;
} addrTreePtr;

typedef struct _PortListEntry {
	unsigned short begin;
	unsigned short end;
} PortListEntry;

typedef struct _RejAccTableEntry {
	in_addr_t network;//起始地址
	unsigned short bits;//主机数
	unsigned short flag;//类号
} RejAccTableEntry;


typedef struct _pointerStruct{
        USHORT		num_groups;
        addrTreeNode		*addrTree;//对应于主机类的地址
        PortListEntry		*t_dst_out;
        PortListEntry		*t_src_out;
        PortListEntry		*u_dst_out;
        PortListEntry		*u_src_out;
        PortListEntry		*i_typ_in;
        PortListEntry		*i_typ_out;
	PortListEntry		*t_rst_in;
	PortListEntry		*t_rst_out;
	addrTreeNode		*acceptTree;
	RejAccTableEntry	*acceptTable;
    addrTreeNode        *In_DmTree;
    OutaddrTreeNode     *Out_DmTree;
	ULONG		accept_bytes;
	OutaddrTreeNode		*rejectTree;
	RejAccTableEntry	*rejectTable;
	ULONG		reject_bytes;
	ULONG		override_bytes;
	ULONG		num_override;
} pointerStruct;


//输入输出的数据结构

typedef struct _initReq {
	char ifnumber;   //设备编号
	ULONG ifIpaddr;    //接口IP地址
	ULONG mask;        //子网掩码
	char flag;   //表示是内、外、中设备。0：为内部设备；1：为外部设备；2：为DM设备
} initReq;


typedef struct _flagsReq {
	ULONG discardMulticast;
	ULONG discardNonIp;
	ULONG discardOtherIp;
	ULONG discardSuspectOffset;
	ULONG discardFragmentedICMP;
	ULONG discardAttackICMP;
	ULONG discardRouteIp;    //源路由攻击
} flagsReq;


typedef struct _portListReq {
	unsigned short id;
	unsigned short group;
	PortListEntry *ptr;
	unsigned short bytes;
} portListReq;   

typedef struct _addrTreeReq {
	ULONG num;
	ULONG newtable;
	in_addr_t addr[128];
	unsigned short bits[128];
	unsigned short group[128];
} addrTreeReq;

typedef struct _manageTablesReq {
	ULONG command;
	USHORT val;
} manageTablesReq;

typedef struct _TCPresetTableReq {
	PortListEntry in[LIST_SIZE];
	PortListEntry out[LIST_SIZE];
} TCPresetTableReq;
typedef struct _rejaccTableReq {
	USHORT bytes;
	RejAccTableEntry *ptr;
	ULONG addr;
	unsigned long flag;
} rejaccTableReq;

typedef struct _overrideTableReq {
	ULONG bytes;
	void *ptr;
	ULONG addr;
	unsigned short index;
} overrideTableReq;


typedef struct _logMaskReq {
	ULONG mask;
} logMaskReq;

/*==============================================================*/
/*                         宏定义                        */
/*==============================================================*/

#define IS_BROADCAST(address) 	(*((unsigned char *) (address)) & 0x01)


#define MALLOC_LEAF (addrTreeLeaf *)malloc(sizeof(addrTreeLeaf))

#define MALLOC_NODE (addrTreeNode *)malloc(sizeof(addrTreeNode))
#define MALLOC_OUTNODE (OutaddrTreeNode *)malloc(sizeof(OutaddrTreeNode))

#define MALLOC_GROUP(a) (PortListEntry *)malloc((a) * LIST_TABLE_SIZE)




/*==============================================================*/
/*                         函数                                     */
/*==============================================================*/

#define FROMOUT 10
#define FROMIN  11
//主函数
ULONG  ipfilter_test_pkt(USHORT dev_instance, UCHAR direction,ULONG ethlen,BYTE *m);
VOID   ipfilter_pkt(ULONG ethlen,BYTE *m);
void ipfilter_response_pkt(BYTE *rout,ULONG outlen,ULONG inlen,BYTE *min);
//配置命令发生器
ULONG  filter_command( unsigned long cmd, char* addr);
VOID mysearch();

#endif
