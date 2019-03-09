
//对日志和流量统计都将有所改变
//IP源路由
//注意:类表仅仅用于对内网主机的设置
//内网访问的是允许表
//注意:覆盖表是当内网到外网时使用的对外部主机ip地址的设置（仅当tcp使用）
#include "StdAfx.h"
#include "ip.h"
#include "ipfilter.h"
#include "winsock2.h"
#include "Struct.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern DefaultSetData TheDefaultSetData;
struct LogView  logview;
struct LogViewIcmp  logviewIcmp;
extern USHORT logType;

 Statistics filterstats = { NO }; 	//缺省运行状态
 FilterConfig filtercfg = { DEFAULT_SYSL_MASK };	//缺省日志掩码
 ULONG ptrs_initialized = 0;      
 ULONG initialized = NO;
extern HWND myhWnd1;

 pointerStruct active, newspace;
 FILE *hlogFile,*statFile;
 int filtercount=0;
 struct pkt_struct *mtodPkt(BYTE *r);
 static void freeOutTree (OutaddrTreeNode *tbl, unsigned char level);
 VOID GetStatisticsInfo(Statistics* StatisticInfo);
 USHORT treeOutLookup (OutaddrTreeNode *tbl, ULONG addr, unsigned short max_group);
 static void 
 syslogMessage(unsigned char eventNo,ULONG protocolNo=NULL,ULONG srcAddr=NULL,
			  ULONG dstAddr=NULL, unsigned short srcPort=NULL,unsigned short dstPort=NULL,
			  unsigned char icmptype=NULL,UCHAR dostype=NULL);
 void writestatistic();
 VOID clearipfilter();
 //释放主结构空间
 static void freeAllPointers (pointerStruct *p);
//初始化主结构，将groups设置为1（1各主机类）
 static ULONG  initTables (pointerStruct *p, unsigned short groups);
//释放地址树空间（pointerStruct中的指针）
 static void freeTree (addrTreeNode *tbl, unsigned char level);
//现在使用的函数
static ULONG
OutnetaddToTree(OutaddrTreeNode *tbl, in_addr_t addr,/*主机类起始地址,*/ 
			unsigned short hostnums,/*从主机类起始地址开始的主机数,*/
			unsigned short group/*组号与端口列表的组号一至*/);
static ULONG
GjaddToTree(addrTreeNode *tbl, in_addr_t addr/*主机类起始地址*/, 
			unsigned short hostnums/*从主机类起始地址开始的主机数*/,
			unsigned short group/*组号与端口列表的组号一至*/);
//将portListReq结构中的端口列表添加到（pointerStruct的）PortListEntry结构中
static ULONG copyGroup (portListReq *u, PortListEntry *kptr);
//设置端口列表请求到主结构中
static ULONG portListRequest (portListReq *u, pointerStruct *p);
//添加地址表请求
static ULONG addrTreeRequest (addrTreeReq *u);
//添加拒绝表请求（到主结构）
static ULONG rejaccTableRequest (ULONG id, rejaccTableReq *u, pointerStruct *p);
static int
OutrejaccTableRequest(int id, rejaccTableReq *u, pointerStruct *p);
//添加覆盖表请求
static ULONG overrideTableRequest (overrideTableReq *u, pointerStruct *p);
//重设(主结构)请求;更新当前配置
static ULONG manageTablesRequest (manageTablesReq *u);
//收索对应的地址
USHORT treeLookup (addrTreeNode *tbl, ULONG addr, unsigned short max_group);
//检查入站的tcp包
static ULONG checkIncomingTcp (ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort);
//检查出站的tcp包
static ULONG checkOutgoingTcp (ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort);
//检查入站的udp
static ULONG checkIncomingUdp (ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort);
//检查出站udp
static ULONG checkOutgoingUdp (ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort);
//检查入站的icmp类型
static ULONG checkIncomingIcmp (ULONG srcAddr, ULONG dstAddr, unsigned char type);
//检查入站的icmp类型
static ULONG checkOutgoingIcmp (ULONG srcAddr, ULONG dstAddr, unsigned char type);
//检查入DM的tcp(无)
static ULONG checkIncomingDMTcp (ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort);
//检查入DM的udp(无)
static ULONG checkIncomingDMUdp (ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort);
//检查入DM的icmp(无)
static ULONG checkIncomingDMIcmp(ULONG srcAddr, ULONG dstAddr, unsigned char type);
//检查拒绝表
static ULONG checkRejectTable (ip *ipHeader, ULONG srcAddr, ULONG dstAddr);
//static ULONG checkIncomingPacket (unsigned char *pkt,unsigned char protolen);
//检查入站包
static ULONG checkIncomingPacket(USHORT,BYTE*,ULONG,ULONG);
//检查允许表
static ULONG checkAcceptTable (struct ip *ipHeader, ULONG srcAddr, ULONG dstAddr);//END:GJ
//检查出站包
static ULONG checkOutgoingPacket (unsigned short protocol,unsigned char *pkt,unsigned short protolen);
//现无
static ULONG checkIncomingDM(ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort,UCHAR from);
//初始化包过滤请求
static ULONG initRequest (initReq *req);
//校验和
extern USHORT  GetCksum(USHORT* buffer,int size);
USHORT in_cksum_tcp(BYTE *buffer);//gj
BOOL   in_cksum_hdr(BYTE* buffer);
//input eth and calculate checksum
struct pkt_struct *mtodPkt(BYTE *r)
{
    struct pkt_struct *pkt;
	pkt=(struct pkt_struct *)r[14];
    return pkt;
}

//input eth and calculate checksum
USHORT in_cksum_tcp(BYTE* buffer)
{
    //BYTE* tempbuffer;
	USHORT  tcpleng;
	USHORT  tcpchecksum;
	USHORT*  tempushort;
	USHORT  IPleng;
	//
	BYTE*   tempbuffer;
	//set checksum=0;
	buffer[34+16] = 0;
	buffer[34+17] = 0;

    //getIPleng(total)
    ((BYTE*)(&IPleng))[0] = buffer[17];
    ((BYTE*)(&IPleng))[1] = buffer[16];
	//get tcpleng(total)
	tcpleng = IPleng - 20;
    
	//get pseudo head
		tempbuffer = new BYTE[tcpleng+12];
		memcpy(tempbuffer+12,buffer+34,tcpleng);
		tempbuffer[0] = buffer[26];
		tempbuffer[1] = buffer[27];
		tempbuffer[2] = buffer[28];
		tempbuffer[3] = buffer[29];
		tempbuffer[4] = buffer[30];
		tempbuffer[5] = buffer[31];
		tempbuffer[6] = buffer[32];
		tempbuffer[7] = buffer[33];
        tempbuffer[8] = 0;
		tempbuffer[9] = 6;
		tempbuffer[10] = ((BYTE*)(&tcpleng))[1];
		tempbuffer[11] = ((BYTE*)(&tcpleng))[0];
        
		tempushort = (USHORT*) tempbuffer;
		tcpchecksum = GetCksum(tempushort, tcpleng+12);
		buffer[34+16] = ((BYTE*)(&tcpchecksum))[0];
		buffer[34+17] = ((BYTE*)(&tcpchecksum))[1];
		return tcpchecksum;
}


//input eth and calculate checksum
BOOL in_cksum_hdr(BYTE* buffer)
{
	BYTE* tempbufferttt;
	USHORT* tempushortbuffer;
	USHORT  checksum;
	
	tempbufferttt = &buffer[14];
	tempushortbuffer = (USHORT* )tempbufferttt;
	tempushortbuffer[5] = 0;
    checksum = GetCksum(tempushortbuffer,20);
	tempushortbuffer[5] = checksum;

    return TRUE;
}
/*
//input buffer  and calculate checksum
USHORT  GetCksum(USHORT* buffer,ULONG size)
{
	DWORD cksum;
	USHORT OddByte;

	cksum=0;
	while(size>1)
	{
		cksum+=*buffer++;
		size-=2;
	}
	if(size==1)
	{
        OddByte = 0;
		*((unsigned char*)&OddByte)= *(unsigned char*)buffer;
		cksum+=OddByte;
	}
	cksum=(cksum>>16)+(cksum&0xffff);
	cksum+=(cksum>>16);
	cksum=~cksum;

    return (unsigned short)cksum;
}
*/


static ULONG checkIncomingDM(ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort,UCHAR from)
{
	USHORT group;
	//查找目的地址是否是组播
	if (IN_CLASSD(dstAddr)) //0xe0000000~0xef000000
	{
		if (filtercfg.discardMulticast)
		{
			filterstats.f_insideMcast++;
			syslogMessage(SYSL_OUT_CLASSD,IP_PROTO_TCP,
				srcAddr, dstAddr, srcPort, dstPort);
			return DISCARD_PKT;
		}
		return ALLOW_PKT;
	}
    if(from=FROMOUT)
	   group = treeOutLookup(active.Out_DmTree, srcAddr, active.num_groups);
	else
	   group = treeLookup(active.In_DmTree, srcAddr, active.num_groups);
    if(group==1)
       return ALLOW_PKT;
	syslogMessage(SYSL_IN_REJECT,NULL,
			         srcAddr, dstAddr);
    return DISCARD_PKT;
}

// 所有参数都是主机顺序。
// 注意：如果下面的表改变，在ipfilter.h中的enum结构也要改变。
// 注意: DM的日志
// log函数
static SyslogMessageEntry syslogMessages[] = {
	{ "入组播",				LOG_WARNING },//	SYSL_IN_CLASSD,
	{ "出组播",				LOG_WARNING },//	SYSL_OUT_CLASSD,
	{ "入端口",				LOG_WARNING },//	SYSL_IN_PORT,
	{ "出端口",				LOG_WARNING },//	SYSL_OUT_PORT,		/* only TCP */
	{ "入协议头太短",			LOG_WARNING },//	SYSL_IN_LENGTH,		/* TCP/UDP-Header maybe not initialized */
	{ "出协议头太短",			LOG_WARNING },//	SYSL_IN_DOS,		/* denial of service attack packet in */
	{ "入ICMP类型",				LOG_WARNING },//	SYSL_IN_TYPE,		/* only srcAddr, dstAddr, and icmp type */
	{ "出ICMP类型",				LOG_WARNING },//	SYSL_OUT_TYPE,		/* only srcAddr, dstAddr, and icmp type */
	{ "入拒绝IP",			LOG_WARNING },//	SYSL_IN_REJECT,		/* only protocolNo, srcAddr and dstAddr */
	{ "出拒绝IP",			LOG_WARNING },//	SYSL_OUT_ACCEPT,	/* only protocolNo, srcAddr and dstAddr */
	{ "入非IP",				LOG_NOTICE  },//	SYSL_IN_PROT,		/* only protocolNo, srcAddr and dstAddr */
	{ "出非IP",				LOG_WARNING },//	SYSL_OUT_PROT,		/* only protocolNo, srcAddr and dstAddr */
	{ "入偏移量为1",	LOG_WARNING },//	SYSL_IN_OFFSET,		/* suspect fragment offset */
	{ "出偏移量为1",	LOG_WARNING },//	SYSL_OUT_OFFSET,	/* suspect fragment offset */
	{ "入分段",				LOG_WARNING },//	SYSL_IN_FRAG,		/* fragmented packet */
	{ "出分段",				LOG_WARNING },//	SYSL_OUT_FRAG,		/* fragmented packet */
	{ "入D_O_S攻击",			LOG_WARNING },//	SYSL_IN_DOS,		/* denial of service attack packet in */
	{ "出D_O_S攻击",			LOG_WARNING },//	SYSL_OUT_DOS,		/* denial of service attack packet out */
	{"原路由攻击",LOG_WARNING},
	{"入ICMP",LOG_WARNING},
	{"出ICMP",LOG_WARNING},
	{"入IGMP",LOG_WARNING},
	{"出IGMP",LOG_WARNING},
};

