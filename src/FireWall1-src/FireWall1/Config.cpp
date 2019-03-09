
#include "StdAfx.h"
#include "Struct.h"
#include "ip.h"
#include "ipfilter.h"
#include "winsock2.h"
#include "config.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern  FilterConfig filtercfg;//ipfilter.cpp
extern  LogManage TheLogManage;
extern  DefaultSetData TheDefaultSetData;
extern  BYTE INNO;
extern  BYTE OUTNO;
extern  BYTE DMNO;
static unsigned int netmask(unsigned int);
static int numzeros(ULONG a);
static int
numzeros(ULONG a)
{
	register int i;

	for (i=0; (i < 32) && !(a & 1); i++, a >>= 1);
	return i;
}       

static unsigned int netmask(unsigned int bits)
{
	return (0xFFFFFFFFUL << ((bits < 32) ? 32 - bits : 0));
}
VOID StartFilter()
{
   manageTablesReq *NewToActive=new manageTablesReq;
   NewToActive->command=ACTIVATE_NEW;
   NewToActive->val=0;//类的最大数；在此函数下无用
   filter_command(DIOCMNETWORK,(char*)NewToActive);
   filter_command(DIOCSTART,NULL);
   delete NewToActive;
}

VOID SetFilterlogMask()
{
   return;
}
int SetDmIpTab(CPtrList &listRejIp,UCHAR type)
{
   int tt=sizeof(addrTreeNode);
   rejaccTableReq  *pRej=new rejaccTableReq;

   int count=listRejIp.GetCount();
   pRej->ptr=new RejAccTableEntry[count];
   struct IpaddrRange *iprang;
   POSITION pos;
   int i=0;
   pos=listRejIp.GetHeadPosition();
   while(pos!=NULL)
   {
	   iprang=(struct IpaddrRange *)listRejIp.GetNext(pos);
	   pRej->ptr[i].network.ss_addr=ntohl(iprang->nIpAddr);
	   pRej->ptr[i].bits=iprang->nIpaddrNum;
	   pRej->ptr[i].flag=1;//恒定
	   i++;
   }

 
   pRej->addr=0;//无用！！
   pRej->bytes=count*(sizeof(RejAccTableEntry));//bytes 数
   int ret;
   switch(type)
   {
   case T_INDM:
	   ret= filter_command(DIOCS_INDM,(char*)pRej);
	   break;
   case T_OUTDM:
	   ret= filter_command(DIOCS_OUTDM,(char*)pRej);
	   break;
   }
   delete pRej->ptr;
   delete pRej;
   return ret;
}
int SetFilterAcceptTab(CPtrList &listRejIp)
{
   int tt=sizeof(addrTreeNode);
   rejaccTableReq  *pRej=new rejaccTableReq;

   int count=listRejIp.GetCount();
   pRej->ptr=new RejAccTableEntry[count];
   struct IpaddrRange *iprang;
   POSITION pos;
   int i=0;
   pos=listRejIp.GetHeadPosition();
   while(pos!=NULL)
   {
	   iprang=(struct IpaddrRange *)listRejIp.GetNext(pos);
	   pRej->ptr[i].network.ss_addr=ntohl(iprang->nIpAddr);
	   pRej->ptr[i].bits=iprang->nIpaddrNum;
	   pRej->ptr[i].flag=1;//恒定
	   i++;
   }

   pRej->bytes=count*(sizeof(RejAccTableEntry));//bytes 数

   int ret= filter_command(DIOCSREJECT,(char*)pRej);
   delete pRej->ptr;
   delete pRej;
   return ret;
}

