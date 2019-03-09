
#include "stdafx.h"
#include "winsock2.h"
#include "global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDataConvert::CDataConvert()
{
  m_CurIp=0;
  ZeroMemory(m_cCurIp,24);
//  ZeroMemory(&m_alllinechar,sizeof(struct AllLineChar));
  m_alllinechar.CurHostNum=0; 

}
CDataConvert::~CDataConvert()
{
  while(!m_ptrlistAllowIp.IsEmpty())
  {
	struct IpaddrRange   *IpRange=(struct IpaddrRange *)m_ptrlistAllowIp.RemoveHead();
	delete IpRange;
  }
   while(!m_ptrlistRejectIp.IsEmpty())
  {
	struct IpaddrRange   *IpRange=(struct IpaddrRange *)m_ptrlistRejectIp.RemoveHead();
	delete IpRange;
  }
  //先删除m_ptrlistHostPort里面的struct PortList的pPortList所指向的内容 

  while(!m_ptrlistUlong.IsEmpty())
  {
	ULONG  *LON=(ULONG*)m_ptrlistUlong.RemoveHead();
	delete LON;
  }
  while(!m_ptrlistIcmp.IsEmpty())
  {
	struct IcmpList   *icmprange=(struct IcmpList *)m_ptrlistIcmp.RemoveHead();
	delete icmprange;
  }
   while(!m_ptrlistIcmpOut.IsEmpty())
  {
	struct IcmpList   *icmprange=(struct IcmpList *)m_ptrlistIcmpOut.RemoveHead();
	delete icmprange;
  }
  while(!m_ptrlistPort.IsEmpty())
  {
	struct PortList   *portlist=(struct PortList *)m_ptrlistPort.RemoveHead();
	delete portlist;
  }
  //后删除主结构
  while(!m_ptrlistHostPort.IsEmpty())
  {
	struct HostList   *phostlist=(struct HostList *)m_ptrlistHostPort.RemoveHead();
	delete phostlist;
  }
  while(!m_listInHttp.IsEmpty())
  {
	struct IpaddrRange   *IpRange=(struct IpaddrRange *)m_listInHttp.RemoveHead();
	delete IpRange;
  }
   while(!m_listOutHttp.IsEmpty())
  {
	struct IpaddrRange   *IpRange=(struct IpaddrRange *)m_listOutHttp.RemoveHead();
	delete IpRange;
  }
//delete []natcfg->StaticTable.item;
}
//将用户的需要回显的数据还要以字符形式保存,便于现示
//调用SetHostIp后，不能调用SaveChar
//仅对CEdit型EDIT控件调用该函数
//对CString型EDIT控件直接付值
//主机ip的存取在SetHostIp()函数中直接进行了付值
VOID CDataConvert::SaveChar(CEdit &edit,BOOL ishostCfg,UCHAR type)
{
  	int  Lines=edit.GetLineCount();
	char tempchar[20];
	CString csPort;

	int pos;
	pos=m_alllinechar.CurHostNum;
	//考虑修改的情况
	if(ishostCfg)
	{
	  for(int ii=0;ii<m_alllinechar.CurHostNum;ii++)
	  {
		if(strcmp(m_cCurIp,m_alllinechar.Hostchar[ii].HostIp)==0)
		{
			pos=ii;
			break;
		}
	  }
	}
	switch(type)
	{
	case MAPSTATIC_UDP:
		{
		m_alllinechar.FireWallCfg.sNatCfg.us_PortNumberUdp=0;
		memset(m_alllinechar.FireWallCfg.sNatCfg.MapStaticUdp,0,sizeof(m_alllinechar.FireWallCfg.sNatCfg.MapStaticUdp));
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 		 if(nret==0)continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.FireWallCfg.sNatCfg.MapStaticUdp[i],csPort.GetBuffer(len));
		 m_alllinechar.FireWallCfg.sNatCfg.us_PortNumberUdp++;
		}
		}
		break;
	case MAPSTATIC_TCP:
		{
		m_alllinechar.FireWallCfg.sNatCfg.us_PortNumberTcp=0;
		memset(m_alllinechar.FireWallCfg.sNatCfg.MapStaticTcp,0,sizeof(m_alllinechar.FireWallCfg.sNatCfg.MapStaticTcp));
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 		 if(nret==0)continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.FireWallCfg.sNatCfg.MapStaticTcp[i],csPort.GetBuffer(len));
		 m_alllinechar.FireWallCfg.sNatCfg.us_PortNumberTcp++;
		}
		}
		break;
	case TCPSRCOUT:
		{
		m_alllinechar.Hostchar[pos].nPos[TCPSRCOUT]=0;
		memset(m_alllinechar.Hostchar[pos].TcpSrcOut,0,sizeof(m_alllinechar.Hostchar[pos].TcpSrcOut));
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.Hostchar[pos].TcpSrcOut[i],csPort.GetBuffer(len));
         m_alllinechar.Hostchar[pos].nPos[TCPSRCOUT]++;
		}
		}
		break;
    case TCPDSTOUT:
		{
		m_alllinechar.Hostchar[pos].nPos[TCPDSTOUT]=0;
		memset(m_alllinechar.Hostchar[pos].TcpDstOut,0,sizeof(m_alllinechar.Hostchar[pos].TcpDstOut));
//		m_alllinechar.TcpDstOut=new  char[Lines][30];
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.Hostchar[pos].TcpDstOut[i],csPort.GetBuffer(len));
		 m_alllinechar.Hostchar[pos].nPos[TCPDSTOUT]++;
		}
		}
		break;

	case UDPSRCOUT:
		{
		m_alllinechar.Hostchar[pos].nPos[UDPSRCOUT]=0;
		memset(m_alllinechar.Hostchar[pos].UdpSrcOut,0,sizeof(m_alllinechar.Hostchar[pos].UdpSrcOut));
//		m_alllinechar.UdpSrcOut=new  char[Lines][30];
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.Hostchar[pos].UdpSrcOut[i],csPort.GetBuffer(len));
		 m_alllinechar.Hostchar[pos].nPos[UDPSRCOUT]++;
		}
		}
		break;

	case UDPDSTOUT:
		{
		m_alllinechar.Hostchar[pos].nPos[UDPDSTOUT]=0;
		memset(m_alllinechar.Hostchar[pos].UdpDstOut,0,sizeof(m_alllinechar.Hostchar[pos].UdpDstOut));
//		m_alllinechar.TcpDstIn=new  char[Lines][30];
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.Hostchar[pos].UdpDstOut[i],csPort.GetBuffer(len));
		 m_alllinechar.Hostchar[pos].nPos[UDPDSTOUT]++;
		}
		}
		break;
	case ICMPTYPEIN:
		{
		m_alllinechar.Hostchar[pos].nPos[ICMPTYPEIN]=0;
		memset(m_alllinechar.Hostchar[pos].IcmpTypeIn,0,sizeof(m_alllinechar.Hostchar[pos].IcmpTypeIn));
//		m_alllinechar.IcmpTypeIn=new  char[Lines][30];
		for(int i=0;i<Lines;i++)
		{
          int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.Hostchar[pos].IcmpTypeIn[i],csPort.GetBuffer(len));
		m_alllinechar.Hostchar[pos].nPos[ICMPTYPEIN]++;
		}
		}
		break;
	case ICMPTYPEOUT:
		{
		m_alllinechar.Hostchar[pos].nPos[ICMPTYPEOUT]=0;
		memset(m_alllinechar.Hostchar[pos].IcmpTypeOut,0,sizeof(m_alllinechar.Hostchar[pos].IcmpTypeOut));
   //     m_alllinechar.IcmpTypeOut=new  char[Lines][30];
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.Hostchar[pos].IcmpTypeOut[i],csPort.GetBuffer(len));
		 m_alllinechar.Hostchar[pos].nPos[ICMPTYPEOUT]++;
		}
		}
		break;
	case INDM:
		{
		m_alllinechar.InDmPos=Lines;
		memset(m_alllinechar.InDmIpTab,0,sizeof(m_alllinechar.InDmIpTab));
       // m_alllinechar.AllowIpTab=new  char[Lines][30];
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)
		 {
			 m_alllinechar.InDmPos--;
			 continue;
		 }
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.InDmIpTab[i],csPort.GetBuffer(len));
		}
		}
		break;
	case OUTDM:
		{
		m_alllinechar.OutDmPos=0;
		memset(m_alllinechar.OutDmIpTab,0,sizeof(m_alllinechar.OutDmIpTab));
       // m_alllinechar.AllowIpTab=new  char[Lines][30];
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)
			continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.OutDmIpTab[i],csPort.GetBuffer(len));
		 m_alllinechar.OutDmPos++;
		}
		}
		break;

	case  ALLOWIPTAB:
		{
		m_alllinechar.AllowPos=0;
		memset(m_alllinechar.AllowIpTab,0,sizeof(m_alllinechar.AllowIpTab));
       // m_alllinechar.AllowIpTab=new  char[Lines][30];
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)
			 continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.AllowIpTab[i],csPort.GetBuffer(len));
		 m_alllinechar.AllowPos++;
		}
		}
		break;
	case  REJIPTAB:
		{
		m_alllinechar.RejPos=0;
		memset(m_alllinechar.RejIpTab,0,sizeof(m_alllinechar.RejIpTab));
      //  m_alllinechar.RejIpTab=new  char[Lines][30];
		for(int i=0;i<Lines;i++)
		{
         int nret=edit.GetLine(i,tempchar); 
		 if(nret==0)
			 continue;
		 csPort=(CString)tempchar;
		 int len=csPort.GetLength();
	     csPort.Delete(nret,len-nret);//不能超过缓冲区大小
         strcpy(m_alllinechar.RejIpTab[i],csPort.GetBuffer(len));
		m_alllinechar.RejPos++;
		}
		}
		break;
	default:
		return;
	}
}