static const char *syslogDosTypes[] = { "\"Smurf\"", "\"Pong\"" };
//Config.cpp中添加日志请求
static void
syslogMessage(unsigned char eventNo,ULONG protocolNo,ULONG srcaddr,
			  ULONG dstaddr, unsigned short srcport,unsigned short dstport,
			  unsigned char icmptype,UCHAR dostype)
{

	//如果该日志被禁止，就返回。
	if ( !((1UL << eventNo) & filtercfg.logMask))
		return;
	char tempchar[10];
	SYSTEMTIME  SystemTime;
	GetLocalTime(&SystemTime);
	char cdstaddr[16];
	char csrcaddr[16];
    struct in_addr dstin,srcin;
	dstin.S_un.S_addr=htonl(dstaddr);
	srcin.S_un.S_addr=htonl(srcaddr);
    memcpy(cdstaddr,inet_ntoa(dstin),16);
    memcpy(csrcaddr,inet_ntoa(srcin),16);
	switch ( eventNo ) {
		case SYSL_IN_OFFSET:
		case SYSL_OUT_OFFSET:
		case SYSL_IN_FRAG:
		case SYSL_OUT_FRAG:
		case SYSL_IN_REJECT:
		case SYSL_OUT_ACCEPT:
		case SYSL_IN_PROT: 
		case SYSL_OUT_PROT: 
		case SYSL_OUT_ROUT:
        case SYSL_IN_ICMP:
		case SYSL_OUT_ICMP:
		case SYSL_IN_IGMP:
		case SYSL_OUT_IGMP:
		{	/* IP - general */
		    	switch(protocolNo)
				{
		    	case IP_PROTO_TCP:
				    strcpy(tempchar,"TCP");
			    	break;
			    case IP_PROTO_UDP:
				    strcpy(tempchar,"UDP");
				    break;
			    case IP_PROTO_ICMP:
				    strcpy(tempchar,"ICMP");
				    break;
				case	IP_PROTO_IGMP:
					strcpy(tempchar,"IGMP");
			    default:
					strcpy(tempchar,"");
				    break;
			}
/*			sprintf(filterlog.str[filterlog.pos],"时间：%d月%d日%d:%d:%d\t说明：%s\t协议：%s\tFrom:%s" "To:%s\n",
				SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				SystemTime.wMinute,SystemTime.wSecond,syslogMessages[eventNo].message,
				tempchar,csrcaddr,cdstaddr);
			filterlog.pos++;
*/          if(logType==1)
			{
            sprintf(logview.time,"%d月%d日%d:%d:%d",SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				      SystemTime.wMinute,SystemTime.wSecond);
            strcpy(logview.prot,tempchar);
			strcpy(logview.src,csrcaddr);
			strcpy(logview.dst,cdstaddr);
			strcpy(logview.demo,syslogMessages[eventNo].message);
            //PostMessage(AfxGetMainWnd()->GetSafeHwnd(),M_LOGVIEW,(WPARAM)&logview,(LPARAM)111);
            PostMessage(myhWnd1,M_LOGVIEW,(WPARAM)&logview,(LPARAM)111);
			}
			fprintf(hlogFile,"时间：%d月%d日%d:%d:%d\t说明：%s\t协议：%s\tFrom:%s" "To:%s\n"
				,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				SystemTime.wMinute,SystemTime.wSecond,syslogMessages[eventNo].message,
				tempchar,csrcaddr,cdstaddr);
              filtercount++;
			break;
			}
		case SYSL_IN_CLASSD:
		case SYSL_OUT_CLASSD:
		case SYSL_IN_PORT:
		case SYSL_OUT_PORT:
		case SYSL_IN_LENGTH:
		case SYSL_OUT_LENGTH:
			{	/* IP - TCP/UDP */
			  strcpy(tempchar,(protocolNo == IP_PROTO_UDP) ? "UDP" : "TCP");
/*			  sprintf(filterlog.str[filterlog.pos],"时间：%d月%d日%d:%d:%d\t说明：%s\t协议：%s\tFrom%s:%d"  "To%s:%d\n",
				SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				SystemTime.wMinute,SystemTime.wSecond,syslogMessages[eventNo].message,
				tempchar,csrcaddr,ntohs(srcport),cdstaddr,ntohs(dstport));	
			  filterlog.pos++;
*/
			if(logType==1)
            {
            memset((char*)&logview,0,sizeof(LOGVIEW));
			sprintf(logview.time,"%d月%d日%d:%d:%d",SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				      SystemTime.wMinute,SystemTime.wSecond);
            strcpy(logview.prot,tempchar);
			sprintf(logview.src,"%s:%d",csrcaddr,ntohs(srcport));
			sprintf(logview.dst,"%s:%d",cdstaddr,ntohs(dstport));
			strcpy(logview.demo,syslogMessages[eventNo].message);
            PostMessage(myhWnd1,M_LOGVIEW,(WPARAM)&logview,(LPARAM)111);
 //           PostMessage(AfxGetMainWnd()->GetSafeHwnd(),M_LOGVIEW,(WPARAM)&logview,(LPARAM)111);
			}

			  fprintf(hlogFile,"时间：%d月%d日%d:%d:%d\t说明：%s\t协议：%s\tFrom%s:%d"  "To%s:%d\n"
				,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				SystemTime.wMinute,SystemTime.wSecond,syslogMessages[eventNo].message,
				tempchar,csrcaddr,ntohs(srcport),cdstaddr,ntohs(dstport));	
              filtercount++;
	          break;
			}

		case SYSL_IN_TYPE: 	/* IP - ICMP */
		case SYSL_OUT_TYPE:
			{
/*			  sprintf(filterlog.str[filterlog.pos],"时间：%d月%d日%d:%d:%d\t说明：%s\tFrom%s"  "To%s\tICMP类型：%d\n",
				SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				SystemTime.wMinute,SystemTime.wSecond,syslogMessages[eventNo].message,
				csrcaddr,cdstaddr,icmptype);	
			  filterlog.pos++;
*/
		    strcpy(tempchar,"ICMP");
			if(logType==1)
			{
            memset((char*)&logviewIcmp,0,sizeof(LOGVIEWICMP));
			sprintf(logviewIcmp.time,"%d月%d日%d:%d:%d",SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				      SystemTime.wMinute,SystemTime.wSecond);
            strcpy(logviewIcmp.prot,tempchar);
			strcpy(logviewIcmp.src,csrcaddr);
			strcpy(logviewIcmp.dst,cdstaddr);
			strcpy(logviewIcmp.demo,syslogMessages[eventNo].message);
			sprintf(logviewIcmp.type,"%d",icmptype);
   //         PostMessage(AfxGetMainWnd()->GetSafeHwnd(),M_LOGVIEW,(WPARAM)&logviewIcmp,(LPARAM)112);
            PostMessage(myhWnd1,M_LOGVIEW,(WPARAM)&logviewIcmp,(LPARAM)112);
			}

			  fprintf(hlogFile,"时间：%d月%d日%d:%d:%d\t说明：%s\tFrom%s"  "To%s\tICMP类型：%d\n",
				SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				SystemTime.wMinute,SystemTime.wSecond,syslogMessages[eventNo].message,
				csrcaddr,cdstaddr,icmptype);
              filtercount++;
              break;
			}

		case SYSL_IN_DOS:	/* DOS 攻击 */
		case SYSL_OUT_DOS:
			{
/*			  sprintf(filterlog.str[filterlog.pos],"时间：%d月%d日%d:%d:%d\t说明：%s\tDOS类型：%d\tFrom%s"  "To%s\n",
				SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
			  SystemTime.wMinute,SystemTime.wSecond,syslogMessages[eventNo].message,syslogDosTypes[dostype],
			    csrcaddr,cdstaddr);	
			  filterlog.pos++;
*/
            if(logType==1)
            {
            memset((char*)&logview,0,sizeof(LOGVIEW));
			sprintf(logview.time,"%d月%d日%d:%d:%d",SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
				      SystemTime.wMinute,SystemTime.wSecond);
			strcpy(logview.src,csrcaddr);
			strcpy(logview.dst,cdstaddr);
			strcpy(logview.demo,syslogMessages[eventNo].message);
    //        PostMessage(AfxGetMainWnd()->GetSafeHwnd(),M_LOGVIEW,(WPARAM)&logview,(LPARAM)111);
            PostMessage(myhWnd1,M_LOGVIEW,(WPARAM)&logview,(LPARAM)111);
			}	

			  fprintf(hlogFile,"时间：%d月%d日%d:%d:%d\t说明：%s\tDOS类型：%d\tFrom%s"  "To%s\n"
				,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,
			  SystemTime.wMinute,SystemTime.wSecond,syslogMessages[eventNo].message,syslogDosTypes[dostype],
			    csrcaddr,cdstaddr);
              filtercount++;
			  break;
			}
/*
			if(filterlog.pos==1000)
			{
	            fwrite(filterlog.str,sizeof(char),filterlog.pos*100,hlogFile);
				memset(&filterlog,0,sizeof(FilterLog));
				filterlog.pos=0;
                writestatistic();
				fprintf(statFile,"\n");
				fclose(statFile);
	            fclose(hlogFile);
				hlogFile=fopen("日志.txt","a");
	            statFile=fopen("统计.txt","a");
			}
*/	
			if(filtercount==100)
			{
				memset(&filterstats,0,sizeof(Statistics));
				filtercount=0;
                writestatistic();
				fprintf(statFile,"\n");
				fclose(statFile);
	            fclose(hlogFile);
				hlogFile=fopen("日志.txt","a");
	            statFile=fopen("统计.txt","a");
			}



	}

}

/* ------------------------------------------------------------------------ */

static void
freeAllPointers(pointerStruct *p)
{//在free函数中去掉了M_DEVBUF参数；此参数在头文件中
	if (p->addrTree)	freeTree(p->addrTree, 0);
	if (p->t_dst_out)	free(p->t_dst_out);
        if (p->t_src_out)	free(p->t_src_out);
        if (p->u_dst_out)	free(p->u_dst_out);
        if (p->u_src_out)	free(p->u_src_out);
        if (p->i_typ_in)	free(p->i_typ_in);
        if (p->i_typ_out)	free(p->i_typ_out);
        if (p->t_rst_in)	free(p->t_rst_in);
        if (p->t_rst_out)	free(p->t_rst_out);
        if (p->acceptTable)	free(p->acceptTable);
        if (p->acceptTree)	freeTree(p->acceptTree, 0);
        if (p->rejectTable)	free(p->rejectTable);
        if (p->rejectTree)	freeOutTree(p->rejectTree, 0);
        if(p->In_DmTree)freeTree(p->In_DmTree,0);
        if(p->Out_DmTree)freeOutTree(p->Out_DmTree,0);

	memset(p,0,sizeof(pointerStruct));
}