int SetFilterRejTab(CPtrList &listRejIp)
{
   int tt=sizeof(addrTreeNode);
   rejaccTableReq  *pRej=new rejaccTableReq;

   int count=listRejIp.GetCount();
   pRej->ptr=new RejAccTableEntry[count];
   struct IpaddrRange *iprang;
   POSITION pos;
   int i=0;
   pos=listRejIp.GetHeadPosition();
   while(pos!=NULL)
   {
	   iprang=(struct IpaddrRange *)listRejIp.GetNext(pos);
	   pRej->ptr[i].network.ss_addr=ntohl(iprang->nIpAddr);
	   pRej->ptr[i].bits=iprang->nIpaddrNum;
	   pRej->ptr[i].flag=1;//恒定
	   i++;
   }
   pRej->bytes=count*(sizeof(RejAccTableEntry));//bytes 数

   int ret=filter_command(DIOCSACCEPT,(char*)pRej);
   delete  []pRej->ptr;
   delete pRej;
   return ret;

}

//设置端口列表请求的同时设置地址表
//通过该地址树查找该地址所对应的端口列表所在的位置
VOID SetFilterAddrTree(struct IpaddrRange iprange,USHORT group)
{
   addrTreeReq *paddrTree=new addrTreeReq;
   paddrTree->num=0;//节点的总数
   paddrTree->addr[paddrTree->num].ss_addr=ntohl(iprange.nIpAddr);
  
   paddrTree->bits[paddrTree->num]=iprange.nIpaddrNum;
   paddrTree->group[paddrTree->num]=group;//所属的主机类
   paddrTree->num++;
   filter_command(DIOCSNETWORK,(char*)paddrTree);
   delete paddrTree;
}
//END????????????????????????????????????????????????
//对于设置的每一台主机分别调用;
VOID SetFilterPortList(struct 	HostList* phostlist,UCHAR type,unsigned short group)
{
   
   portListReq *  portlistreq=new  portListReq; 
   portlistreq->group=group;
   switch (type) 
   {
	case TCPSRCOUT:  
		{
	   ULONG num=phostlist->sTcpPortSet.sAllowOutSrcPortList.GetCount();
	   struct PortList *portlist;
       portlistreq->ptr=new PortListEntry[num];
       portlistreq->id=T_SRC_OUT;
	   int i=0;
	   POSITION pos;
	   pos=phostlist->sTcpPortSet.sAllowOutSrcPortList.GetHeadPosition();
	   while(pos!=NULL)
	   {
		  portlist=(struct PortList *)phostlist->sTcpPortSet.sAllowOutSrcPortList.GetNext(pos);
		  if(portlist->bRange)
		  {
			  portlistreq->ptr[i].begin=portlist->sPortRange.nPortBegin;
			  portlistreq->ptr[i].end=portlist->sPortRange.nPortEnd;
		  }
		  else
		  {
			  if(portlist->nPort==0)
			  {
				  num--;
			  	i--;
			  }
			  else
			  {
			  portlistreq->ptr[i].begin=portlist->nPort;
			  portlistreq->ptr[i].end=portlist->nPort;
			  }
		  }
		  i++;
	   }
	        portlistreq->bytes=num*sizeof(PortListEntry);
	       filter_command(DIOCSGROUP,(char*)portlistreq);
	delete []portlistreq->ptr;
    delete portlistreq;
		}
        break;
	case TCPDSTOUT:  
		{
	   ULONG num=phostlist->sTcpPortSet.sAllowOutDstPortList.GetCount();
	   struct PortList *portlist;
       portlistreq->ptr=new PortListEntry[num];
       portlistreq->id=T_DST_OUT;
	   int i=0;
	   POSITION pos;
	   pos=phostlist->sTcpPortSet.sAllowOutDstPortList.GetHeadPosition();
	   while(pos!=NULL)
	   {
		  portlist=(struct PortList *)phostlist->sTcpPortSet.sAllowOutDstPortList.GetNext(pos);
		  if(portlist->bRange)
		  {
			  portlistreq->ptr[i].begin=portlist->sPortRange.nPortBegin;
			  portlistreq->ptr[i].end=portlist->sPortRange.nPortEnd;
		  }
		  else
		  {
			  if(portlist->nPort==0)
			  {
				  num--;
				   i--;
			  }
			  else
			  {
			  portlistreq->ptr[i].begin=portlist->nPort;
			  portlistreq->ptr[i].end=portlist->nPort;
			  }
		  }
		  i++;
	   }
       portlistreq->bytes=num*sizeof(PortListEntry);
	       filter_command(DIOCSGROUP,(char*)portlistreq);
	delete []portlistreq->ptr;
    delete portlistreq;
		}
        break;
	case UDPSRCOUT:  
		{
	   ULONG num=phostlist->sUdpPortSet.sAllowOutSrcPortList.GetCount();
	   struct PortList *portlist;
       portlistreq->ptr=new PortListEntry[num];
       portlistreq->id=U_SRC_OUT;
	   int i=0;
	   POSITION pos;
	   pos=phostlist->sUdpPortSet.sAllowOutSrcPortList.GetHeadPosition();
	   while(pos!=NULL)
	   {
		  portlist=(struct PortList *)phostlist->sUdpPortSet.sAllowOutSrcPortList.GetNext(pos);
		  if(portlist->bRange)
		  {
			  portlistreq->ptr[i].begin=portlist->sPortRange.nPortBegin;
			  portlistreq->ptr[i].end=portlist->sPortRange.nPortEnd;
		  }
		  else
		  {
			  if(portlist->nPort==0)
			  {
				  num--;
				  i--;
			  }
			  else
			  {
			  portlistreq->ptr[i].begin=portlist->nPort;
			  portlistreq->ptr[i].end=portlist->nPort;
			  }
		  }
		  i++;
	   }
       portlistreq->bytes=num*sizeof(PortListEntry);
	       filter_command(DIOCSGROUP,(char*)portlistreq);
	delete []portlistreq->ptr;
    delete portlistreq;
		}
        break;
	case UDPDSTOUT:  
		{
	   ULONG num=phostlist->sUdpPortSet.sAllowOutDstPortList.GetCount();
	   struct PortList *portlist;
       portlistreq->ptr=new PortListEntry[num];
       portlistreq->id=U_DST_OUT;
	   int i=0;
	   POSITION pos;
	   pos=phostlist->sUdpPortSet.sAllowOutDstPortList.GetHeadPosition();
	   while(pos!=NULL)
	   {
		  portlist=(struct PortList *)phostlist->sUdpPortSet.sAllowOutDstPortList.GetNext(pos);
		  if(portlist->bRange)
		  {
			  portlistreq->ptr[i].begin=portlist->sPortRange.nPortBegin;
			  portlistreq->ptr[i].end=portlist->sPortRange.nPortEnd;
		  }
		  else
		  {
			  if(portlist->nPort==0)
			  {
				  num--;
				  				  i--;
			  }
			  else
			  {
			  portlistreq->ptr[i].begin=portlist->nPort;
			  portlistreq->ptr[i].end=portlist->nPort;
			  }
		  }
		  i++;
	   }
       portlistreq->bytes=num*sizeof(PortListEntry);
	       filter_command(DIOCSGROUP,(char*)portlistreq);
	delete []portlistreq->ptr;
    delete portlistreq;
		}
        break;
	case ICMPTYPEIN:
		{
	   ULONG num=phostlist->sInIcmpSet.GetCount();
	   struct IcmpList *portlist;
       portlistreq->ptr=new PortListEntry[num];
       portlistreq->id=I_TYP_IN;
	   int i=0;
	   POSITION pos;
	   pos=phostlist->sInIcmpSet.GetHeadPosition();
	   while(pos!=NULL)
	   {
		  portlist=(struct IcmpList *)phostlist->sInIcmpSet.GetNext(pos);
		  if(portlist->bRange)
		  {
			  portlistreq->ptr[i].begin=portlist->sIcmpRange.nIcmpBegin;
			  portlistreq->ptr[i].end=portlist->sIcmpRange.nIcmpEnd;
		  }
		  else
		  {
		   if(portlist->nIcmpType==0)
			  {
				  num--;
				  				  i--;
			  }
			  else
			  {
			  //ICM类型规定从1开始！！！因此减一
				  portlistreq->ptr[i].begin=portlist->nIcmpType-1;
			  portlistreq->ptr[i].end=portlist->nIcmpType-1;
			  }
		  }
		  i++;
	   }
       portlistreq->bytes=num*4;
	       filter_command(DIOCSGROUP,(char*)portlistreq);
	delete []portlistreq->ptr;
    delete portlistreq;
		}
        break;
	case ICMPTYPEOUT:
		{
	   ULONG num=phostlist->sOutIcmpSet.GetCount();
	   struct IcmpList *portlist;
       portlistreq->ptr=new PortListEntry[num];
       portlistreq->id=I_TYP_OUT;
	   int i=0;
	   POSITION pos;
	   pos=phostlist->sOutIcmpSet.GetHeadPosition();
	   while(pos!=NULL)
	   {
		  portlist=(struct IcmpList *)phostlist->sOutIcmpSet.GetNext(pos);
		  if(portlist->bRange)
		  {
			  portlistreq->ptr[i].begin=portlist->sIcmpRange.nIcmpBegin;
			  portlistreq->ptr[i].end=portlist->sIcmpRange.nIcmpEnd;
		  }
		  else
		  {
			  if(portlist->nIcmpType==0)
			  {
				  num--;
				  				  i--;
			  }
			  else				  
			  {
			  //ICM类型规定从1开始！！！因此减一
				  portlistreq->ptr[i].begin=portlist->nIcmpType-1;
			  portlistreq->ptr[i].end=portlist->nIcmpType-1;
			  }
		  }
		  i++;
	   }
	          portlistreq->bytes=num*4;
	       filter_command(DIOCSGROUP,(char*)portlistreq);
	delete []portlistreq->ptr;
    delete portlistreq;
		}
        break;
	default:
     delete portlistreq;
		return;
   }

}