VOID CDataConvert::WriteToFile()
{
  	CFile myFile;
	myFile.Open("UserSet.txt",CFile::modeCreate|CFile::modeWrite,NULL);	
    ULONG size=sizeof(struct AllLineChar);
	myFile.Write(&m_alllinechar, size);
	//myFile.WriteHuge(&m_alllinechar, size);
	myFile.Close();
}
VOID CDataConvert::ReadFromFile()
{
  	CFile myFile;
	if(myFile.Open("UserSet.txt",CFile::modeRead,NULL)==0)
		return;
    ULONG size=sizeof(struct AllLineChar);
	myFile.Read(&m_alllinechar, size);
	//myFile.ReadHuge(&m_alllinechar, size);
	myFile.Close();
}

//添加拒绝IP表和允许IP表
VOID CDataConvert::AddIpTable(UCHAR type,CEdit &edit)
{
// TODO: Add extra validation here
	BOOL  bFirst=TRUE;	
	char Ipaddr[20];
	ULONG   nIpaddr;
	char pIpaddr[30];
	int  IpCount=0;
	int  Lines;
	//begin系统初始化时装载以前的用户设置数据，（也可以：无需！当用户重新设置后生效）
	switch(type)
	{
	case INITALLOWIPTAB:
		Lines=m_alllinechar.AllowPos;
		break;
	case INITREJIPTAB:
		Lines=m_alllinechar.RejPos;
		break;
	case INITINDMTAB:
		Lines=m_alllinechar.InDmPos;
		break;
	case INITOUTDMTAB:
		Lines=m_alllinechar.OutDmPos;
		break;
	case ALLOWIPTAB:
		while(!m_ptrlistAllowIp.IsEmpty())
		{
	          struct IpaddrRange   *IpRange1=(struct IpaddrRange *)m_ptrlistAllowIp.RemoveHead();
	          delete IpRange1;
		}
        Lines=edit.GetLineCount();
		break;
    case REJIPTAB:
		while(!m_ptrlistRejectIp.IsEmpty())//重新设定后先清空
		{
	         struct IpaddrRange   *IpRange2=(struct IpaddrRange *)m_ptrlistRejectIp.RemoveHead();
	         delete IpRange2;
		}
        Lines=edit.GetLineCount();
		break;
	case INDM:
     while(!m_listInHttp.IsEmpty())
	 { 
	struct IpaddrRange   *IpRange=(struct IpaddrRange *)m_ptrlistRejectIp.RemoveHead();
	delete IpRange;
	 } 
	 break;
	case OUTDM:
    while(!m_listOutHttp.IsEmpty())
	{ 
	struct IpaddrRange   *IpRange=(struct IpaddrRange *)m_ptrlistRejectIp.RemoveHead();
	delete IpRange;
	}  
	break;
  default:
		return;
	}
   //end
   //     Lines=edit.GetLineCount();
    CString  csIpaddr;
    int nret;
	for(int i=0;i<Lines;i++)
	{
	  csIpaddr.Empty();
	  memset(Ipaddr,0,20);
	 //begin系统初始化时装载以前的用户设置数据，（也可以：无需！当用户重新设置后生效）
	 switch(type)
	 {
	 case INITALLOWIPTAB:
		memcpy(pIpaddr,m_alllinechar.AllowIpTab[i],30);
		nret=strlen(pIpaddr);
		break;
	 case INITREJIPTAB:
		memcpy(pIpaddr,m_alllinechar.RejIpTab[i],30);
		nret=strlen(pIpaddr);
		break;
	 case INITINDMTAB:
		memcpy(pIpaddr,m_alllinechar.InDmIpTab[i],30);
		nret=strlen(pIpaddr);
		break;
	 case INITOUTDMTAB:
		memcpy(pIpaddr,m_alllinechar.OutDmIpTab[i],30);
		nret=strlen(pIpaddr);
		break;
	 default:
		 {
	       nret=edit.GetLine(i,pIpaddr);
	       if(nret==0)continue;//空行
		 }
	 }
   //end
	//  int nret=edit.GetLine(i,pIpaddr);
	 // if(nret==0)continue;//空行
      csIpaddr=(CString)pIpaddr;
	  int pos,nrem;
      csIpaddr.Delete(nret,29-nret);//不能超过缓冲区大小
	  pos=csIpaddr.Find(':');
	  nrem=csIpaddr.Remove(' ');
	  struct IpaddrRange   *IpRange=new  struct IpaddrRange;
	  if(pos!=-1)
	  {
	    CString str1;
	    str1=csIpaddr.SpanExcluding(":");
	   // Ipaddr=str1.GetBuffer(str1.GetLength());
		int i5=str1.GetLength();
		memcpy(Ipaddr,str1.GetBuffer(i5),i5);

        nIpaddr=inet_addr(Ipaddr);
		if(nIpaddr==INADDR_NONE)
		{
			AfxMessageBox("有不正确的IP地址格式，请重新输入！");
			delete IpRange;
			break;
		}
	    CString  str;
	    int i2=nret-pos;
		//if()
	    str=csIpaddr.Mid(pos-nrem+1,i2);
		IpRange->nIpAddr=nIpaddr;
        IpRange->nIpaddrNum=atoi(str.GetBuffer(i2));
	  //  IpRange->nIpMask=;.................add
	  }
	  else
	  {
	  //  IpRange->nIpMask=;.................add
		memcpy(Ipaddr,csIpaddr.GetBufferSetLength(nret-nrem),nret-nrem);
        nIpaddr=inet_addr(Ipaddr);
		if(nIpaddr==INADDR_NONE)
		{
			AfxMessageBox("有不正确的IP地址格式，请重新输入！");
			delete IpRange;
			break;
		}
        IpRange->nIpAddr=nIpaddr;
        IpRange->nIpaddrNum=1;
	  }
	  switch(type)
	  {
	  case INITALLOWIPTAB:
	  case ALLOWIPTAB:
	     m_ptrlistAllowIp.AddHead(IpRange);
		 break;
	  case INITREJIPTAB:
	  case REJIPTAB:
		  m_ptrlistRejectIp.AddHead(IpRange);
		 break;
	  case  INITINDMTAB:
	  case	INDM:
		  m_listInHttp.AddHead(IpRange);
		  break;
	  case INITOUTDMTAB:
	  case OUTDM:
		  m_listOutHttp.AddHead(IpRange);
		  break;
	  }
	}
}
VOID CDataConvert:: DeleteHost(CString m_HostIp)
{
    int nrem=m_HostIp.Remove(' ');
    int pos=m_HostIp.Find(':');
    int i1=m_HostIp.GetLength();
	BOOL bhave=FALSE;
	for(int i=0;i<m_alllinechar.CurHostNum;i++)
	{
		 if(strcmp(m_alllinechar.Hostchar[i].HostIp,m_HostIp.GetBufferSetLength(24))==0)
		 {//循环移位，填充删除的主机信息
			 int k=i;
			 bhave=TRUE;
			 memset((struct HostChar*)&m_alllinechar.Hostchar[i],0,sizeof(struct HostChar));
			 for(int j=i+1;j<m_alllinechar.CurHostNum;j++)
			 {
                memcpy((struct HostChar*)&m_alllinechar.Hostchar[k],(struct HostChar*)&m_alllinechar.Hostchar[j],sizeof(struct HostChar));
				k++;
			    memset((struct HostChar*)&m_alllinechar.Hostchar[k],0,sizeof(struct HostChar));
			 }
           m_alllinechar.CurHostNum--;
		 //  break;
		 }
	}
//删除链表中的对应项
	if(bhave)
	{
     struct HostList*  phostlist;
     int nIp;
	 if(pos!=-1)
	 {
	    CString str1;
	    str1=m_HostIp.SpanExcluding(":");
	    nIp=inet_addr(str1.GetBuffer(str1.GetLength()));
		if(nIp==INADDR_NONE)
		{
			AfxMessageBox("有不正确的IP地址格式，请重新输入！");
			return;
		}
	   POSITION pos,curpos;
	   pos=m_ptrlistHostPort.GetHeadPosition();
	   while(pos!=NULL)
	   {
		  curpos=pos;
          phostlist=(struct HostList*)m_ptrlistHostPort.GetNext(pos);
	      if(phostlist->sIpRange.nIpAddr==nIp)	
		  {//所有的端口列表在退出时删除
               m_ptrlistHostPort.RemoveAt(curpos);
			   delete phostlist;
		       break;
		  }
	   }

	 }
     else
	 {
	   nIp=inet_addr(m_HostIp.GetBuffer(i1));
	   if(nIp==INADDR_NONE)
	   {
			AfxMessageBox("有不正确的IP地址格式，请重新输入！");
			return;
	   }
	   POSITION pos,curpos;
	   pos=m_ptrlistHostPort.GetHeadPosition();
	   while(pos!=NULL)
	   {
		  curpos=pos;
          phostlist=(struct HostList*)m_ptrlistHostPort.GetNext(pos);
	      if(phostlist->sIpRange.nIpAddr==nIp)	
		  {
               m_ptrlistHostPort.RemoveAt(curpos);
			   delete phostlist;
		       break;
		  }
	   }
	 } 
	} 
}