/* ------------------------------------------------------------------------ */
//groups:最多可设定的主机类的数目
static ULONG
initTables(pointerStruct *p, unsigned short groups)
{
	/* 必须至少有一类 */
	if (groups == 0)
		groups = 1;

	/* 释放所有由指针结构分配的空间*/
	freeAllPointers(p);

	/* 建立一个缺省配置 */
	p->addrTree  = MALLOC_NODE;

 //groups*32*sizeof(PortListEntry)
	p->t_dst_out = MALLOC_GROUP(groups);
	p->t_src_out = MALLOC_GROUP(groups);
	p->u_dst_out = MALLOC_GROUP(groups);
	p->u_src_out = MALLOC_GROUP(groups);
	p->i_typ_in  = MALLOC_GROUP(groups);
	p->i_typ_out = MALLOC_GROUP(groups);
	p->t_rst_in  = MALLOC_GROUP(groups);
	p->t_rst_out = MALLOC_GROUP(groups);
	if ((p->addrTree == NULL) ||
	    (p->t_dst_out == NULL) ||(p->t_src_out == NULL) ||
        (p->u_dst_out == NULL) ||(p->u_src_out == NULL) ||
	    (p->i_typ_in  == NULL) || (p->i_typ_out == NULL) ||
	    (p->t_rst_in  == NULL) || (p->t_rst_out == NULL)) {
		freeAllPointers(p);
		return ERROR_NO_MEMORY;
	}
	p->num_groups = groups;
	memset(p->addrTree, 0, sizeof(addrTreeNode));
	memset(p->t_dst_out,0,groups * LIST_TABLE_SIZE);
	memset(p->t_src_out,0,groups * LIST_TABLE_SIZE);
	memset(p->u_dst_out,0,groups * LIST_TABLE_SIZE);
	memset(p->u_src_out,0,groups * LIST_TABLE_SIZE);
	memset(p->i_typ_in,0,groups * LIST_TABLE_SIZE);
	memset(p->i_typ_out,0,groups * LIST_TABLE_SIZE);
	memset(p->t_rst_in,0,LIST_TABLE_SIZE);
	memset(p->t_rst_out,0,LIST_TABLE_SIZE);

	/* 填写缺省类表*/
	switch(TheDefaultSetData.Priority)
	{
	case 0://high
        p->t_dst_out[0].begin = 2;	// TCP - 1个 
        p->t_dst_out[1].begin = 80;	// TCP -http out    
        p->t_dst_out[1].end = 80;
        p->t_dst_out[2].begin=1024;//>=1024 OUT
	    p->t_dst_out[2].begin=0xFFFF;
        
		p->t_src_out[0].begin=1;
	    p->t_src_out[1].begin=1024;
	    p->t_src_out[1].end=0xFFFF;

	    p->u_src_out[0].begin=1;
	    p->u_src_out[1].begin=1024;
        p->u_src_out[1].end=0xFFFF;
	    p->u_dst_out[0].begin = 1;	///* UDP - 1个
	    p->u_dst_out[1].begin = 1024;	///* UDP >=1024 out    
	    p->u_dst_out[1].end = 0xFFFF;

	    p->i_typ_in[0].begin = 1;	///* ICMP - 1个
	    p->i_typ_in[1].begin = 0;	///* ICMP - FORBID      
	    p->i_typ_in[1].end = 0;

	    p->i_typ_out[0].begin = 1;	//* ICMP - 1个
	    p->i_typ_out[1].begin = 0;	//* ICMP - FORBID       
	    p->i_typ_out[1].end = 0;
		break;
	case 1://middle
        p->t_dst_out[0].begin = 5;	// TCP - 1个 
        p->t_dst_out[1].begin = 80;	// TCP -http out    
        p->t_dst_out[1].end = 80;
        p->t_dst_out[2].begin =20;	// TCP -FTP TELNET out    
        p->t_dst_out[2].end = 23;
        p->t_dst_out[3].begin =137;	// TCP -NBT out    
        p->t_dst_out[3].end =139;
        p->t_dst_out[4].begin =161;	// TCP -SNMP out    
        p->t_dst_out[4].end = 162;	
		p->t_dst_out[5].begin=1024;//>=1024 OUT
	    p->t_dst_out[5].begin=0xFFFF;
        
		p->t_src_out[0].begin=1;
	    p->t_src_out[1].begin=0;
	    p->t_src_out[1].end=0xFFFF;

	    p->u_src_out[0].begin=1;
	    p->u_src_out[1].begin=0;
        p->u_src_out[1].end=0xFFFF;
	    p->u_dst_out[0].begin = 1;	///* UDP - 1个
	    p->u_dst_out[1].begin = 1024;	///* UDP >=1024 out    
	    p->u_dst_out[1].end = 0xFFFF;

	    p->i_typ_in[0].begin = 1;	///* ICMP - 1个
	    p->i_typ_in[1].begin = 0;	///* ICMP - everything out 
	    p->i_typ_in[1].end = 0xFF;

	    p->i_typ_out[0].begin = 1;	//* ICMP - 1个
	    p->i_typ_out[1].begin = 0;	//* ICMP - everything out       
	    p->i_typ_out[1].end = 0xFF;
		break;
	case 2://low
        p->t_dst_out[0].begin = 5;	// TCP - 1个 
        p->t_dst_out[1].begin = 80;	// TCP -http out    
        p->t_dst_out[1].end = 80;
        p->t_dst_out[2].begin =20;	// TCP -FTP TELNET out    
        p->t_dst_out[2].end = 23;
        p->t_dst_out[3].begin =137;	// TCP -NBT out    
        p->t_dst_out[3].end =139;
        p->t_dst_out[4].begin =161;	// TCP -SNMP out    
        p->t_dst_out[4].end = 162;	
		p->t_dst_out[5].begin=1024;//>=1024 OUT
	    p->t_dst_out[5].begin=0xFFFF;
        
		p->t_src_out[0].begin=1;
	    p->t_src_out[1].begin=0;
	    p->t_src_out[1].end=0xFFFF;

	    p->u_src_out[0].begin=1;
	    p->u_src_out[1].begin=0;
        p->u_src_out[1].end=0xFFFF;
	    p->u_dst_out[0].begin = 2;	///* UDP - 1个
	    p->u_dst_out[1].begin = 53;	///* UDP dns out    
	    p->u_dst_out[1].end = 53;
	    p->u_dst_out[1].begin =1024;
	    p->u_dst_out[1].end = 0xFFFF;
	    p->i_typ_in[0].begin = 1;	///* ICMP - 1个
	    p->i_typ_in[1].begin = 0;	///* ICMP - everything out 
	    p->i_typ_in[1].end = 0xFF;

	    p->i_typ_out[0].begin = 1;	//* ICMP - 1个
	    p->i_typ_out[1].begin = 0;	//* ICMP - everything out       
	    p->i_typ_out[1].end = 0xFF;
		break;
	default://high
        p->t_dst_out[0].begin = 2;	// TCP - 1个 
        p->t_dst_out[1].begin = 80;	// TCP -http out    
        p->t_dst_out[1].end = 80;
        p->t_dst_out[2].begin=1024;//>=1024 OUT
	    p->t_dst_out[2].begin=0xFFFF;
        
		p->t_src_out[0].begin=1;
	    p->t_src_out[1].begin=1024;
	    p->t_src_out[1].end=0xFFFF;

	    p->u_src_out[0].begin=1;
	    p->u_src_out[1].begin=1024;
        p->u_src_out[1].end=0xFFFF;
	    p->u_dst_out[0].begin = 1;	///* UDP - 1个
	    p->u_dst_out[1].begin = 1024;	///* UDP >=1024 out    
	    p->u_dst_out[1].end = 0xFFFF;

	    p->i_typ_in[0].begin = 1;	///* ICMP - 1个
	    p->i_typ_in[1].begin = 0;	///* ICMP - FORBID      
	    p->i_typ_in[1].end = 0;

	    p->i_typ_out[0].begin = 1;	//* ICMP - 1个
	    p->i_typ_out[1].begin = 0;	//* ICMP - FORBID       
	    p->i_typ_out[1].end = 0;
	}
	return NOO_ERROR;
}

/*-----------------------------------------------------------------------*/

static void
freeTree(addrTreeNode *tbl, unsigned char level)
{
        ULONG i;

        if (!tbl || (level >1)) 
		{
		//参数错误
                return;
		}

        for (i=0; i<256; i++) 
		{
                if (tbl->u[i].pd)
				{
			        free(tbl->u[i].pd);
				}
		}

	free(tbl);
}

static ULONG
copyGroup(portListReq *u, PortListEntry *kptr)
{
	if (kptr == NULL)
		return ERROR_NET_NOT_FOUND; 
	//准确的添加到尾
	 USHORT pos=kptr[0].begin;
     kptr[0].begin+=u->bytes/sizeof(PortListEntry);
	 if(pos==0)
		 pos++;
	 memcpy(&kptr[pos],u->ptr, u->bytes);//需要判断已经设定的端口数
	 return u->bytes;
}

/* ------------------------------------------------------------------------ */

static ULONG
portListRequest(portListReq *u, pointerStruct *p)
{
	//u->group:当前的主机类
	ULONG byte=u->bytes*sizeof(PortListEntry);
	ULONG ret=u->group + ((byte/ LIST_TABLE_SIZE) +
		        (byte% LIST_TABLE_SIZE) ? 1 : 0);
	if (ret > p->num_groups)
		return ERROR_OUT_OF_RANGE;

	switch (u->id) {
	case T_DST_OUT:
		return copyGroup(u ,&(p->t_dst_out[(u->group-1)  * LIST_SIZE]));
	case T_SRC_OUT:
		return copyGroup(u ,&(p->t_src_out[(u->group-1) * LIST_SIZE]));
	case U_DST_OUT:
		return copyGroup(u ,&(p->u_dst_out[(u->group-1) * LIST_SIZE]));
	case U_SRC_OUT:
		return copyGroup(u ,&(p->u_src_out[(u->group-1) * LIST_SIZE]));
	case I_TYP_IN:
		return copyGroup(u ,&(p->i_typ_in[(u->group-1) * LIST_SIZE]));
	case I_TYP_OUT:
		return copyGroup(u ,&(p->i_typ_out[(u->group-1) * LIST_SIZE]));
	case T_RST_IN:
		if (u->group > 1)
			return ERROR_OUT_OF_RANGE;
		return copyGroup(u, p->t_rst_in);
	case T_RST_OUT:
		if (u->group > 1)
			return ERROR_OUT_OF_RANGE;
		return copyGroup(u, p->t_rst_out);
	default:
		return ERROR_UNKNOWN_GROUP;
	}
}

/* ------------------------------------------------------------------------ */

static ULONG
addrTreeRequest(addrTreeReq *u)
{
	register unsigned char i;
    if (newspace.addrTree == NULL) 
			return ERROR_NET_NOT_FOUND; 

		/* Add the specified IP cidr(s) to the 'newspace' config */
	//最多128个ip地址（可能是每个主机类的起始ip）(即最多128个主机类)
		for (i=0; (i < u->num) && (i < 128); i++) {
			if (GjaddToTree(newspace.addrTree, u->addr[i],
				     u->bits[i], u->group[i])) {
				freeTree(newspace.addrTree, 0);
				newspace.addrTree = NULL;
				return ERROR_NO_MEMORY;
			}
		}
	return NOO_ERROR;
}

/* ------------------------------------------------------------------------ */
static int
OutrejaccTableRequest(int id, rejaccTableReq *u, pointerStruct *p)
{
	register unsigned char i;
	RejAccTableEntry *table;   //允许拒绝表
	OutaddrTreeNode *tree;        //地址树，用于包过滤
	//u->bytes必须是sizeof(RejAccTableEntry)的整数倍
	if ((u->bytes == 0) || (u->bytes % sizeof(RejAccTableEntry)))
		return ERROR_INVALID_OP;

	//为树分配空间		 
	table = (RejAccTableEntry *)malloc(u->bytes);//,M_DEVBUF,M_NOWAIT);
	if (table == NULL)
		return ERROR_NO_MEMORY;
	tree = MALLOC_OUTNODE;
	if (tree == NULL)
	{
		free(table);//);
		return ERROR_NO_MEMORY;
	} 
	memset(tree,0,sizeof(addrTreeNode));
	memset(table,0, u->bytes);
    memcpy(table, u->ptr, u->bytes);//gj
//		return u->bytes;//gj
		// Populate the tree from the table - keep the table around
		// to spit back out when requested by the user interface.
	for (i=u->bytes/sizeof(RejAccTableEntry); i; i--) {
		register int r;
		//将拒绝的IP地址添加到树
		if ((r=OutnetaddToTree(tree, table[i-1].network,
			table[i-1].bits, table[i-1].flag))) {
			freeOutTree(tree, 0);
			free(table);//);
			return (r>0) ? ERROR_NO_MEMORY :
			ERROR_INVALID_OP;
			}
		}
	//传到主结构
	 switch(id)
	 {
	   case T_REJTBL:
		if ((p->rejectTable != NULL) || (p->rejectTree != NULL))
		{
		free(p->rejectTable);
		freeOutTree(p->rejectTree,0);
		} 
		p->rejectTable = table;
		p->rejectTree = tree;
		p->reject_bytes = u->bytes;
		break;
	   case T_OUTDM:
		 if (p->Out_DmTree!= NULL)
		freeOutTree(p->Out_DmTree,0);
		p->Out_DmTree = tree;
		free(table);
		   break;
	   default:
		   freeOutTree(tree,0);
		   free(table);
		   return ERROR_INVALID_OP;
	 }
		return NOO_ERROR;
}