//首先必须调用次函数
VOID SetFilterInitReq(struct FireWallConfig  mycfg)
{
//	initReq * inireq=new initReq;
	filter_command(DIOCINIT,(char*)NULL);
//	delete inireq;
}
VOID SetFilterCfg(struct FireWallConfig  mycfg,struct GlobalFlags myflags)
{
   filtercfg.discardAttackICMP=myflags.bSusPectIcmp;
   filtercfg.discardIcmp=myflags.bforbidIcmp;
   filtercfg.discardFragmentedICMP=myflags.bFragmentIcmp;
   filtercfg.discardMulticast=myflags.bMulticast;
   filtercfg.discardNonIp=myflags.bNonIpPacket;
   filtercfg.discardOtherIp=myflags.bOtherIpPacket;
//   filtercfg.discardRouteIp=myflags.现无
   filtercfg.discardSuspectOffset=myflags.bSusPectOffset;
   filtercfg.dm_ip=ntohl(mycfg.nMiddleIp);
   filtercfg.dm_mask=ntohl(mycfg.nMiddleMask);
   filtercfg.dm_number=DMNO-0X30;//mycfg.MdNumber;//for test!!
   filtercfg.in_number=INNO-0X30;//mycfg.InNumber;
   filtercfg.in_ip=ntohl(mycfg.nIntraIp);
   filtercfg.in_mask=ntohl(mycfg.nIntraMask);
   filtercfg.logMask=TheLogManage.TheIpFilter.Options|0XFFFE0000;//根据日志管理器设置
   filtercfg.media_type=IFT_ETHER;//现在硬编码在此:以太
   filtercfg.out_ip=ntohl(mycfg.nOutsideIp);
   filtercfg.out_mask=ntohl(mycfg.nOutsideMask);
   filtercfg.out_number=OUTNO-0X30;//mycfg.OutNumber;
}


VOID ClearStatisticsInfo()
{
	filter_command(DIOCCSTATS,(char*)NULL);
}