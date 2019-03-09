

#if !defined(FIREWALLSTRUCT)
#define FIREWALLSTRUCT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//设备号与IP地址的映射
struct MapDevToIp{
          int  nDevNum;
          ULONG nIpAddr;
};
//超时设置：用动态端口NAT
struct TimeOut{
          int nTcpTimeOut;//小时
          int nUdpTimeOut;//分钟
};

//(静态、动态)内外NAT映射
struct MapAddrNat{
          unsigned long nIntraLocalAddr;//内网ip地址
          unsigned long nOutsideGlobalAddr;//外网ip地址
};

// 动态端口NAT设置
struct DynamicPortNat{
          USHORT  nMinPort;//与MaxPort一起表示端口转换范围
          USHORT  nMaxPort;
          USHORT  nMaxPortConversion;//端口最大转换数
          struct TimeOut sTimeOut;// 超时设置
};
// 动态端口NAT映射
struct MapDynamicPortNat{
          unsigned long nIntraLocalAddr; //内网ip地址
          unsigned long nOutsideGlobalAddr;//外网ip地址
          unsigned short  nPort;//端口
};
//静态端口NAT映射
struct STATICTABLEITEM{
	BYTE     type;                  //=1:tcp;=2:udp;3:icmp 
    ULONG    ulIpadress;              
	USHORT   usPort;                  
};

struct STATICTABLE{
	int   count; 
    STATICTABLEITEM*  item;
};
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

//配置数据
struct  NAT_Cfg {
        ULONG   nGlobalAddress; //外部地址:即nOutsideIp
//      int     nDenyInternalAddressNumber; //禁止NAT的内部地址数量
//      ULONG*  ul_DenyGlobalAddress;      //内部禁止地址范围
//      int     nInternalNetWorkNumber;     //内部地址网络数量
//      ULONG*  ul_NetworkAddress;          //内部网络地址
        int     nTimeOut_TCP;           //动态端口转换的时间溢出值
        int     nTimeOut_UDP;           //动态端口转换的时间溢出值
        USHORT  us_LowPort;             //最低可用端口号 
        USHORT  us_HighPort;            //最高可用端口号
        USHORT  us_MaxNatNumber;        //最大端口转换数量
        USHORT  us_PortNumber;          //静态端口映射数量
        struct  STATICTABLE  StaticTable;//静态端口NAT映射列表
};

struct  Filterdrv_Cfg {
int         n_Dev_number;          //安装网络设备数量
USHORT      us_Dev_Status[10][2];    //设备状态
            //列1 项 = 设备编号
            //列 2 项 = 设备拦截状态
                     // 0        拦截所有数据包
                    // 1          不拦截数据包
                    // 2          只拦截入站数据包
                    // 3          只拦截出站数据包
                    // 4          丢弃所有数据包
};


//内网、外网、DM区设置
struct  FireWallConfig{
	      USHORT   OutNumber;
		  USHORT   InNumber;
		  USHORT   MdNumber;
          ULONG     nOutsideIp;//外网ip
		  ULONG     nOutsideMask;//外网地址掩码
		  ULONG     nMiddleIp;//DM区IP
		  ULONG     nMiddleMask;
          ULONG     nIntraIp;
		  ULONG     nIntraMask;
//          struct  Filterdrv_Cfg   sFilterdrvCfg;
//          struct  NAT_Cfg    sNatCfg;         
};
struct  IpaddrRange{
         USHORT    nIpaddrNum;//个数；从1开始
         ULONG     nIpMask;//IP掩码
         ULONG     nIpAddr;//起始IP地址，根据nIpaddrNum推算后续IP地址
};

//端口范围
struct  PortRange{
         unsigned short  nPortBegin;
         unsigned short  nPortEnd;
};
// 端口列表
struct  PortList {
         BOOL  bRange;//true: PortRange有效
         union{
               unsigned short  nPort;// 拒绝的具体端口,
               struct   PortRange     sPortRange;// 拒绝的端口范围
		 };
};
//允许与拒绝的端口列表
struct  PortSet{
         CPtrList  sAllowOutDstPortList;// 允许出去的目的端口列表
         CPtrList  sAllowOutSrcPortList;// 允许出去的源端口列表
};

struct  IcmpRange{
         USHORT   nIcmpBegin;
         USHORT   nIcmpEnd;
};// 拒绝的ICMP类型范围
// ICMP列表
struct  IcmpList {
   //      IcmpList * pIcmpList;
         BOOL  bRange;//true: PortRange有效
         union{
               USHORT   nIcmpType;// 拒绝的具体ICMP类型,
               struct  IcmpRange sIcmpRange;// 拒绝的ICMP类型范围
		 };
};
//主结构：对所有主机的设置
struct  HostList{
 //       HostList *pHostList;
         struct   IpaddrRange   sIpRange;//标识主机范围（可以为一台主机或多台）
         struct  PortSet  sTcpPortSet;
         struct  PortSet  sUdpPortSet;
		 CPtrList    sInIcmpSet;//允许进来的ICMP报文类型
         CPtrList    sOutIcmpSet; //允许出去的ICMP报文类型
};