/* ------------------------------------------------------------------------ */
static ULONG
rejaccTableRequest(ULONG id, rejaccTableReq *u, pointerStruct *p)
{
	register unsigned char i;
	RejAccTableEntry *table;   //允许拒绝表
	addrTreeNode *tree;        //地址树，用于包过滤
	//u->bytes必须是sizeof(RejAccTableEntry)的整数倍
	if ((u->bytes == 0) || (u->bytes % sizeof(RejAccTableEntry)))
		return ERROR_INVALID_OP;

	//为树分配空间		 
	table = (RejAccTableEntry *)malloc(u->bytes);//,M_DEVBUF,M_NOWAIT);
	if (table == NULL)
		return ERROR_NO_MEMORY;
	tree = MALLOC_NODE;
	if (tree == NULL)
	{
		free(table);//);
		return ERROR_NO_MEMORY;
	} 
	memset(tree,0,sizeof(addrTreeNode));
	memset(table,0, u->bytes);
    memcpy(table, u->ptr, u->bytes);//gj
//		return u->bytes;//gj
		// Populate the tree from the table - keep the table around
		// to spit back out when requested by the user interface.
	for (i=u->bytes/sizeof(RejAccTableEntry); i; i--) {
		register ULONG r;
		//将拒绝的IP地址添加到树
		if ((r=GjaddToTree(tree, table[i-1].network,
			table[i-1].bits, table[i-1].flag))) {
			freeTree(tree, 0);
			free(table);//);
			return (r>0) ? ERROR_NO_MEMORY :
			ERROR_INVALID_OP;
			}
		}
	//传到主结构
	 switch(id)
	 {
	   case T_ACCTBL:
		if ((p->acceptTree != NULL) || (p->acceptTable!= NULL))
		{
		free(p->acceptTable);
		freeTree(p->acceptTree,0);
		} 
		p->acceptTable = table;
		p->acceptTree = tree;
		p->accept_bytes = u->bytes;
		break;
	   case T_INDM:
		 if (p->In_DmTree != NULL)
		  freeTree(p->In_DmTree,0);
		   p->In_DmTree = tree;
		free(table);
		   break;
	   default:
		   freeTree(tree,0);
		   free(table);
		   return ERROR_INVALID_OP;
	 }
		return NOO_ERROR;
}

static ULONG
manageTablesRequest(manageTablesReq *u)
{
	pointerStruct tmp;
	unsigned long rcode;
	switch (u->command) {
	//新配置设为当前配置，并清除原有配置
	case ACTIVATE_NEW:
		if ((newspace.num_groups == 0) || (newspace.addrTree == NULL) ||
		    (newspace.t_dst_out == NULL) ||(newspace.t_src_out == NULL) ||
           (newspace.u_dst_out == NULL) ||(newspace.u_src_out == NULL) ||
		    (newspace.i_typ_in == NULL) || (newspace.i_typ_out == NULL) ||
		    (newspace.t_rst_in == NULL) || (newspace.t_rst_out == NULL))
			return ERROR_NET_NOT_FOUND;
		memcpy(&tmp, &active, sizeof(pointerStruct));
		memcpy(&active, &newspace, sizeof(pointerStruct));
		memset(&newspace,0,sizeof(pointerStruct));
		freeAllPointers(&tmp);
		break;
	//将用户配置设为缺省配置
	case CLEAR_ALL:
		rcode = initTables(&newspace, 1);
		if (rcode)
		 return rcode;
		memcpy(&tmp, &active, sizeof(pointerStruct));
		memcpy(&active, &newspace, sizeof(pointerStruct));
		memset(&newspace,0,sizeof(pointerStruct));
		freeAllPointers(&tmp);
		break;

	//在载入新配置前要重新初始化
	case INIT_NEW:
		rcode = initTables(&newspace, u->val);
		if (rcode)
			return rcode;
		break;
	}
	return NOO_ERROR;
}

/*-----------------------------------------------------------------------*/
/*
 *查找IP地址所属的类
 */
USHORT
treeOutLookup(OutaddrTreeNode *tbl, ULONG addr, unsigned short max_group)
{
    addrTreePtr t1,t2;
	in_addr_t ip;
	if (tbl == NULL)
		return DEFAULT_ACCESS;
//	ip = (in_addr_t)addr;
	ip.ss_addr=addr;//gj..................
        //查找地址的第一个八位     
		//flg=1: group ;   flg=0: pointer
        if ((t1.p = tbl->u[ip.s_b[3]].p) == NULL)
                return DEFAULT_ACCESS;
        if (tbl->flg[ip.s_b[3]])
                return ((t1.c < max_group) ? t1.c : DEFAULT_ACCESS);

        //查找地址的第二个八位
        //flg=1: group ;   flg=0: pointer
        // t1.p = tbl->u[ip.s_b[3]].p
      if ((t2.p = t1.p->u[ip.s_b[2]].p) == NULL)
                return DEFAULT_ACCESS;
        if (t1.p->flg[ip.s_b[2]])
                return ((t2.c < max_group) ? t2.c : DEFAULT_ACCESS);

		//begin gj  //查找地址的第三个八位
       if ((t1.p =t2.p->u[ip.s_b[1]].p) == NULL)
                return DEFAULT_ACCESS;
       if (t2.p->flg[ip.s_b[1]])
                return ((t2.c < max_group) ? t2.c : DEFAULT_ACCESS);      
		//end gj
		//flg=1: group;    flg=0: pointer
       //t2.p = tbl->u[ip.s_b[3]].p->u[ip.s_b[2]].p
/*        if ((t1.pd = t2.p->u[ip.s_b[1]].pd) == NULL)
			    return DEFAULT_ACCESS;
        if (t2.p->flg[ip.s_b[1]])
                return ((t1.c < max_group) ? t1.c : DEFAULT_ACCESS);
*/

		//查找地址的第四个八位
		//a group
        // * t1.pd = tbl->u[ip.s_b[3]].p->u[ip.s_b[2]].p->u[ip.s_b[1]].pd
        t2.c = t1.pd->c[ip.s_b[0]];
        return ((t2.c < max_group) ? t2.c : DEFAULT_ACCESS);
}


USHORT
treeLookup(addrTreeNode *tbl, ULONG addr, unsigned short max_group)
{
	in_addr_t ip;
	if (tbl == NULL)
		return DEFAULT_ACCESS;
//	ip = (in_addr_t)addr;
	ip.ss_addr=addr;//gj..................
        //查找地址的第一个八位     
		//flg=1: group ;   flg=0: pointer
	USHORT group;
	if(tbl->u[ip.s_b[1]].pd)
	{
		group=tbl->u[ip.s_b[1]].pd->c[ip.s_b[0]];
		return ((group<max_group)?group:DEFAULT_ACCESS);
	}
	else 
		return DEFAULT_ACCESS;
}

/* ------------------------------------------------------------------------ */
//参数是主机顺序
static ULONG
checkIncomingTcp(ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort)
{
	//检查是否组播
	if (IN_CLASSD(dstAddr))//dstAddr是组播地址
	{
		if (filtercfg.discardMulticast)
		{
			filterstats.f_outsideMcast++;
			syslogMessage(SYSL_IN_CLASSD, IP_PROTO_TCP,
			      	srcAddr, dstAddr, srcPort, dstPort);
			return DISCARD_PKT;
		}
		return ALLOW_PKT;
	}
	/*
	//通过目的地址从树中查找该地址所属的类
	group = treeLookup(active.addrTree, dstAddr, active.num_groups);
	if(group>0&&group<active.num_groups)
	   portList = &active.t_dst_in[(group-1) * LIST_SIZE];
	else
	    	return DISCARD_PKT;
	//到该地址对应的目的端口设置处
	//查找目的端口是否允许
	for (i=1; i <= portList[0].begin; i++)
	{
		if ((dstPort >= portList[i].begin) &&
		    (dstPort <= portList[i].end))
	    		return ALLOW_PKT;
	}
	//dstPort<1024则不检查源端口；直接禁止
	if (dstPort > 1024) 
	{
		//到该地址对应的源端口设置处
		portList = &active.t_src_in[group * LIST_SIZE];
		//比较源端口
		for (i=1; i <= portList[0].begin; i++)
		{
			if ((srcPort >= portList[i].begin) &&
			    (srcPort <= portList[i].end))
				return ALLOW_PKT;
		}
	}
	filterstats.f_outsideTcpPort++;
//	syslogMessage(SYSL_IN_PORT, IP_PROTO_TCP,
//		      srcAddr, dstAddr, (ULONG)srcPort, (ULONG)dstPort);
	//查找内网主机的TCP端口响应表（界面可加上）
	for (i=1; i <= active.t_rst_in[0].begin; i++)
	{
		if ((dstPort >= active.t_rst_in[i].begin) &&
		    (dstPort <= active.t_rst_in[i].end))
		{
			filterstats.outsideTCPreset++;
			return DISCARD_AND_ANSWER_PKT;
		}
	}
	*/
	return DISCARD_PKT;
}

/* ------------------------------------------------------------------------ */
static ULONG
checkOutgoingTcp(ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort)
{
	ULONG i;
	ULONG group;
	PortListEntry *portList;

	//查找目的地址是否是组播
	if (IN_CLASSD(dstAddr)) 
	{
		if (filtercfg.discardMulticast)
		{
			filterstats.f_insideMcast++;
		syslogMessage(SYSL_OUT_CLASSD,IP_PROTO_TCP,
				srcAddr, dstAddr, srcPort, dstPort);
			return DISCARD_PKT;
		}
		return ALLOW_PKT;
	}

	//通过源地址查找类
	group = treeLookup(active.addrTree, srcAddr, active.num_groups);
	if(group>0&&group<active.num_groups)
	{		
	portList = &active.t_dst_out[(group-1) * LIST_SIZE];
	}
	else 
       return ALLOW_PKT;//未设置端口列表，默认所有的端口允许
	//查找目的端口是否是允许出去访问的端口
	for (i=1; i <= portList[0].begin; i++) 
	{
		if ((dstPort >= portList[i].begin) &&
		    (dstPort <= portList[i].end))
			return ALLOW_PKT;
	}
	if (dstPort > 1024)
	{
		portList = &active.t_src_out[group * LIST_SIZE];
		for (i=1; i <= portList[0].begin; i++)
		{
			if ((srcPort >= portList[i].begin) &&
			    (srcPort <= portList[i].end))
				return ALLOW_PKT;
		}
	}
		syslogMessage(SYSL_OUT_PORT,IP_PROTO_TCP,
				srcAddr, dstAddr, srcPort, dstPort);
		return DISCARD_PKT;
}
	
/* ------------------------------------------------------------------------ */
static ULONG
checkIncomingUdp(ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort)
{
//	PortListEntry *portList;
	if (IN_CLASSD(dstAddr)) 
	{
		if (filtercfg.discardMulticast) 
		{
			filterstats.f_outsideMcast++;
			syslogMessage(SYSL_IN_CLASSD,IP_PROTO_UDP,
				srcAddr, dstAddr, srcPort,dstPort);
			return DISCARD_PKT;
		}
		return ALLOW_PKT;
	}
	syslogMessage(SYSL_IN_PORT, IP_PROTO_UDP,
		      srcAddr, dstAddr, srcPort, dstPort);

	return DISCARD_PKT;
}

/* ------------------------------------------------------------------------ */

static ULONG
checkOutgoingUdp(ULONG srcAddr, ULONG dstAddr,
		 unsigned short srcPort, unsigned short dstPort)
{
	register ULONG i;
	ULONG group;
	PortListEntry *portList;
	if (IN_CLASSD(dstAddr)) {
		if (filtercfg.discardMulticast) {
			filterstats.f_insideMcast++;
			syslogMessage(SYSL_OUT_CLASSD,IP_PROTO_UDP,
				srcAddr, dstAddr,srcPort, dstPort);
			return DISCARD_PKT;
		}
		return ALLOW_PKT;
	}
	group = treeLookup(active.addrTree, srcAddr, active.num_groups);
    if(group>0&&group<active.num_groups)
	{
	portList = &active.u_dst_out[(group-1) * LIST_SIZE];
	}
	else
	   return	ALLOW_PKT;
	for (i=1; i <= portList[0].begin; i++) {
		if ((dstPort >= portList[i].begin) &&
		    (dstPort <= portList[i].end))
			return ALLOW_PKT;
	}
	if (dstPort >= 1024) {
		portList = &active.u_src_out[(group-1) * LIST_SIZE];
		for (i=1; i <= portList[0].begin; i++) {
			if ((srcPort >= portList[i].begin) &&
			    (srcPort <= portList[i].end))
				return ALLOW_PKT;
		}
	}
	filterstats.f_insideUdpPort++;
	syslogMessage(SYSL_OUT_PORT, IP_PROTO_UDP,
		      srcAddr, dstAddr, srcPort, dstPort);

	return DISCARD_PKT;
}

