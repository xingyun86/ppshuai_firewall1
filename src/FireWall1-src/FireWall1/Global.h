
#if !defined(FIREWALLGLOBAL)
#define FIREWALLGLOBAL

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include "Struct.h"
#define  ALLOWIPTAB  22
#define  REJIPTAB  23
#define  HOSTIP   24
#define  INITALLOWIPTAB 25
#define  INITREJIPTAB  26
#define   MAPSTATIC_UDP 29
#define   MAPSTATIC_TCP  30
#define  INITINDMTAB 31
#define  INITOUTDMTAB  32
#define   INDM  37
#define   OUTDM 38
#define   INSMTP   41
#define   OUTSMTP  42

class CDataConvert
{ 
public:
	CPtrList m_ptrlistIpRange;//仅为了删除相应类型的空间
	CPtrList m_ptrlistUlong;//仅为了删除相应类型的空间
	CPtrList m_ptrlistIcmp,m_ptrlistIcmpOut;//仅为了删除相应类型的空间
	CDataConvert();
	~CDataConvert();
	struct AllLineChar   m_alllinechar;
	ULONG m_CurIp;
    char m_cCurIp[24];
    struct GlobalFlags  globalFlags;
    CPtrList  m_ptrlistRejectIp,m_ptrlistdel;
    CPtrList  m_ptrlistAllowIp,m_listInHttp,m_listOutHttp,m_listInFtp;//items:
	CPtrList  m_listOutFtp,m_listInSmtp,m_listOutSmtp;
    struct  FireWallConfig m_firewallconfig;
    CPtrList m_ptrlistHostPort;//items:struct  HostList
    CPtrList m_ptrlistPort;////注意：m_ptrlistPort,m_ptrlistdel只是为了删除空间
	VOID  AddIpTable(UCHAR type,CEdit &edit);//添加ip地址到链表中
	//将主机类的ip地址范围添加到主机类列表
    VOID  SetHostIp(CString m_HostIp,BOOL &bhaved,struct HostChar*phostchar);
	//将主机类的端口设置添加到主机类列表中
    VOID  SetPortList(CEdit &edit,USHORT type);
	// 设置NAT模块所需配置数据
	VOID  SetNatCfg(struct  NAT_Cfg *natcfg);
	//已字符方式保存用户设置，以便一次写入文件
    VOID  SaveChar(CEdit &edit,BOOL ishostCfg,UCHAR type);
	//写入文件
    VOID   WriteToFile();
	//从文件读出
	VOID  ReadFromFile();
	//填充struct  FireWallConfig结构
    VOID  InitFireWall();//m_firewallconfig
	//将编辑框中的内容转换为配置数据类型
    VOID  ConvertEditChar(char *pchar,ULONG &mylong,USHORT &mynum,BOOL bIp);
	//显示特定的主机类设置
    VOID  DisplayHost(struct HostChar *phostchar,USHORT type,CEdit &edit);
	//从文件中装载用户上次主机类端口设置
    VOID  InitPortList(USHORT type,int &ii,struct HostList *pHostList);
	//从文件中装载用户上次主机类设置，其中调用了InitPortList（）函数
    VOID  InitHostList();
    //显示网络策略模块数据
    VOID  DisplayFireCfg(CEdit &edit,USHORT type);
    //删除特定的主机类
    VOID  DeleteHost(CString m_HostIp);

};
#endif 