//设定主机IP或IP范围；决定了SetPortList()函数中的端口列表
//如果bhaved为TRUE，则取得具体的主机；以让用户修改主机的所有信息
//调用函数中无需对phost分配空间
VOID CDataConvert::SetHostIp(CString m_HostIp,BOOL &bhaved,struct  HostChar *phostchar)
{
    //初始化，有效的区分tcp,udp与ftp,http,telnet对端口的相应设置
	bhaved=FALSE;
    int nrem=m_HostIp.Remove(' ');
    int pos=m_HostIp.Find(':');
    int i1=m_HostIp.GetLength();
	BOOL bhave=FALSE;
	memset(m_cCurIp,0,24);
	strcpy(m_cCurIp,m_HostIp.GetBufferSetLength(24));
    struct  HostList *pHostList;
	int i = 0;
	for(i=0;i<m_alllinechar.CurHostNum;i++)
	{
		 if(strcmp(m_alllinechar.Hostchar[i].HostIp,m_HostIp.GetBufferSetLength(24))==0)
		 {
           bhave=TRUE;
		   bhaved=TRUE;
           memcpy(phostchar,(struct HostChar*)&m_alllinechar.Hostchar[i],sizeof(struct HostChar));
		   break;
		 }
	}
	if(!bhave)
	{
       strcpy(m_alllinechar.Hostchar[i].HostIp,m_HostIp.GetBufferSetLength(24));
       m_alllinechar.CurHostNum++; 
	}
	if(pos!=-1)
	{
	    CString str1;
	    str1=m_HostIp.SpanExcluding(":");
		int nIp=inet_addr(str1.GetBuffer(str1.GetLength()));
		if(nIp==INADDR_NONE)
		{
			AfxMessageBox("有不正确的IP地址格式，请重新输入！");
			return;
		}
	CString  str;
	int i2=i1-pos;
	str=m_HostIp.Mid(pos+1,i2);
	m_CurIp=nIp;

	if(!bhave)
	{

     pHostList=new struct  HostList;
	 pHostList->sIpRange.nIpAddr=nIp;    	
     pHostList->sIpRange.nIpaddrNum=atoi(str.GetBuffer(i2));
	}
	}
    else
	{
	   int nIp1=inet_addr(m_HostIp.GetBuffer(i1));
	   if(nIp1==INADDR_NONE)
	   {
			AfxMessageBox("有不正确的IP地址格式，请重新输入！");
			return;
	   }
	   m_CurIp=nIp1;
	   //当第一次配置以后，如果设定的主机已经配置，则直接修改链表

	if(!bhave)
	{
    pHostList=new struct  HostList;
	pHostList->sIpRange.nIpAddr=nIp1; 
	pHostList->sIpRange.nIpaddrNum=1;
	}
	}
	if(!bhave)
     m_ptrlistHostPort.AddHead(pHostList);
}