/* ------------------------------------------------------------------------ */
static ULONG
checkIncomingIcmp(ULONG srcAddr, ULONG dstAddr, unsigned char type)
{
	register ULONG i;
	PortListEntry *portList;

	portList = &active.i_typ_in[ (treeLookup(active.addrTree,
				dstAddr, active.num_groups)-1) * LIST_SIZE ];
	for (i=1; i <= portList[0].begin; i++) {
		if ((type >= portList[i].begin) &&
		    (type <= portList[i].end))
		{
			syslogMessage(SYSL_IN_TYPE,IP_PROTO_ICMP,srcAddr, dstAddr,NULL,NULL,type);
			filterstats.f_outsideIcmpType++;
			return DISCARD_PKT;
		}

	}

	return ALLOW_PKT;
}

/* ------------------------------------------------------------------------ */
static ULONG
checkOutgoingIcmp(ULONG srcAddr, ULONG dstAddr, unsigned char type)
{
	register ULONG i;
	PortListEntry *portList;
	//先从地址树中得到该地址所对应的主机类号*LIST_SIZE后得到icmp类型列表对应的下标
	ULONG group;
	group=treeLookup(active.addrTree,srcAddr,active.num_groups);
	if(group>0&&group<active.num_groups)
	{
		portList=&active.i_typ_out[(group-1)*LIST_SIZE];
	}
	else//添加未配置该地址日志
        return ALLOW_PKT;//未设置类型限制，默认允许

	//判断比较
	for (i=1; i <= portList[0].begin; i++) {
		if ((type >= portList[i].begin) &&
		    (type <= portList[i].end))
		{
     	  syslogMessage(SYSL_OUT_TYPE,IP_PROTO_ICMP,srcAddr, dstAddr,NULL,NULL, type);
	      filterstats.f_insideIcmpType++;
	      return DISCARD_PKT;
		}
	}

			return ALLOW_PKT;
}

/* ------------------------------------------------------------------------ */

static ULONG
checkRejectTable(struct ip *ipHeader, ULONG srcAddr, ULONG dstAddr)
{
  //拒绝表为空则通过所有的包
	if (active.rejectTree == NULL)
		return ALLOW_PKT;

	/* If srcAddr is in the tree and it's group is 1, then discard pkt */
	//拒绝ip表的类号为1
	if (treeOutLookup(active.rejectTree, srcAddr, 2) == 1) {
		filterstats.f_outsideRejectTable++;
		syslogMessage(SYSL_IN_REJECT, (ULONG)ipHeader->Prot,
			srcAddr, dstAddr);
			
		return DISCARD_PKT;
	}

	/* Process by default */
	return ALLOW_PKT;
}

/* ------------------------------------------------------------------------ */
/*
 * Parameters 'protocol' and 'protolen' are expected to be in host byte order
 * The packet pointed to by 'pkt' is, of course, in network btye order
 */

static ULONG
checkIncomingPacket(unsigned short protocol/*报文协议类型*/, unsigned char *pkt/*报文头*/,
		    unsigned long protolen, ULONG checkAccept/*检查允许表或拒绝表*/)
{
	struct ip *ipHeader;
	struct tcphdr *tcpHeader;
	struct udphdr *udpHeader;
	struct icmp *icmpHeader;
	ULONG srcAddr, dstAddr;
	unsigned short ip_len,dstport,srcport;

	switch (protocol) 
	{

	case ETHER_PROTO_IP:
		ipHeader = (struct ip *)pkt;
		//12.4:广播包允许
		if(ipHeader->ip_dst.s_b[3]==255)
			return ALLOW_PKT;
		//12.4
	    srcAddr = ntohl(ipHeader->ip_src.ss_addr);
		dstAddr = ntohl(ipHeader->ip_dst.ss_addr);
		switch (ipHeader->Prot/*>ip_p*/)
		{
		 case IP_PROTO_TCP:
			 {
		     tcpHeader = (struct tcphdr *)(pkt + 
						(ipHeader->ip_hl<< 2));//ipHeader->ip_hl=5；左移后为20(bytes);
			 dstport=ntohs(tcpHeader->th_dport);
			 srcport=ntohs(tcpHeader->th_sport);
			 }
			 break;
		 case IP_PROTO_UDP:
			 {
			 udpHeader = (struct udphdr *)(pkt + 
						(ipHeader->ip_hl << 2));
			 dstport=ntohs(udpHeader->uh_dport);
			 srcport=ntohs(udpHeader->uh_sport);
			 }
			 break;
		}
		/*
		 * Pass all IP fragments that do not have offset 0 (beginning
		 * of the packet) without checking since the TCP/UDP
		 * headers are not in this packet.
		 这种情况下；TCP/UDP头不在此包中；如无攻击行为则通过!
		 */
		   if ((ipHeader->Flgoff/*>ip_off*/ & SW_IP_OFFMASK) != 0)
		   {
			/*
			 * If option is set, then discard pkts with offset of 1
			 */
			  if (filtercfg.discardSuspectOffset &&
			    (ntohs(ipHeader->Flgoff/*ip_off*/ & SW_IP_OFFMASK) == 1) &&
			    (ipHeader->Prot/*>ip_p*/ == IP_PROTO_TCP)) 
			  {

				filterstats.f_outsideSuspectOffset++;
				syslogMessage(SYSL_IN_OFFSET, IP_PROTO_TCP,
					srcAddr, dstAddr);
					
				return DISCARD_PKT;
			  } 

			/*
			 * If option is set, then discard fragmented ICMP pkts
			 */
			//丢弃分段的icmp报文
			   if (filtercfg.discardFragmentedICMP &&
			    (ipHeader->Prot/*>ip_p*/ == IP_PROTO_ICMP)) 
			   { 

				filterstats.f_outsideIcmpFrag++;
			    	syslogMessage(SYSL_IN_FRAG, IP_PROTO_ICMP,
					srcAddr, dstAddr);
					
				return DISCARD_PKT;
			   } 

			return ALLOW_PKT;
		   } 
         if(dstport==21||dstport==23||dstport==80||dstport==53)
		 {//外网到DM区
            if (checkIncomingDM(srcAddr,dstAddr,srcport,dstport,FROMOUT) == DISCARD_PKT)
			{
				filterstats.b_dmFiltered++;
//				filterstats.b_dmFiltered+=pktlen;
			    return IPF_DROP_PKT;
			}
			else 
			    return ALLOW_PKT;
		 }
		 else//进入内网检查
		 {
		  if (checkAccept)
		  { //检查允许表
			if (checkAcceptTable(ipHeader, srcAddr, dstAddr)
			    == DISCARD_PKT)
				return DISCARD_PKT;
		  }  
		  else 
		  {//检查拒绝表（进站包）
			if (checkRejectTable(ipHeader, srcAddr, dstAddr)
			    == DISCARD_PKT)
				return DISCARD_PKT;
		  }
        //检查ip的上层协议
		 switch (ipHeader->Prot/*>ip_p*/)
		 { 

		 case IP_PROTO_TCP:
		/*	tcpHeader = (struct tcphdr *)(pkt + 
						(ipHeader->ip_hl<< 2));//ipHeader->ip_hl=5；左移后为20(bytes);
			/*
			 * Make sure this packet (fragment) includes enough of
			 * the TCP header before making the other checks.
			 * Otherwise a SYN can sneak through since we might be
			 * trying to test something which is actually in the
			 * next fragment.
			 *
			 * NOTE: we drop all of these since we do not keep any
			 *       state between fragments.
			 *
			 * Many thanks to Uwe Ellermann at DFN-CERT for
			 * reporting this problem.
			 */
			//保证最小的tcp头长:至少包含SYN
			ip_len = ntohs(ipHeader->TtlLen/*>ip_len*/);
			if ((ip_len >65535/* protolen*/) ||
			    ((ip_len - (ipHeader->ip_hl << 2)) < 14)) 
			{
//				filterstats.f_outsideShortTcpHdr++;
				syslogMessage(SYSL_IN_LENGTH, IP_PROTO_TCP,
					srcAddr, dstAddr,
					ntohs(tcpHeader->th_sport),
				ntohs(tcpHeader->th_dport));
	
				return DISCARD_PKT;
			}
            //对于进入内网的包；因目的地址为防火墙；故不检查端口设置	
			/* Check for "ACKless SYN" */
		/*	if ((tcpHeader->th_flags & (TH_SYN|TH_ACK)) == TH_SYN)
			{
				return checkIncomingTcp(srcAddr, dstAddr,
						ntohs(tcpHeader->th_sport),
						ntohs(tcpHeader->th_dport));
			}
			*/
			break;
//检查udp
		case IP_PROTO_UDP:
		/*	udpHeader = (struct udphdr *)(pkt + 
						(ipHeader->ip_hl << 2));
			/*
			 * Make sure this packet (fragment) includes enough
			 * of the UDP header before making the other checks.
			 */
			//包含足够的UDP头:至少包含srcPort,dstPort;
			ip_len = ntohs(ipHeader->TtlLen/*>ip_len*/);
			if ((ip_len >65535 /*protolen*/) ||
			    ((ip_len - (ipHeader->ip_hl<< 2)) < 4)) {
//				filterstats.f_outsideShortUdpHdr++;
				syslogMessage(SYSL_IN_LENGTH, IP_PROTO_UDP,
					srcAddr, dstAddr,
					ntohs(udpHeader->uh_sport),
					ntohs(udpHeader->uh_dport));

				return DISCARD_PKT;
			}

			/* Check UDP protocols. */
            //对于进入内网的包；因目的地址为防火墙；故不进行更多的检查
			/*return checkIncomingUdp(srcAddr, dstAddr,
						ntohs(udpHeader->uh_sport),
						ntohs(udpHeader->uh_dport));
						*/
			break;
//ICMP
		case IP_PROTO_ICMP:
			if(filtercfg.discardIcmp)
			{
			    syslogMessage(SYSL_IN_ICMP, IP_PROTO_ICMP,
					srcAddr, dstAddr);
				return DISCARD_PKT;
			}
			icmpHeader = (struct icmp *)
				     (pkt + (ipHeader->ip_hl << 2));

			/*
			 * If option is set, then discard fragmented ICMP pkts
			 */
			if (filtercfg.discardFragmentedICMP &&
			    (ipHeader->Flgoff/*>ip_off */& IP_MF) != 0) 
			{
				filterstats.f_outsideIcmpFrag++;
			    	syslogMessage(SYSL_IN_FRAG, IP_PROTO_ICMP,
					srcAddr, dstAddr);
					

				return DISCARD_PKT;
			}

			/*
			 * If option is set, discard attack ICMP pkts.
			 *
			 * If the ICMP type, code, identifier, and seq num all
			 * equal 0, this may be an ICMP echo reply attack pkt.
			 */
			if (filtercfg.discardAttackICMP &&
			    (*((unsigned short *)icmpHeader) == 0) && /*type & code*/
			    (icmpHeader->icmp_void == 0))
			{      /*id & seq*/

				if (icmpHeader->icmp_cksum == 0xFFFF) 
				{
				
					/* This is a "smurf" attack */
					filterstats.f_outsideSmurfDos++;
			    		syslogMessage(SYSL_IN_DOS,NULL,
						      srcAddr, dstAddr,
						      DOS_SMURF);
							  
					return DISCARD_PKT;

				}
				else
					if (*((unsigned short *)icmpHeader->icmp_data)//?????????
					   == htons(0x4500)) 
					{
					/* This is a "pong" attack *///???????????????////???????????
					filterstats.f_outsidePongDos++;
			    		syslogMessage(SYSL_IN_DOS,NULL,
						      srcAddr, dstAddr,NULL,NULL,NULL,
						      DOS_PONG);
							  
					return DISCARD_PKT;
					}
			} 
            /*不进行更多的检查
			return checkIncomingIcmp(srcAddr, dstAddr,
						 icmpHeader->icmp_type);
						 */
			break;
		case IP_PROTO_IGMP:
			if((filtercfg.discardMulticast)||((ipHeader->Flgoff& IP_MF) != 0)) 
			{
			    syslogMessage(SYSL_IN_IGMP, IP_PROTO_IGMP,
					srcAddr, dstAddr);
			  return DISCARD_PKT;
			}
			else 
              return ALLOW_PKT; 
		default:
			if (filtercfg.discardOtherIp)
			{

				filterstats.f_outsideOtherIp++;
				syslogMessage(SYSL_IN_PROT, (ULONG)ipHeader->Prot,
					srcAddr, dstAddr);

				return DISCARD_PKT;
			}

			/* Everything is allowed so far. */
			break;
		 }
//first switch语句
		break;
/*不考虑ARP与RARP报文
	case ETHER_PROTO_ARP:
	//case ETHER_PROTO_REVARP:
		/* pass these guys through no matter what */
//		break;


	default:
		/* Only pass the rest if told to do so. */
		if (filtercfg.discardNonIp)
		{

			/* Boy will this get ugly if the syslog mask is not */
			/* configured properly..... */

//			filterstats.f_outsideNonIp++;
			return DISCARD_PKT;
		}

	  } 

	return ALLOW_PKT;
	}
	return ALLOW_PKT;
}