//全局设置：针对firewall主机
struct GlobalFlags{
        BOOL bCheckInNetAttack;//是否检查对内网的访问；
        BOOL bCheckOutNetAccack;//是否检查对外网的访问；
        BOOL bCheckDmAccack;//是否检查对DM区的访问；
        BOOL bMulticast;//组播允许标志
        BOOL bNonIpPacket;//非Ip包
        BOOL bOtherIpPacket;//除tcp,udp,icmp以外的ip包
        BOOL bSusPectOffset;// Ip包内发现可疑的偏移
        BOOL bFragmentIcmp;//分段的ICMP包
        BOOL bSusPectIcmp;//可疑的ICMP包
		BOOL bforbidIcmp;
};


//以下用于界面显示及文件存取
//对内网的主机的设置
struct HostChar{
	    char HostIp[24];         //主机类ip范围
	    char TcpSrcOut[30][16];  //允许出去的tcp源端口
	    char TcpDstOut[30][16];  //允许出去的tcp目的端口
	    char UdpSrcOut[30][16];  //允许出去的udp源端口
	    char UdpDstOut[30][16];  //允许出去的udp目的端口
	    char IcmpTypeIn[30][5];  //允许进来的icmp类型
	    char IcmpTypeOut[30][5]; //允许出去的icmp类型
		int  nPos[10];           
};


struct  Nat_CfgChar{
//        ULONG   nGlobalAddress; //外部地址
//		ULONG   nTotalIntraIp;//禁止NAT的所有内部地址数量;当对DenyIntraAddr付值的同时对该成员付值
//        USHORT  nDenyIntraAddrlen;// struct  IpaddrRange的数量，即子网数
//       char    DenyIntraAddr[100][24];//禁止NAT的所有内部地址
//        int     nInternalNetWorkNumber;     //内部地址网络数量(网段数)
//        char    NetworkAddress[50][20];   //内部网络地址 界面未加。。。。。。。。。。。。                
        UINT     nTimeOut_TCP;           //动态端口转换的时间溢出值
        UINT     nTimeOut_UDP;           //动态端口转换的时间溢出值
        UINT  nLowPort;             //最低可用端口号 
        UINT  nHighPort;            //最高可用端口号
        UINT  nMaxNatNumber;        //最大端口转换数量
        USHORT   us_PortNumberTcp;          //tcp静态端口映射数量
		USHORT   us_PortNumberUdp;
		char  MapStaticTcp[50][24];//tcp静态端口NAT映射
		char  MapStaticUdp[50][24];//udp静态端口NAT映射
};
struct  IpFilter{
	BOOL isLogIp;			//1,要进行IP过滤层日志；
	DWORD Options;//bit0, inbroad;  //1，要记录入组播；
				//bit1,  outbroad;//1,要记录出组播
				//bit 2,incommingPort; //1，要记录进入端口；
				//bit 3,outgoingPort; //1，要记录出去端口；
				//bit 4,inlength;//1,要记录入协议头太短
				//bit 5,outlength;//1,要记录出协议头太短
				//bit 6, incommingType; //1，要记录进入ICMP类型；
				//bit 7, outgoingType; //1，要记录出去ICMP类型；
				//bit 8, incommingIP; //1，要记录进入IP；
				//bit 9, outgoingIP;	//1，要记录出去IP；
				//bit 10,11,=0;//非IP;
				//bit 12,inoffset;//记录入可疑偏移量
				//bit 13,outoffset;//记录出可疑偏移量
				//bit 14,inmore;//记录入分段
				//bit 15,outmore;//记录出分段
				//bit 16, incommingDOS; //1，要记录进入DOS；
				//bit 17, outgoingDOS; //1，要记录出去DOS；
};
struct ProCommentsLog{
	BOOL isLogProtocal;	//1,要进行协议内容过滤日志；
	DWORD Options;	    //bit 0,记录IP和端口；
						//bit 1,记录协议类型；
						//bit 2,记录阻断URL；
						//bit 3,记录时间
};
struct DefaultSetData{
	DWORD Priority;			//防火墙安全等级高度：
							//0，High;1,Moderate;2,Low;
	DWORD CheckINorOUTNetCard;	//bit 0,IN,=1,check;
								//bit 1,OUT,=1,check;
								//高中级别时，都检查，低，检查外网。
	DWORD Pro;//BIT 0,ICMP,PERMIT OR DENY INtoOUT(1 OR 0);
			//BIT 1,ICMP,PERMIT OR DENY OUTtoIN(1,OR 0);
			// BIT 2,IGMP,PERMIT OR DENY INtoOUT(1 OR 0);
			//BIT 3,IGMP,PERMIT OR DENY OUTtoIN(1,OR 0);
			//注释：级别高时，全0；低时，全1；
			//级别中间，ICMP，IGMP 0；
	int TCPProtocal; //BIT 0,PERMIT OR DENY IN 80(1 OR 0);
					//BIT 1,PERMIT OR DENY OUT 80(1,OR 0);
					//BIT 2,PERMIT OR DENY IN FTP(1 OR 0);
					//BIT 3,PERMIT OR DENY OUT FTP(1,OR 0);
					//BIT 4,PERMIT OR DENY IN NBT(1 OR 0);
					//BIT 5,PERMIT OR DENY OUT NBT(1,OR 0);
					//BIT 6,PERMIT OR DENY IN TELNET(1 OR 0);
					//BIT 7,PERMIT OR DENY OUT TELNET(1,OR 0);
					//BIT 8,SNMP,PERMIT OR DENY IN(1 OR 0);
					//BIT 9,SNMP,PERMIT OR DENY OUT(1,OR 0);
					//BIT 10, IN and OUT FINGER，79，PERMIT OR DENY OUT(1,OR 0);
					//BIT 11,内部出去端口<=1024,但除以上端口外：
					//1， PERMIT；0，DENY；
					//BIT 12，内部出去端口>1024:1，PERMIT；0，DENY；
					//bit 13,外部进来端口>1024：1，PERMIET；0，DENY；
					//BIT 14，外部进来端口<=1024，但除以上端口外：1，PERMIET；0，DENY；
										//bit 15-31,保留
										//级别高时，BIT 0、1、12位，=1，其余=0；
					//级别中时，BIT 10、13=0，其余=1；
					//级别低时，BIT 10为1；