VOID CDataConvert::SetPortList(CEdit &edit,USHORT type)
{
	char port[20];
	BOOL bFirst=TRUE;
	int  Lines=edit.GetLineCount();
    CString  csPort;
    struct HostList *pHostList;//先取出链表中的对应项；借此添加端口列表到链表的对应项
	//先取出链表中的对应项；
	POSITION pos;
	pos=m_ptrlistHostPort.GetHeadPosition();
	while(pos!=NULL)
	{
          pHostList=(struct HostList*)m_ptrlistHostPort.GetNext(pos);
	      if(pHostList->sIpRange.nIpAddr==m_CurIp)		    
		  break;
	}
	//清空先前配置
    switch(type)
	{
			//begin:拒绝的ICMP类型设定
		case ICMPTYPEIN:
			{
              while(!m_ptrlistIcmp.IsEmpty())
			  {
            	struct IcmpList   *icmprange=(struct IcmpList *)m_ptrlistIcmp.RemoveHead();
	            delete icmprange;
			  }
			} 
			break;
		case ICMPTYPEOUT:
			{
              while(!m_ptrlistIcmpOut.IsEmpty())
			  {
            	struct IcmpList   *icmprange=(struct IcmpList *)m_ptrlistIcmpOut.RemoveHead();
	            delete icmprange;
			  }
			}
			break;
        //end:拒绝的ICMP类型设定
		//begin:允许TCP端口设定
		case TCPSRCOUT:
			{
              while(!pHostList->sTcpPortSet.sAllowOutSrcPortList.IsEmpty())
			  {
            	struct PortList   *pportlist=(struct PortList *)pHostList->sTcpPortSet.sAllowOutSrcPortList.RemoveHead();
	            delete pportlist;
			  }
			}
			break;
		case TCPDSTOUT:
			{
              while(!pHostList->sTcpPortSet.sAllowOutDstPortList.IsEmpty())
			  {
            	struct PortList   *pportlist=(struct PortList *)pHostList->sTcpPortSet.sAllowOutDstPortList.RemoveHead();
	            delete pportlist;
			  }
			}
			break;
			//end:允许的TCP端口设定
		   //begin:允许的udp端口设定
		case UDPSRCOUT:
			{
              while(!pHostList->sTcpPortSet.sAllowOutSrcPortList.IsEmpty())
			  {
            	struct PortList   *pportlist=(struct PortList *)pHostList->sTcpPortSet.sAllowOutSrcPortList.RemoveHead();
	            delete pportlist;
			  }
			}
			break;
		case UDPDSTOUT:
			{
              while(!pHostList->sTcpPortSet.sAllowOutDstPortList.IsEmpty())
			  {
            	struct PortList   *pportlist=(struct PortList *)pHostList->sTcpPortSet.sAllowOutDstPortList.RemoveHead();
	            delete pportlist;
			  }
			}
			break;
			//end:允许的udp端口设定

		default:
			return;
	}
	for(int i=0;i<Lines;i++)
	{
	  csPort.Empty();
    //memset(port,0,20);
	//memset(pIpaddr,0,30);
	  int nret=edit.GetLine(i,port);
	  if(nret==0)continue;//空行
      csPort=(CString)port;
	  csPort.Delete(nret,csPort.GetLength()-nret);//不能超过缓冲区大小
	  int pos=csPort.Find(':');
	  int nrem=csPort.Remove(' ');
	  //m_ptrlistPort有利于内存回收
//      struct HostList *pHostList;//先取出链表中的对应项；借此添加端口列表到链表的对应项
	  if(pos!=-1)
	  {//端口用范围表示法
	    CString str1;
	    str1=csPort.SpanExcluding(":");
		CString  str;
	    int i2=nret-pos;
	    str=csPort.Mid(pos-nrem+1,i2);
	 
        USHORT portbegin=atoi(str1.GetBuffer(pos));	
		USHORT portend=atoi(str.GetBuffer(i2));
/*	    //先取出链表中的对应项；
		POSITION pos;
	    pos=m_ptrlistHostPort.GetHeadPosition();
	    while(pos!=NULL)
		{
          pHostList=(struct HostList*)m_ptrlistHostPort.GetNext(pos);
	      if(pHostList->sIpRange.nIpAddr==m_CurIp)		    
		  break;
		}
		*/
		switch(type)
		{
			//begin:拒绝的ICMP类型设定
		case ICMPTYPEIN:
			{
            struct  IcmpList *picmplist=new struct IcmpList;
		    picmplist->bRange=TRUE;
            picmplist->sIcmpRange.nIcmpBegin=portbegin;
            picmplist->sIcmpRange.nIcmpEnd=portend;
	        m_ptrlistIcmp.AddHead(picmplist);//注意：m_ptrlistIcmp只是为了删除空间
		    pHostList->sInIcmpSet.AddHead(picmplist);
			}
			break;
		case ICMPTYPEOUT:
			{
            struct  IcmpList *picmplist=new struct IcmpList;
		    picmplist->bRange=TRUE;
            picmplist->sIcmpRange.nIcmpBegin=portbegin;
            picmplist->sIcmpRange.nIcmpEnd=portend;
	        m_ptrlistIcmpOut.AddHead(picmplist);//注意：m_ptrlistIcmp只是为了删除空间
		    pHostList->sOutIcmpSet.AddHead(picmplist);
			}
			break;
        //end:拒绝的ICMP类型设定
		//begin:允许TCP端口设定
		case TCPSRCOUT:
			{
            struct  PortList *pportlist=new struct PortList;
			pportlist->bRange=TRUE;
			pportlist->sPortRange.nPortBegin=portbegin;
			pportlist->sPortRange.nPortEnd=portend;
            pHostList->sTcpPortSet.sAllowOutSrcPortList.AddHead(pportlist);
	        m_ptrlistPort.AddHead(pportlist);//注意：m_ptrlistPort只是为了删除空间
			}
			break;
		case TCPDSTOUT:
			{
            struct  PortList *pportlist=new struct PortList;
			pportlist->bRange=TRUE;
			pportlist->sPortRange.nPortBegin=portbegin;
			pportlist->sPortRange.nPortEnd=portend;
            pHostList->sTcpPortSet.sAllowOutDstPortList.AddHead(pportlist);
	        m_ptrlistPort.AddHead(pportlist);//注意：m_ptrlistPort只是为了删除空间
			}
			break;
			//end:允许的TCP端口设定
		   //begin:允许的udp端口设定
		case UDPSRCOUT:
			{
            struct  PortList *pportlist=new struct PortList;
			pportlist->bRange=TRUE;
			pportlist->sPortRange.nPortBegin=portbegin;
			pportlist->sPortRange.nPortEnd=portend;
            pHostList->sUdpPortSet.sAllowOutSrcPortList.AddHead(pportlist);
	        m_ptrlistPort.AddHead(pportlist);//注意：m_ptrlistPort只是为了删除空间
			}
			break;
		case UDPDSTOUT:
			{
            struct  PortList *pportlist=new struct PortList;
			pportlist->bRange=TRUE;
			pportlist->sPortRange.nPortBegin=portbegin;
			pportlist->sPortRange.nPortEnd=portend;
            pHostList->sUdpPortSet.sAllowOutDstPortList.AddHead(pportlist);
	        m_ptrlistPort.AddHead(pportlist);//注意：m_ptrlistPort只是为了删除空间
			}
			break;
			//end:允许的udp端口设定

		default:
			return;
		}
	  }
	  else//具体的端口表示法
	  {	
/*		 POSITION pos;
	     pos=m_ptrlistHostPort.GetHeadPosition();
	     while(pos!=NULL)
		 {
            pHostList=(struct HostList*)m_ptrlistHostPort.GetNext(pos);
	        if(pHostList->sIpRange.nIpAddr==m_CurIp)		    
		    break;
		 }
		 */
		USHORT portbegin=atoi(csPort.GetBufferSetLength(nret-nrem));
		switch(type)
		{
		case ICMPTYPEIN:
            {
             struct IcmpList *picmplist=new struct IcmpList;
		     picmplist->bRange=FALSE;
             picmplist->nIcmpType=portbegin;
	         m_ptrlistIcmp.AddHead(picmplist);
		     pHostList->sInIcmpSet.AddHead(picmplist);
			}
			break;
		case ICMPTYPEOUT:
           {
             struct IcmpList *picmplist=new struct IcmpList;
		     picmplist->bRange=FALSE;
             picmplist->nIcmpType=portbegin;
	         m_ptrlistIcmpOut.AddHead(picmplist);
		     pHostList->sOutIcmpSet.AddHead(picmplist);
			}
		   break;

		case TCPSRCOUT:
 			{ 
             struct PortList *pportlist=new struct PortList;
		     pportlist->bRange=FALSE;
             pportlist->nPort=portbegin;
	         m_ptrlistPort.AddHead(pportlist);
			 pHostList->sTcpPortSet.sAllowOutSrcPortList.AddHead(pportlist);
			}
			break;
		case TCPDSTOUT:
 			{ 
             struct PortList *pportlist=new struct PortList;
		     pportlist->bRange=FALSE;
             pportlist->nPort=portbegin;
	         m_ptrlistPort.AddHead(pportlist);
			 pHostList->sTcpPortSet.sAllowOutDstPortList.AddHead(pportlist);
			}
			break;
		case UDPSRCOUT:
 			{ 
             struct PortList *pportlist=new struct PortList;
		     pportlist->bRange=FALSE;
             pportlist->nPort=portbegin;
	         m_ptrlistPort.AddHead(pportlist);
			 pHostList->sUdpPortSet.sAllowOutSrcPortList.AddHead(pportlist);
			}
			break;
		case UDPDSTOUT:
			{ 
             struct PortList *pportlist=new struct PortList;
		     pportlist->bRange=FALSE;
             pportlist->nPort=portbegin;
	         m_ptrlistPort.AddHead(pportlist);
			 pHostList->sUdpPortSet.sAllowOutDstPortList.AddHead(pportlist);
			}
			break;
		default:
            return;
		}
	  } 
	  bFirst=FALSE;
   }
}
//填充NAT配置信息,注意：分配的空间尚未释放
VOID CDataConvert::SetNatCfg(struct  NAT_Cfg *natcfg)
{
   natcfg->nGlobalAddress=m_firewallconfig.nOutsideIp;
   natcfg->us_HighPort=m_alllinechar.FireWallCfg.sNatCfg.nHighPort;
   natcfg->us_LowPort=m_alllinechar.FireWallCfg.sNatCfg.nLowPort;
   natcfg->us_MaxNatNumber=m_alllinechar.FireWallCfg.sNatCfg.nMaxNatNumber;
   natcfg->nTimeOut_TCP=m_alllinechar.FireWallCfg.sNatCfg.nTimeOut_TCP;
   natcfg->nTimeOut_UDP=m_alllinechar.FireWallCfg.sNatCfg.nTimeOut_UDP;
   //静态端口NAT映射
   int tcpnum=m_alllinechar.FireWallCfg.sNatCfg.us_PortNumberTcp;
   int udpnum=m_alllinechar.FireWallCfg.sNatCfg.us_PortNumberUdp;
   int itemnum=0;
   natcfg->StaticTable.count=tcpnum+udpnum;
   natcfg->StaticTable.item=new struct  STATICTABLEITEM[natcfg->StaticTable.count];
   for(int i=0;i<tcpnum;i++)
   {
     natcfg->StaticTable.item[itemnum].type=1;//tcp
     ConvertEditChar(m_alllinechar.FireWallCfg.sNatCfg.MapStaticTcp[i],natcfg->StaticTable.item[itemnum].ulIpadress, natcfg->StaticTable.item[itemnum].usPort,TRUE);
	 itemnum++;
   }
   for(int j=0;j<udpnum;j++)
   {
     natcfg->StaticTable.item[itemnum].type=2;//udp
     ConvertEditChar(m_alllinechar.FireWallCfg.sNatCfg.MapStaticUdp[j],natcfg->StaticTable.item[itemnum].ulIpadress, natcfg->StaticTable.item[itemnum].usPort,TRUE);
	 itemnum++;
   }

}