/* ------------------------------------------------------------------------ */

static ULONG
checkAcceptTable(struct ip *ipHeader, ULONG srcAddr, ULONG dstAddr)
{
	/* If the accept table is empty, then forward all packets */
	if (active.acceptTree == NULL)
		return ALLOW_PKT;

	/* If srcAddr is in the tree and it's group is 1, then process pkt */
	//允许ip表的类号也应设置为1
	if (treeLookup(active.acceptTree, srcAddr, 2) == 1)
	{
	/* Discard by default */
	filterstats.f_insideAcceptTable++;
	syslogMessage(SYSL_OUT_ACCEPT, (ULONG)ipHeader->Prot,
			srcAddr, dstAddr);
	  return DISCARD_PKT;
	}
	return ALLOW_PKT;
}

/* ------------------------------------------------------------------------ */
/*
 * Parameters 'protocol' and 'protolen' are expected to be in host byte order
 * The packet pointed to by 'pkt' is, of course, in network btye order
 */
//出去的只检查允许表
static ULONG
checkOutgoingPacket(unsigned short protocol, unsigned char *pkt, unsigned short protolen)
{

	struct ip *ipHeader;
	struct tcphdr *tcpHeader;
	struct udphdr *udpHeader;
	struct icmp *icmpHeader;
	unsigned short ip_len,dstport,srcport;
	ULONG srcAddr, dstAddr;

	switch (protocol)
	{
	case ETHER_PROTO_IP:
		ipHeader = (struct ip *)pkt;
		//12.4:广播包允许
		if(ipHeader->ip_dst.s_b[3]==255)
			return ALLOW_PKT;
		//12.4
		srcAddr = ntohl(ipHeader->ip_src.ss_addr);
		dstAddr = ntohl(ipHeader->ip_dst.ss_addr);
		//首先进行攻击检查
		/*
		 * Pass all IP fragments that do not have offset 0 (beginning
		 * of the packet) without checking since the TCP/UDP
		 * headers are not in this packet.  An area for improvement
		 * would be to cache session info so we could drop all
		 * disallowed fragments also instead of just the first one.
		 这种情况下(偏移量不在（0~224）)；TCP/UDP头不在此包中；如无攻击行为则通过!
		 */
		if ((ipHeader->Flgoff/*>ip_off*/ & SW_IP_OFFMASK) != 0)
		{//非first数据包SW_IP_OFFMASK:ntohs(0xFF1F)=0x1FFF:因为前三位为标志
			if (filtercfg.discardSuspectOffset &&
			    (ntohs(ipHeader->Flgoff/**/ & SW_IP_OFFMASK) == 1) &&//ipHeader->Flgoff:0X0100~0X01E0(256~480)
			    (ipHeader->Prot/*>ip_p*/ == IP_PROTO_TCP)) 
			{

				filterstats.f_insideSuspectOffset++;
				syslogMessage(SYSL_OUT_OFFSET, IP_PROTO_TCP,
					srcAddr, dstAddr);
				return DISCARD_PKT;
			}

			/*
			 * If option is set, then discard fragmented ICMP pkts
			 */
			if (filtercfg.discardFragmentedICMP &&
			    (ipHeader->Prot/*>ip_p*/ == IP_PROTO_ICMP)) 
			{

				filterstats.f_insideIcmpFrag++;
			    	syslogMessage(SYSL_OUT_FRAG, IP_PROTO_ICMP,
					srcAddr, dstAddr);
					
				return DISCARD_PKT;
			}

			return ALLOW_PKT;
		}
        //是第一个ip分片包，可能包含上层协议报头，因此检查上层协议
		switch (ipHeader->Prot/*>ip_p*/)
		 {
		  case IP_PROTO_TCP:
			  {
		     tcpHeader = (struct tcphdr *)(pkt + 
						(ipHeader->ip_hl<< 2));//ipHeader->ip_hl=5；左移后为20(bytes);
			 dstport=ntohs(tcpHeader->th_dport);
			 srcport=ntohs(tcpHeader->th_sport);
			  }
			 break;
		  case IP_PROTO_UDP:
			  {
			 udpHeader = (struct udphdr *)(pkt + 
						(ipHeader->ip_hl << 2));
			 dstport=ntohs(udpHeader->uh_dport);
			 srcport=ntohs(udpHeader->uh_sport);
			  }
			 break;
		 }
        if(dstAddr==filtercfg.dm_ip)//进入dm检查
		{
		   if(checkIncomingDM(srcAddr,dstAddr,srcport,dstport,FROMIN) == DISCARD_PKT)
			{
				filterstats.b_dmFiltered++;
		//		filterstats.b_dmFiltered += pktlen;
			    return IPF_DROP_PKT;
			}
			else 
		    	return ALLOW_PKT;
		}
		else//进入外网检查
		{
		if (checkAcceptTable(ipHeader,srcAddr,dstAddr) == DISCARD_PKT)
			return DISCARD_PKT;
		switch (ipHeader->Prot/*>ip_p*/) 
		{
		case IP_PROTO_TCP:
			tcpHeader = (struct tcphdr *)(pkt + 
						(ipHeader->ip_hl<< 2)); 
			/*
			 * Make sure this packet (fragment) includes enough
			 * of the TCP header before making the other checks.
			 */
			ip_len = ntohs(ipHeader->TtlLen/*>ip_len*/);
			if ((ip_len >65535/* protolen*/) ||
			    ((ip_len - (ipHeader->ip_hl<< 2)) < 14))
			{
//				filterstats.f_insideShortTcpHdr++;
				syslogMessage(SYSL_OUT_LENGTH, IP_PROTO_TCP,
					srcAddr, dstAddr,
					ntohs(tcpHeader->th_sport),
					ntohs(tcpHeader->th_dport));

				return DISCARD_PKT;
			}

			/* Check for "ACKless SYN" */
			if ((tcpHeader->th_flags & (TCP_SYN|TCP_ACK)) == TCP_SYN)
			{
				return checkOutgoingTcp(srcAddr, dstAddr,
						ntohs(tcpHeader->th_sport),
						ntohs(tcpHeader->th_dport));
			}
			break;

		case IP_PROTO_UDP:
			udpHeader = (struct udphdr *)(pkt + 
						(ipHeader->ip_hl << 2));
			/*
			 * Make sure this packet (fragment) includes enough
			 * of the UDP header before making the other checks.
			 */
			ip_len = ntohs(ipHeader->TtlLen);
			if ((ip_len >65535 /*protolen*/) ||
			    ((ip_len - (ipHeader->ip_hl << 2)) < 4)) 
			{
//				filterstats.f_insideShortUdpHdr++;
				syslogMessage(SYSL_OUT_LENGTH, IP_PROTO_UDP,
					srcAddr, dstAddr,
					ntohs(udpHeader->uh_sport),
					ntohs(udpHeader->uh_dport));

				return DISCARD_PKT;
			}

			return  checkOutgoingUdp( srcAddr, dstAddr,
					ntohs(udpHeader->uh_sport),
					ntohs(udpHeader->uh_dport));
        case IP_PROTO_IGMP:
			if(filtercfg.discardMulticast)
			{
			    syslogMessage(SYSL_OUT_IGMP, IP_PROTO_IGMP,
					srcAddr, dstAddr);
				return DISCARD_PKT;
			}
			else 
				return ALLOW_PKT;
		case IP_PROTO_ICMP:
			if(filtercfg.discardIcmp)
			{
			    syslogMessage(SYSL_OUT_ICMP, IP_PROTO_ICMP,
					srcAddr, dstAddr);
				return DISCARD_PKT;
			}
			icmpHeader = (struct icmp *)
				     (pkt + (ipHeader->ip_hl<< 2));

			/*
			 * If option is set, then discard fragmented ICMP pkts
			 */
			if (filtercfg.discardFragmentedICMP &&
			    (ipHeader->Flgoff/*>ip_off*/ & IP_MF) != 0)
			{//IP_MF:0x4000;标志域的第二位为1，说明有后续的分片
				filterstats.f_insideIcmpFrag++;
			    	syslogMessage(SYSL_OUT_FRAG, IP_PROTO_ICMP,
					srcAddr, dstAddr);
					

				return DISCARD_PKT;
			}

			/*
			 * If option is set, discard attack ICMP pkts.
			 *
			 * If the ICMP type, code, identifier, and seq num all
			 * equal 0, this may be an ICMP echo reply attack pkt.
			 */
			if (filtercfg.discardAttackICMP &&
			    (*((unsigned short *)icmpHeader) == 0) && /*type & code*/
			    (icmpHeader->icmp_void == 0))
			{      /*id & seq*/

				if (icmpHeader->icmp_cksum == 0xFFFF)
				{
				
					/* This is a "smurf" attack */
					filterstats.f_insideSmurfDos++;
			    		syslogMessage(SYSL_OUT_DOS,NULL,
						      srcAddr, dstAddr,NULL,NULL,NULL,
						      DOS_SMURF);
							  
					return DISCARD_PKT;

				} 
				else
					if (*((unsigned short *)icmpHeader->icmp_data)
					   == htons(0x4500))//69
					{
					/* This is a "pong" attack */
					filterstats.f_insidePongDos++;
			    		syslogMessage(SYSL_OUT_DOS,NULL,
						      srcAddr, dstAddr,NULL,NULL,NULL,
						      DOS_PONG);
							  
					return DISCARD_PKT;
					}
			}

			return checkOutgoingIcmp(srcAddr, dstAddr,
						 icmpHeader->icmp_type);

		default:
			if (filtercfg.discardOtherIp)
			{

				filterstats.f_insideOtherIp++;
				syslogMessage(SYSL_OUT_PROT,(ULONG)ipHeader->Prot,
					srcAddr, dstAddr);

				return DISCARD_PKT;
			}

			/* Everything is allowed so far. */
			break;
		}
      }
		break;

//	case ETHER_PROTO_ARP:
//	case ETHER_PROTO_REVARP:
		/* pass these guys through no matter what */
//		break;

	default:
		/* Only pass the rest if told to do so. */
		if (filtercfg.discardNonIp) {
//			filterstats.f_insideNonIp++;
	//		syslogMessage(SYSL_OUT_FILTER, (ULONG)protocol);
		return DISCARD_PKT;
		}
	}

	return ALLOW_PKT;
}