	int PortocalComments;
					//BIT 0,PERMIT OR DENY IN JAVA(1,OR 0);
					//BIT 1,PERMIT OR DENY IN SCRIPT(1,OR 0);
					//BIT 2,PERMIT OR DENY IN ACTIVEX (1,OR 0);
					//BIT 3-31,保留，Priority=高，全0；低，1；
					//注释：级别高中时，全0；低时，全1；
	DWORD UDPProtocal;//BIT 0，OuterNetwork DNS,1,permit,0,deny;
						//保留，Priority=高，全0；低，全1；
};


struct  LogManage{
	BOOL RequireLog;	//1，需要日志记录管理；0，不需要
	IpFilter TheIpFilter;
	ProCommentsLog TheProCommentsLog;
};
//内网、外网、DM区设置
struct  FireWallConfigChar{
          char   csOutsideIp[16];//外网ip
		  char   csOutsideMask[16];//外网地址掩码
		  char   csMiddleIp[16];//DM区IP
		  char   csMiddleMask[16];
          char   csIntraIp[16];
		  char   csIntraMask[16];
 //         struct  Filterdrv_Cfg   sFilterdrvCfg;
          struct  Nat_CfgChar    sNatCfg;         
};
//该结构作文件存取，界面显示，初始化配置等用
struct AllLineChar{
        struct GlobalFlags  sGlobalFlags;
	    ULONG  CurHostNum;//Hostchar的当前下标
	    struct HostChar Hostchar[100];
		int    AllowPos;
	    char   AllowIpTab[100][24];
		int    RejPos;
		char   RejIpTab[100][24];
		struct FireWallConfigChar FireWallCfg;
		int     InDmPos;
        char    InDmIpTab[100][24];
		int     OutDmPos;
        char    OutDmIpTab[100][24];
};

//静态端口NAT映射
struct MapStaticPortNatChar{
          char  ServicePort[16];//标准服务端口
          char  IntraIpaddr[24];//内网ip地址
};

typedef struct LogView{
	char time[30];
	char prot[10];
	char src[24];
	char dst[24];
	char demo[20];
}LOGVIEW;
struct LogView1{
	char time[30];
	char keyword[20];
	char URL[250];
	char src[24];
	char dst[24];
	char demo[20];
};
typedef struct LogViewIcmp{
	char time[30];
	char prot[10];
	char src[24];
	char dst[24];
	char demo[20];
	char type[3];
}LOGVIEWICMP;
 
#define M_LOGVIEW WM_USER+1111
enum{TCPSRCOUT=0,TCPDSTOUT,UDPSRCOUT,UDPDSTOUT,ICMPTYPEIN,ICMPTYPEOUT};
#endif