VOID CDataConvert::InitFireWall()
{	
	//begin初始化全局标志
    globalFlags=m_alllinechar.sGlobalFlags;

	//end初始化全局标志
	m_firewallconfig.nIntraIp=inet_addr(m_alllinechar.FireWallCfg.csIntraIp);
    m_firewallconfig.nIntraMask=inet_addr(m_alllinechar.FireWallCfg.csIntraMask);

    m_firewallconfig.nMiddleIp=inet_addr(m_alllinechar.FireWallCfg.csMiddleIp);
    m_firewallconfig.nMiddleMask=inet_addr(m_alllinechar.FireWallCfg.csMiddleMask);

    m_firewallconfig.nOutsideIp=inet_addr(m_alllinechar.FireWallCfg.csOutsideIp);
    m_firewallconfig.nOutsideMask=inet_addr(m_alllinechar.FireWallCfg.csOutsideMask);
//    memcpy(&m_firewallconfig.sFilterdrvCfg,&m_alllinechar.FireWallCfg.sFilterdrvCfg,sizeof(struct  Filterdrv_Cfg));

}

VOID CDataConvert::ConvertEditChar(char *pchar,ULONG &mylong,USHORT &mynum,BOOL bIp)
{
    CString  csPort;
    csPort=(CString)pchar;
	int nrem=csPort.Remove(' ');
	int pos=csPort.Find(':');
	CString str1;
	if(pos!=-1)
	{//端口用范围表示法
	    str1=csPort.SpanExcluding(":");
		CString  str;
	    int i2=csPort.GetLength()-pos+1;
	    str=csPort.Mid(pos+1,i2);

	    if(bIp)//是IP
		{
         mylong=inet_addr(str1.GetBuffer(str1.GetLength()));
		 mynum=atoi(str.GetBuffer(i2));
		}
		else//是端口
		{
         mylong=atoi(str1.GetBuffer(str1.GetLength()));	
		 mynum=atoi(str.GetBuffer(i2));
		}
	}
	else
	{
	    if(bIp)//是IP
		{
         mylong=inet_addr(pchar);
		 mynum=1;
		}
		else//是端口
		{
        mylong=atoi(pchar);	
		mynum=1;
		}
		/*		char tempchar[30];
		unsigned char*uchar=(unsigned char*)pchar;
		int len=strlen((const char *)pchar);
		int i=0;
		for(i;i<30;i++)
		{
			if(uchar[i]!=0xCC)
			{
				tempchar[i]=uchar[i];
			}
			else
				break;
		}
		tempchar[i]=0x0;

	    if(bIp)//是IP
		{
         mylong=inet_addr(tempchar);
		 mynum=1;
		}
		else//是端口
		{
        mylong=atoi(tempchar);	
		mynum=1;
		}
	*/
	}
}