/* ------------------------------------------------------------------------ */
/* char      dev_instance;   //网络设备编号
   char      direction;       //数据流向
                               // 1 入站
 							   // 2 出站  
   struct ether_header  h;    //以太帧头的指针
   struct mbuf    m;          //数据缓冲区的指针
*/
ULONG
ipfilter_test_pkt(USHORT dev_instance, UCHAR direction,ULONG ethlen,/* struct ether_header *h,*/BYTE *m)
{
	unsigned short protocol,protolen;
	unsigned char *pkt;
	unsigned long  pktlen;
	struct ip *ipHeader;
	ULONG srcAddr, dstAddr;
//	unsigned short ip_len;
	 //如果包过滤器没有运行则通过所有包
	if (!filterstats.running)
		return IPF_PROCESS_PKT;
    struct Ether_Header *h=(struct Ether_Header *)m;

	//得到以太帧的协议号，以太帧的数据部分，数据部分长和以太帧总长
	protocol = ntohs(h->ether_type);
	pkt = m+sizeof(struct Ether_Header);
	protolen =ethlen-sizeof(struct Ether_Header)/* m->m_pkthdr.len*/;
	pktlen =ethlen;

	//只对IP包进行过滤检查，非IP包允许通过
	if (protocol != ETHER_PROTO_IP) 
		return IPF_PROCESS_PKT;
	else 
	{//如果是IP包，则得到IP包的目的地址和源地址      
		ipHeader = (struct ip *)pkt;
		if(ipHeader->ip_hl>5)
		{
		    syslogMessage((SYSL_OUT_ROUT,ntohs(ipHeader->Prot),srcAddr,dstAddr));		             
			return IPF_DROP_PKT;
		}

//		static iii;
//		iii++;
//		TRACE("AAA%d\n",iii);
		srcAddr = ntohl(ipHeader->ip_src.ss_addr);
		dstAddr = ntohl(ipHeader->ip_dst.ss_addr);
	}
	if (dev_instance == filtercfg.in_number)//判断设备号
	{   /* 来自内部接口的包 */
	    ULONG rc;
		filterstats.p_insideRx++;
		filterstats.b_insideRx += pktlen;		
		//丢弃所有IP地址假冒的包
		if (srcAddr&filtercfg.in_mask == dstAddr&filtercfg.in_mask) //在同一网段
		{
		//在此加上 if (ipHeader->ip_hl != 5) {
		//          if (filtercfg.discardRouteIP){ 
		//             DBstat.f_insideRouteIP++;
//		    syslogMessage((SYSL_OUT_ROUT,ntohs(ipHeader->Prot),srcAddr,dstAddr));		             
			return IPF_DROP_PKT;
		}
			

		//进入外网与DM区检查
		//检查允许表
		rc = checkOutgoingPacket(protocol,pkt,protolen);
		if (rc != ALLOW_PKT)
		{
			filterstats.p_insideFiltered++;
			filterstats.b_insideFiltered += pktlen;
/*			if (rc == DISCARD_AND_ANSWER_PKT)
				return IPF_DROP_AND_ANSWER_PKT;
				*/
			return IPF_DROP_PKT;
		}
		filterstats.p_insideTx++;
		filterstats.b_insideTx+= pktlen;
		return IPF_FORWARD_PKT;
		//end
	}
	 else
	 {
		if (dev_instance == filtercfg.out_number) 
		{  /* from the outside interface1 */ //来自外部接口的包
		 ULONG rc;
		 filterstats.p_outsideRx++;
		 filterstats.b_outsideRx += pktlen;
        
		 if (srcAddr&&filtercfg.out_mask == dstAddr&&filtercfg.out_mask) 
			return IPF_DROP_PKT;
	     
		 //外网进入内网或DM区
         rc = checkIncomingPacket(protocol,pkt,protolen,0);//0:检查拒绝表
   	     if (rc != ALLOW_PKT)
		 {
			filterstats.p_outsideFiltered++;
			filterstats.b_outsideFiltered += pktlen;
			if (rc == DISCARD_AND_ANSWER_PKT)
				return IPF_DROP_AND_ANSWER_PKT;
			return IPF_DROP_PKT;
		 }

	    filterstats.p_outsideTx++;
		filterstats.b_outsideTx+= pktlen;
		return IPF_FORWARD_PKT;
		}
	 }
	/*
	 * The pkt is not from either filter interface so process it normally
	 */
	 //从dm区来的包假定为始终合法
	return DB_PROCESS_PKT;
}
VOID   ipfilter_pkt(ULONG ethlen,BYTE *m)
{
//			TRACE("initialized=%d\n",initialized);
            if(initialized)
			{
				  USHORT dev_instance=m[1];//test
		          UCHAR  direction;//test
		          direction=m[0];
		//		  TRACE("direction=%d\n",direction);
				  if(direction==0)//从网卡进来
				  {
					static int ii;
                    ULONG ret;
		            ret=ipfilter_test_pkt(dev_instance,direction,1600,m+2);
		            switch(ret)
					{      
		               case IPF_DROP_PKT:
	                   case IPF_DROP_AND_ANSWER_PKT:
                           TRACE("memset%d\n",ii);
						   ii++;
						   memset(&m[2],0,1598);
						   break;
			        
		               case  DB_PROCESS_PKT:
		               case  IPF_PROCESS_PKT:
					   case  IPF_FORWARD_PKT:
						   break;
		               default:
						   TRACE("default\n");
			                break;
					}
				  } 
			}

}
/*------------------------------------------------------------------------ */
/*
 * Sends tcp reset packet.
 */
//将rout发送出去。。。。。。。。。。。。。。。。。。。。。。。。。
void
ipfilter_response_pkt(BYTE *rout,ULONG outlen,ULONG inlen/* void *inh, */,BYTE *min)
{
	memcpy(rout,min,inlen);
	ULONG tmp;
	register struct pkt_struct *out_pkt;
	register struct pkt_struct *in_pkt;

	/* Outbound packet's media header */
	if (filtercfg.media_type == IFT_ETHER)
	{
		register struct Ether_Header *in_h =(struct Ether_Header *)/*gj*/ min;
//		register struct Ether_Header *out_h =mtod(r, struct Ether_Header *);
//	    register struct Ether_Header *out_h =mtod(r);////gj	
  //      register struct Ether_Header *out_h =mtod(r);////gj	
        register struct Ether_Header *out_h =(struct Ether_Header *)rout;////gj
		//调还原与目的
		memcpy(&out_h->ether_dhost, &in_h->ether_shost, 6);
		memcpy(&out_h->ether_shost, &in_h->ether_dhost, 6);
		out_h->ether_type = in_h->ether_type;

//		rout->m_data += sizeof(struct ether_header);

	} 
	
/*	else
	{ /* IFT_FDDI */
/*		register struct fddi_header *in_h = (struct fddi_header *)h;
		register struct fddi_header *out_h =mtodFddi(r);//gj

		memcpy(out_h->fddi_dhost, in_h->fddi_shost, 6);
		memcpy(out_h->fddi_shost, in_h->fddi_dhost, 6);
		out_h->fddi_fc = in_h->fddi_fc;

		r->m_data += sizeof(struct fddi_header);
		memcpy(r->m_data, m->m_data, LLC_SNAPLEN);
		r->m_data += LLC_SNAPLEN;
		m->m_data += LLC_SNAPLEN;
	}
*/
	/* Setup pointers and clear outbound packet */
//	in_pkt = mtod(m, struct pkt_struct *);
//	out_pkt = mtod(r, struct pkt_struct *);
	in_pkt = mtodPkt(min);//将以太帧内容转换为struct pkt_struct
	out_pkt = mtodPkt(rout);
//	rout->m_len = sizeof(struct pkt_struct);
	memset(out_pkt,0, sizeof(struct pkt_struct));

	/* IP header fields included in the TCP checksum */
	out_pkt->i.TtlLen = htons(20);		/* TCP len */
	out_pkt->i.Prot = IP_PROTO_TCP;
	out_pkt->i.ip_src = in_pkt->i.ip_dst;
	out_pkt->i.ip_dst = in_pkt->i.ip_src;

	/* TCP header */
	out_pkt->t.th_sport = in_pkt->t.th_dport;
	out_pkt->t.th_dport = in_pkt->t.th_sport;
	out_pkt->t.th_seq = in_pkt->t.th_ack;
	tmp = ntohl(in_pkt->t.th_seq) + 1;
	out_pkt->t.th_ack = htonl(tmp);
	out_pkt->t.th_off = 5;
	out_pkt->t.th_flags = 0x14;//ack,rst

	/* TCP checksum */
	out_pkt->t.th_sum = in_cksum_tcp(rout);//gj:添加函数体

	/* Finish the IP header */
	out_pkt->i.ip_v = 4;
	out_pkt->i.ip_hl = 5;			/* IP hdr len >> 2 */
	out_pkt->i.TtlLen = htons(40);		/* Total length */
	out_pkt->i.TTL= 128;

	/* IP header checksum */
	out_pkt->i.ChkSum = in_cksum_hdr((BYTE*)&out_pkt);
//并未改变以太帧头的内容
//将rout发送出去。。。。。。。。。。。。。。。。。。。。。。。。。

	/* Adjust outbound mbuf to include media header */
/*	if (filtercfg.media_type == IFT_ETHER) {
		rout->m_data -= sizeof(struct ether_header);
		rout->m_len += sizeof(struct ether_header);
	}
	*/
	/*else {
		r->m_data -= (sizeof(struct fddi_header) + LLC_SNAPLEN);
		r->m_len += (sizeof(struct fddi_header) + LLC_SNAPLEN);
	}
	*/
//	rout->m_pkthdr.len = rout->m_len;
	//。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
}

/* ------------------------------------------------------------------------ */

static ULONG
initRequest(initReq *req)
{
	//清除所有的状态信息（除了日志信息）
    memset(&filterstats, 0, sizeof(filterstats));
	hlogFile=fopen("日志.txt","a");
	statFile=fopen("统计.txt","a");

	//建立缺省配置
	initTables(&active, 10);//1:初始化为10个类表
	//:当需要运行时动态增加类表数（超过了initTables(&active, 10)中的设置）时
	//：现在用处不大
	//每个类最多可有LIST_SIZE==32个PortListEntry
    initTables(&newspace, 10);
	GetLocalTime(&filterstats.starttime);
	initialized = YES;

	return NOO_ERROR;
}

//地址是主机顺序
//addrTree是过滤用的。
//函数适用：外网拒绝表设置，主机类最大范围不可超过255；主机号部分全0、全1除外(1~254)
//因为外网范围很广
//相应设置适于外网的搜索函数
static ULONG
OutnetaddToTree(OutaddrTreeNode *tbl, in_addr_t addr,/*主机类起始地址,*/ 
			unsigned short hostnums,/*从主机类起始地址开始的主机数,*/
			unsigned short group/*组号与端口列表的组号一至*/)
{
	int i, start, end, n, level;
	addrTreePtr *ptr=(addrTreePtr *)malloc(sizeof(addrTreePtr));//gj
        if (!tbl || ((256-addr.s_b[0])<hostnums))
           return -1;

	//地址范围
    start=addr.s_b[0];
	end=start+hostnums;
	for (i=start; i<end; i++) 
	{
		ptr->p=tbl;
		addr.s_b[0] = i;
		// 'byte' 说明树有多深 
		//'level'记录正处在何处: 0, 1, 2, or 3 
        for (n=0, level=0; n < 4; n++, level++)
		{
		//IP地址的下一个8位作为索引，索引这个结点的指针或类的数组
		 unsigned char b =addr.s_b[3 - level];
			// 如果n=3, 最后一个字节
			if (n == 3) 
			{
				if (level == 3) 
				{
					//Level 3只包括类
					ptr->pd->c[b] = group;				
				} 
				else
				{
					// Levels 0-2 可能是一个类或者是一个指针
					if (!ptr->p->flg[b] && ptr->p->u[b].p)//访问违例???????????
					freeOutTree(ptr->p->u[b].p,level+1);
                    //释放指针后；存储类号
					ptr->p->u[b].p = NULL;
                    ptr->p->u[b].c = group;
					ptr->p->flg[b] = 1;
				}
			//一个指针
			}
			else
			{
		     if ((!ptr->p->flg[b])&&(ptr->p->u[b].p))//访问违例??????????
				{//对应的标志为0；同时对应的下一节点指针存在
				   //将树向上移动一个深度
				ptr->p = ptr->p->u[b].p;
				}// 对应的标志为1或	对应的下一节点指针不存在
				else
				{
				 unsigned short j;
				 addrTreePtr next;
				 if (level == 2)
				 {
					unsigned int tmp_c = ptr->p->u[b].c;
					if (!(next.pd = MALLOC_LEAF))//最后一个必定是类
						return 1;
					for (j=0; j<256; j++)
					{
						next.pd->c[j] = tmp_c;
					}
				 }
				 else //level=0或1
				 {
					void *tmp_p = ptr->p->u[b].p;
					unsigned char tmp_f = ptr->p->flg[b];
					if (!(next.p = MALLOC_OUTNODE))
						return 1;
					//对next的每一成员付值;添加到树的前部
					for (j=0; j<256; j++) 
					{
						next.p->u[j].p = (OutaddrTreeNode*)tmp_p;
						next.p->flg[j] = tmp_f;
					}
					int gj=0;
				}//生成下一个节点，并将标志位设置为0指示为一个指针
				ptr->p->u[b].p = next.p;//添加到树的上一节点
				ptr->p->flg[b] = 0;
				ptr->p = next.p;//将临时指针指向树的上一节点
				}
			}//end else(2.1)
		}//end for(2)
	}//end for(1)
	free(ptr);
	return 0;
}

