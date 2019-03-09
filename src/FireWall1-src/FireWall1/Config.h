
#if !defined(FILTERCONFIG)
#define FILTERCONFIG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Struct.h"
#include "ip.h"
#include "ipfilter.h"
//add function declaretion
VOID SetFilterlogMask();
int  SetFilterRejTab(CPtrList &listRejIp);
int  SetFilterAcceptTab(CPtrList &listRejIp);
VOID SetFilterAddrTree(struct IpaddrRange iprange,USHORT group);
VOID SetFilterPortList(struct 	HostList* phostlist,UCHAR type,unsigned short group);
VOID SetFilterCfg(struct FireWallConfig  mycfg,struct GlobalFlags myflags);
VOID StartFilter();
int  SetDmIpTab(CPtrList &listRejIp,UCHAR type);
VOID ClearStatisticsInfo();

#endif