VOID CDataConvert::DisplayHost(struct HostChar *phostchar,USHORT type,CEdit &edit)
{
	int len=phostchar->nPos[type];
	CString *str=new CString[len];
	CString tstr;
	for(int i=0;i<len;i++)
	{
		switch(type)
		{
	      case TCPSRCOUT:  
		    str[i]=(CString)phostchar->TcpSrcOut[i];
            break;
	      case TCPDSTOUT:  
		    str[i]=(CString)phostchar->TcpDstOut[i];
            break;
	      case UDPSRCOUT: 
		    str[i]=(CString)phostchar->UdpSrcOut[i];
            break;
	      case UDPDSTOUT:
		    str[i]=(CString)phostchar->UdpDstOut[i];
            break;
	      case ICMPTYPEIN:  
		    str[i]=(CString)phostchar->IcmpTypeIn[i];
            break;
	      case ICMPTYPEOUT: 
		    str[i]=(CString)phostchar->IcmpTypeOut[i];
            break;
		  default:
			  {
			    delete []str;
			    return;
			  }
		} 
		str[i]+="\r\n";
		tstr+=str[i];

	}
	edit.SetWindowText(tstr.GetBuffer(tstr.GetLength()));
	delete []str;
}
/*
VOID CDataConvert::ConvertToStr(char *pchar,CString &str,int nret,int &strlen,CEdit *edit)
{
		 str=(CString)tempchar;
		 int strlen=str.GetLength();
	     str.Delete(nret,strlen-nret);//不能超过缓冲区大小
}
*/
VOID CDataConvert::DisplayFireCfg(CEdit &edit,USHORT type)
{
	int num;
	switch(type)
	{
	case ALLOWIPTAB:
		num=m_alllinechar.AllowPos;
		break;
	case REJIPTAB:
		num=m_alllinechar.RejPos;
		break;
	case MAPSTATIC_UDP:
		num=m_alllinechar.FireWallCfg.sNatCfg.us_PortNumberUdp;
		break;
	case MAPSTATIC_TCP:
		num=m_alllinechar.FireWallCfg.sNatCfg.us_PortNumberTcp;
		break;
	 case INDM:
		num=m_alllinechar.InDmPos;
		break;
	 case OUTDM:
		num=m_alllinechar.OutDmPos;
		break;
	}
	if(num>=0)
	{
	CString *str=new CString[num];
	CString tstr;

	for(int i=0;i<num;i++)
	{ 
	  switch(type)
	  {
	    case ALLOWIPTAB:
		   str[i]=(CString)m_alllinechar.AllowIpTab[i];
		break;
	    case REJIPTAB:
		   str[i]=(CString)m_alllinechar.RejIpTab[i];
		   break;
		case MAPSTATIC_UDP:
		   str[i]=(CString)m_alllinechar.FireWallCfg.sNatCfg.MapStaticUdp[i];
           break;
		case MAPSTATIC_TCP:
		   str[i]=(CString)m_alllinechar.FireWallCfg.sNatCfg.MapStaticTcp[i];
           break;
	 case INDM:
		   str[i]=(CString)m_alllinechar.InDmIpTab[i];
		break;
	 case OUTDM:
		   str[i]=(CString)m_alllinechar.OutDmIpTab[i];
		break;
	  }
		str[i]+="\r\n";
		tstr+=str[i];
	    edit.SetWindowText(tstr.GetBuffer(tstr.GetLength()));
	}
     delete []str;
	}
}
VOID CDataConvert::InitPortList(USHORT type,int &ii,struct HostList *pHostList)
{
	int  Lines;
	switch(type)
	{
	case TCPSRCOUT:
		  Lines=m_alllinechar.Hostchar[ii].nPos[TCPSRCOUT];
		break;		
	case TCPDSTOUT:  
		  Lines=m_alllinechar.Hostchar[ii].nPos[TCPDSTOUT];
		break;
	case UDPSRCOUT: 
		  Lines=m_alllinechar.Hostchar[ii].nPos[UDPSRCOUT];
		break;
	case UDPDSTOUT: 
		  Lines=m_alllinechar.Hostchar[ii].nPos[UDPDSTOUT];
		break;
	case ICMPTYPEIN:  
		  Lines=m_alllinechar.Hostchar[ii].nPos[ICMPTYPEIN];
		break;
	case ICMPTYPEOUT:
		  Lines=m_alllinechar.Hostchar[ii].nPos[ICMPTYPEOUT];
		break;
	default:
		return;
	}
	ULONG  portbegin;
	USHORT  portend;
	for(int i=0;i<Lines;i++)// jl1222
	{
	switch(type)
	{
	case TCPSRCOUT:
        ConvertEditChar(m_alllinechar.Hostchar[ii].TcpSrcOut[i],portbegin,portend,FALSE);
		break;		
	case TCPDSTOUT:  
        ConvertEditChar(m_alllinechar.Hostchar[ii].TcpDstOut[i],portbegin,portend,FALSE);
		break;
	case UDPSRCOUT: 
        ConvertEditChar(m_alllinechar.Hostchar[ii].UdpSrcOut[i],portbegin,portend,FALSE);
		break;
	case UDPDSTOUT: 
        ConvertEditChar(m_alllinechar.Hostchar[ii].UdpDstOut[i],portbegin,portend,FALSE);
		break;
	case ICMPTYPEIN:  
        ConvertEditChar(m_alllinechar.Hostchar[ii].IcmpTypeIn[i],portbegin,portend,FALSE);
		break;
	case ICMPTYPEOUT:
        ConvertEditChar(m_alllinechar.Hostchar[ii].IcmpTypeOut[i],portbegin,portend,FALSE);
		break;
	default:
		return;
	}
	if(portend>1)//范围表示法
	{
    	switch(type)
		{
			//begin:拒绝的ICMP类型设定
		case ICMPTYPEIN:
			{
            struct  IcmpList *picmplist=new struct IcmpList;
		    picmplist->bRange=TRUE;
            picmplist->sIcmpRange.nIcmpBegin=portbegin;
            picmplist->sIcmpRange.nIcmpEnd=portend;
	        m_ptrlistIcmp.AddHead(picmplist);//注意：m_ptrlistIcmp只是为了删除空间
		    pHostList->sInIcmpSet.AddHead(picmplist);
			}
			break;
		case ICMPTYPEOUT:
			{
            struct  IcmpList *picmplist=new struct IcmpList;
		    picmplist->bRange=TRUE;
            picmplist->sIcmpRange.nIcmpBegin=portbegin;
            picmplist->sIcmpRange.nIcmpEnd=portend;
	        m_ptrlistIcmpOut.AddHead(picmplist);//注意：m_ptrlistIcmp只是为了删除空间
		    pHostList->sOutIcmpSet.AddHead(picmplist);
			}
			break;
        //end:拒绝的ICMP类型设定
		//begin:允许TCP端口设定
		case TCPSRCOUT:
			{
            struct  PortList *pportlist=new struct PortList;
			pportlist->bRange=TRUE;
			pportlist->sPortRange.nPortBegin=portbegin;
			pportlist->sPortRange.nPortEnd=portend;
            pHostList->sTcpPortSet.sAllowOutSrcPortList.AddHead(pportlist);
	        m_ptrlistPort.AddHead(pportlist);//注意：m_ptrlistPort只是为了删除空间
			}
			break;
		case TCPDSTOUT:
			{
            struct  PortList *pportlist=new struct PortList;
			pportlist->bRange=TRUE;
			pportlist->sPortRange.nPortBegin=portbegin;
			pportlist->sPortRange.nPortEnd=portend;
            pHostList->sTcpPortSet.sAllowOutDstPortList.AddHead(pportlist);
	        m_ptrlistPort.AddHead(pportlist);//注意：m_ptrlistPort只是为了删除空间
			}
			break;
			//end:允许的TCP端口设定
		   //begin:允许的udp端口设定
		case UDPSRCOUT:
			{
            struct  PortList *pportlist=new struct PortList;
			pportlist->bRange=TRUE;
			pportlist->sPortRange.nPortBegin=portbegin;
			pportlist->sPortRange.nPortEnd=portend;
            pHostList->sUdpPortSet.sAllowOutSrcPortList.AddHead(pportlist);
	        m_ptrlistPort.AddHead(pportlist);//注意：m_ptrlistPort只是为了删除空间
			}
			break;
		case UDPDSTOUT:
			{
            struct  PortList *pportlist=new struct PortList;
			pportlist->bRange=TRUE;
			pportlist->sPortRange.nPortBegin=portbegin;
			pportlist->sPortRange.nPortEnd=portend;
            pHostList->sUdpPortSet.sAllowOutDstPortList.AddHead(pportlist);
	        m_ptrlistPort.AddHead(pportlist);//注意：m_ptrlistPort只是为了删除空间
			}
			break;
			//end:允许的udp端口设定

		default:
			return;
		 }
	 }	  
	 else//具体的端口表示法
	 {	
		switch(type)
		{
		case ICMPTYPEIN:
            {
             struct IcmpList *picmplist=new struct IcmpList;
		     picmplist->bRange=FALSE;
             picmplist->nIcmpType=portbegin;
	         m_ptrlistIcmp.AddHead(picmplist);
		     pHostList->sInIcmpSet.AddHead(picmplist);
			}
			break;
		case ICMPTYPEOUT:
           {
             struct IcmpList *picmplist=new struct IcmpList;
		     picmplist->bRange=FALSE;
             picmplist->nIcmpType=portbegin;
	         m_ptrlistIcmpOut.AddHead(picmplist);
		     pHostList->sOutIcmpSet.AddHead(picmplist);
			}
		   break;

		case TCPSRCOUT:
 			{ 
             struct PortList *pportlist=new struct PortList;
		     pportlist->bRange=FALSE;
             pportlist->nPort=portbegin;
	         m_ptrlistPort.AddHead(pportlist);
			 pHostList->sTcpPortSet.sAllowOutSrcPortList.AddHead(pportlist);
			}
			break;
		case TCPDSTOUT:
 			{ 
             struct PortList *pportlist=new struct PortList;
		     pportlist->bRange=FALSE;
             pportlist->nPort=portbegin;
	         m_ptrlistPort.AddHead(pportlist);
			 pHostList->sTcpPortSet.sAllowOutDstPortList.AddHead(pportlist);
			}
			break;
		case UDPSRCOUT:
 			{ 
             struct PortList *pportlist=new struct PortList;
		     pportlist->bRange=FALSE;
             pportlist->nPort=portbegin;
	         m_ptrlistPort.AddHead(pportlist);
			 pHostList->sUdpPortSet.sAllowOutSrcPortList.AddHead(pportlist);
			}
			break;
		case UDPDSTOUT:
			{ 
             struct PortList *pportlist=new struct PortList;
		     pportlist->bRange=FALSE;
             pportlist->nPort=portbegin;
	         m_ptrlistPort.AddHead(pportlist);
			 pHostList->sUdpPortSet.sAllowOutDstPortList.AddHead(pportlist);
			}
			break;
		default:
            return;
		}
	  } 
   }
}

VOID CDataConvert::InitHostList()
{
 for(int ii=0;ii<m_alllinechar.CurHostNum;ii++)// jl1222
 {
	struct HostList *phostlist=new struct HostList ;
	for(int i=0;i<6;i++)//i：对应于宏定义
	{
         InitPortList(i,ii,phostlist);
	}
    ULONG hostip;
	USHORT ipnum; 
    ConvertEditChar(m_alllinechar.Hostchar[ii].HostIp,hostip,ipnum,TRUE);
	phostlist->sIpRange.nIpAddr=hostip;  
    phostlist->sIpRange.nIpaddrNum=ipnum;
    m_ptrlistHostPort.AddHead(phostlist);  	
 }
}