VOID clearipfilter()
{
	if(initialized==YES)
	{
	freeAllPointers(&active);
	freeAllPointers(&newspace);
/*	fwrite(filterlog.str,sizeof(char),filterlog.pos*100,hlogFile);
	*/
	writestatistic();
	fprintf(statFile,"\n");
	fclose(statFile);
	fclose(hlogFile);
	initialized=NO;
	memset(&filterstats,0,sizeof(Statistics)); 	
	memset(&filtercfg,0,sizeof(FilterConfig)); 
    filtercount=0;
	}

}
//地址是主机顺序
//addrTree是过滤用的。
//函数适用：主机类最大范围可超过255；//主机号部分全0、全1不除外(1~254)

static ULONG
GjaddToTree(addrTreeNode *tbl, in_addr_t addr/*主机类起始地址*/, 
			unsigned short hostnums/*从主机类起始地址开始的主机数*/,
			unsigned short group/*组号与端口列表的组号一至*/)
{
	unsigned short   start, end,i,n, level,total,remove,remain;
	UCHAR b;
	BOOL bfirst=TRUE;
	addrTreeLeaf *ptrleaf;
        if (!tbl)
           return -1;
	//地址范围
	start=addr.s_b[0];
	total=start+hostnums;
	if(total>255)
	{
		end=255;
		remove=255-start;
	}
	else
	{
		end=total;
        remove=hostnums;
	}
    do
	{
    if(!bfirst)
	{
		start=0;
		addr.s_b[1]++;
	}
	for (i=start; i<end; i++) 
	{
		addr.s_b[0] = i;
		//'level'记录正处在何处: 0, 1
        for (n=0, level=0; n < 2; n++, level++)
		{
		//IP地址的下一个8位作为索引，索引这个结点的指针或类的数组
		  b=addr.s_b[1- level];
		  if(n==1)
		  {
			 ptrleaf->c[b]=group;
		  }
		  else
		  {
		    if(!tbl->u[b].pd)
			{
			 tbl->u[b].pd=MALLOC_LEAF;
			}
		    ptrleaf=tbl->u[b].pd;
		  }
		}
	}
	bfirst=FALSE;
	remain=hostnums-remove;
	if(remain>255)
	{
		end=255;
	    remove+=256;
	}
	else
	{
		end=remain;	
		remove+=remain;
	}
	}while(remain);
	return 0;
}
static void
freeOutTree(OutaddrTreeNode *tbl, unsigned char level)
{
        int i;

        if (!tbl || (level > 3)) 
		{
		//参数错误
                return;
		}

        if (level == 3)
		{
		  free((addrTreeLeaf *)tbl);
		  return;
		}

        for (i=0; i<256; i++) 
		{
                if (!tbl->flg[i] && tbl->u[i].p)
				{
			        if (level == 2)
				       free(tbl->u[i].pd);
			        else
				        freeOutTree(tbl->u[i].p, level + 1);//递归的方法？？？？
				}
		}

	free(tbl);
}
/* ------------------------------------------------------------------------ */
//从命令发生器接受命令，并产生动作
ULONG
filter_command( unsigned long cmd, char* initinfo)
{
	ULONG error = 0;
	//未被初始化的IP过滤器，不能接受其他命令
	if (initialized == 0) {
		if ((cmd != DIOCINIT)&&(cmd != DIOCGLOGM) &&
	    	    (cmd != DIOCSLOGM)) {
			return ERROR_NOT_INITILIZED; /* 没有初始化 */
		}
	}

	switch (cmd) {

	case DIOCINIT:		/* 初始化 */		
		if (filterstats.running)
		{
			error = ERROR_RUNNING;
			break;
		}
		error = initRequest((initReq *)initinfo);//!!!!!!!!!!!!!!!!!!!!!!!!!!!
		break;

	case DIOCSTART:		// 启动 		
		if (filterstats.running) 
		{
			error = ERROR_RUNNING;   //已经运行
			break;
		}	
		filterstats.running = YES;
		break;

	case DIOCSTOP:		// 停止		
		if (!filterstats.running) 
		{		
			error = ERROR_NOT_RUNNING; // 没有运行 
			break;
		}
		filterstats.running = NO;
		break;
			
	//输出统计结果//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	case DIOCGSTATS:	
		*((Statistics *)initinfo) = filterstats;
		break;

    //清除统计信息
	case DIOCCSTATS:	
        memset(&filterstats, 0, sizeof(filterstats));
		break;

	//输入到类表中
	case DIOCSGROUP://!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		initTables(&newspace, u->val);//现在仅考虑一个主机类的情况
		error = portListRequest((portListReq *)initinfo, &newspace);
		break;

	//输入到网络表中
	case DIOCSNETWORK:	
		error = addrTreeRequest((addrTreeReq *)initinfo);
		break;
		  
	//修改网络表
	case DIOCMNETWORK:	
		error = manageTablesRequest((manageTablesReq *)initinfo);
		break;

	//输入到拒绝表中
	case DIOCSREJECT:
		error = OutrejaccTableRequest(T_REJTBL,
					   (rejaccTableReq *)initinfo,&newspace );
		break;
   //输入到允许表中
	case DIOCSACCEPT:
		error = rejaccTableRequest(T_ACCTBL,
					   (rejaccTableReq *)initinfo, &newspace);
		break;
    case DIOCS_INDM:
		error = rejaccTableRequest(T_INDM,
					   (rejaccTableReq *)initinfo, &newspace);
		break;
    case DIOCS_OUTDM:
		error = OutrejaccTableRequest(T_OUTDM,
					   (rejaccTableReq *)initinfo, &newspace);
		break;
	default:
		error = ERROR_INVALID_OP; //错误的操作
		break;

	}
	return error;
}
void writestatistic()
{
    Statistics StatisticInfo;
    GetStatisticsInfo(&StatisticInfo);
	fprintf(statFile,"起始时间：%d月%d日%d:%d:%d\t"
		,StatisticInfo.starttime.wMonth,StatisticInfo.starttime.wDay,
		StatisticInfo.starttime.wHour,
		StatisticInfo.starttime.wMinute,StatisticInfo.starttime.wSecond);
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);
	fprintf(statFile,"结束时间：%d月%d日%d:%d:%d\t\n"
		,systemtime.wMonth,systemtime.wDay,systemtime.wHour,
		 systemtime.wMinute,systemtime.wSecond);

	fprintf(statFile,"出站包数：%d"  "字节数：%d"  "通过的内网包数：%d"  "字节数：%d"  "过滤掉的内网包数：%d"  "字节数：%d,\n入站包数：%d"  "字节数：%d"  "通过的外网包数：%d"  "字节数：%d"  "过滤掉的外网包数：%d"  "字节数：%d,\n"
		,StatisticInfo.p_insideRx,StatisticInfo.b_insideRx
		,StatisticInfo.p_insideTx,StatisticInfo.b_insideTx
		,StatisticInfo.p_insideFiltered,StatisticInfo.b_insideFiltered
		,StatisticInfo.p_outsideRx,StatisticInfo.b_outsideRx
		,StatisticInfo.p_outsideTx,StatisticInfo.b_outsideTx
		,StatisticInfo.p_outsideFiltered,StatisticInfo.b_outsideFiltered
		);
	fprintf(statFile,"进DM包数：%d"  "字节数：%d"  "通过DM的包数：%d"  "字节数：%d"  "DM过滤掉的包数：%d"  "字节数：%d,\n"
	    ,StatisticInfo.p_dmRx,StatisticInfo.b_dmRx
		,StatisticInfo.p_dmFiltered,StatisticInfo.b_dmFiltered
		,StatisticInfo.p_dmTx,StatisticInfo.b_dmTx
		);
	fprintf(statFile,"内网:\n\t禁止IP的包数：%d"  "禁止ICMP分段的包数：%d"  "禁止ICMP类型的包数：%d"  "pong攻击包数：%d,\n"  "smurf攻击的包数：%d"  "可疑offset的包数：%d"  "禁止tcp端口的包数：%d"  "禁止udp端口的包数：%d\n禁止组播的包数：%d"  "禁止别的IP包数：%d\n"
		,StatisticInfo.f_insideAcceptTable,StatisticInfo.f_insideIcmpFrag
		,StatisticInfo.f_insideIcmpType,StatisticInfo.f_insidePongDos
		,StatisticInfo.f_insideSmurfDos,StatisticInfo.f_insideSuspectOffset
		,StatisticInfo.f_insideTcpPort,StatisticInfo.f_insideUdpPort
		,StatisticInfo.f_insideMcast,StatisticInfo.f_insideOtherIp);
	 fprintf(statFile,"外网:\n\t禁止IP的包数：%d"  "禁止ICMP分段的包数：%d"  "禁止ICMP类型的包数：%d"  "pong攻击包数：%d\nsmurf攻击的包数：%d"  "可疑offset的包数：%d"  "禁止组播的包数：%d\n"	"禁止别的IP的包数：%d\n"
		,StatisticInfo.f_outsideRejectTable,StatisticInfo.f_outsideIcmpFrag
		,StatisticInfo.f_outsideIcmpType,StatisticInfo.f_outsidePongDos
		,StatisticInfo.f_outsideSmurfDos,StatisticInfo.f_outsideSuspectOffset
		,StatisticInfo.f_outsideMcast,StatisticInfo.f_outsideOtherIp);
}
VOID GetStatisticsInfo(Statistics* StatisticInfo)
{
	filter_command(DIOCGSTATS,(char*)StatisticInfo);
}
VOID mysearch()
{
/*	ULONG group0,group1,group2,group3,group4,group5,group6,group7,group8,group9;
	//没有则返回0
	//主机树
	group0 = treeLookup(active.addrTree, ntohl(inet_addr("192.168.130.205")), active.num_groups);
	group1 = treeLookup(active.addrTree, ntohl(inet_addr("192.168.0.13")), active.num_groups);
	group1 = treeLookup(active.addrTree, ntohl(inet_addr("192.168.0.14")), active.num_groups);
	group2=treeLookup(active.addrTree, ntohl(inet_addr("192.168.3.11")), active.num_groups);//no
	group2=treeLookup(active.addrTree, ntohl(inet_addr("192.168.11.11")), active.num_groups);//no
	//允许树
	group3 = treeLookup(active.acceptTree, ntohl(inet_addr("192.168.130.205")), active.num_groups);
	group4 = treeLookup(active.acceptTree, ntohl(inet_addr("192.168.0.21")), active.num_groups);//no
	group4 = treeLookup(active.acceptTree, ntohl(inet_addr("192.168.13.21")), active.num_groups);//no
	//拒绝树
	group5 = treeOutLookup(active.rejectTree, ntohl(inet_addr("192.168.130.205")), active.num_groups);
	group6 = treeLookup(active.acceptTree, ntohl(inet_addr("192.168.13.21")), active.num_groups);//no
	group7= treeLookup(active.In_FtpTree, ntohl(inet_addr("111.11.111.11")), active.num_groups);//no
	group8 = treeLookup(active.In_HttpTree, ntohl(inet_addr("111.11.111.11")), active.num_groups);//no
	group9 = treeLookup(active.In_SmtpTree, ntohl(inet_addr("111.11.111.11")), active.num_groups);//no
*/
}
