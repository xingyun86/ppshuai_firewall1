
#include "stdafx.h"
#include "outnat.h"
#include "Struct.h"
extern struct  NAT_Cfg pnatcfg;
extern in_addr_t OutGlobalAddr;

extern  BYTE INNO;
extern  BYTE OUTNO;
extern  BYTE DMNO;

#define   ETH_HEAD    14  //以太帧头大小
#define   IP_HEAD     20  //
//#define   DYN_LOW_PORT    5000    //用于转换的最低端口
unsigned short   DYN_LOW_PORT ;  //用于转换的最低端口

//#define   DYN_HIGH_PORT    10000  //用于转换的最高端口
unsigned short   DYN_HIGH_PORT;     //用于转换的最高端口

//#define   MAX_NAT_CONNECT  500    //可支持的最大TCP连接数
unsigned short   MAX_NAT_CONNECT;  //可支持的最大TCP连接数
//#define   MAX_NAT_CONNECTUDP  100 //可支持的最大UDP连接数
//#define   ITEM_LENG        25     
//#define   OTHERITEM_LENG   25
//#define   TCPTIMEOUT       5       //每个TCP转换表项的超时值
unsigned short   TCPTIMEOUT;       //每个TCP转换表项的超时值

//#define   UDPTIMEOUT       5       //每个UDP转换表项的超时值
//#define   MAX_STATICITEM   6

NATTABLE_ITEM*     nattable;          //转换表  
HANDLE             access_event;      //访问控制
BYTE               StaticTCPcount;    //设置的TCP静态转换个数
BYTE               StaticUDPcount;     //设置的UDP静态转换个数
STATICTCPTABLE     StaticTCPtable[100] ;  //静态转换表
STATICUDPTABLE     StaticUDPtable[100] ;  //静态转换表

ULONG        uc_GlobalIPAddress;       //对外的全局地址
//ULONG        ul_InterIPAddress;

NATTABLE_ITEM*   otheritem;           //转换表项
USHORT*  pChooseportBufferheader;     //可选择端口栈的头指针
USHORT*  pChooseportBufferbottom;     //可选择端口栈的尾指针
USHORT         InHashIDTable[1000];   //转换表的入站索引
USHORT         OutHashIDTable[1000];  //转换表的出站索引

//input eth and calculate checksum
BOOL GetTCPCksum(BYTE* buffer,int leng)
{
    //BYTE* tempbuffer;
	USHORT  tcpleng;
	USHORT  tcpchecksum;
	USHORT*  tempushort;

	USHORT  IPleng;
	BYTE*   tempbuffer;
	
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
		delete [] tempbuffer;
		return TRUE;
}

//input eth and calculate checksum
BOOL GetUDPCksum(BYTE* buffer,int leng)
{
	USHORT  udpleng;
	USHORT  udpchecksum;
	USHORT*  tempushort;

	BYTE*   tempbuffer;
	//set checksum=0;
	buffer[34+6] = 0;
	buffer[34+7] = 0;
    //getudpleng(total)
    ((BYTE*)(&udpleng))[0] = buffer[34+5];
    ((BYTE*)(&udpleng))[1] = buffer[34+4];
    //get pseudo head
		tempbuffer = new BYTE[udpleng+12];
		memcpy(tempbuffer+12,buffer+34,udpleng);
		tempbuffer[0] = buffer[26];
		tempbuffer[1] = buffer[27];
		tempbuffer[2] = buffer[28];
		tempbuffer[3] = buffer[29];
		tempbuffer[4] = buffer[30];
		tempbuffer[5] = buffer[31];
		tempbuffer[6] = buffer[32];
		tempbuffer[7] = buffer[33];
        tempbuffer[8] = 0;
		tempbuffer[9] = 17;
		tempbuffer[10] = buffer[34+4];
		tempbuffer[11] = buffer[34+5];
        
		tempushort = (USHORT*) tempbuffer;
		udpchecksum = GetCksum(tempushort, udpleng+12);
		buffer[34+6] = ((BYTE*)(&udpchecksum))[0];
		buffer[34+7] = ((BYTE*)(&udpchecksum))[1];
		delete [] tempbuffer;
		return TRUE;
}

//input eth and calculate checksum
BOOL GetIPCksum(BYTE* buffer,int leng)
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

//input buffer  and calculate checksum
USHORT  GetCksum(USHORT* buffer,int size)
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


/*for test*/
BOOL NatForOutIcmpPacket(BYTE* EthBuffer,ULONG packetlength,USHORT Adapternumber)
{
    BYTE* tempbuffer;
	BYTE* tempbufferttt;
	USHORT*  tempushortbuffer;
	USHORT   checksum;
    int   EheadLength;

	EheadLength=14;
    tempbuffer=EthBuffer;
	if(tempbuffer[12]==0x08&&tempbuffer[13]==0x00)
	{
		if(tempbuffer[23]!=0x01)
		{
			return FALSE;
		}
		//begin modify
		tempbuffer[26] = 0xc0;
        tempbuffer[27] = 0xa8;
		tempbuffer[28] = 0x81;
		tempbuffer[29] = 0xc1;
		//modify ip checksum
        tempbufferttt = &tempbuffer[14];
		tempushortbuffer = (USHORT* )tempbufferttt;
		tempushortbuffer[5] = 0;
        checksum = GetCksum(tempushortbuffer,20);
		tempushortbuffer[5] = checksum;
	}
	else
	{
		TRACE(_T("Found a not IP packet\n"));
	}
	return TRUE;
}

/*for test*/
BOOL NatForInIcmpPacket(BYTE* EthBuffer,ULONG packetlength,USHORT Adapternumber)
{
	
    BYTE* tempbuffer;
	USHORT*  tempushortbuffer;
	USHORT   checksum;
    int   EheadLength;

	EheadLength=14;
    tempbuffer=EthBuffer;
	if(tempbuffer[12]==0x08&&tempbuffer[13]==0x00)
	{
		if(tempbuffer[23]!=0x01)
		{
			TRACE(_T("It's  not a IPandICMP packet\n"));
			return FALSE;
		}
		tempbuffer[30] = 0xc0;
        tempbuffer[31] = 0xa8;
		tempbuffer[32] = 0x0;
		tempbuffer[33] = 0x0b;
		//modify ip checksum
        tempbuffer = &tempbuffer[14];
		tempushortbuffer = (USHORT* )tempbuffer;
		tempushortbuffer[5] = 0;
        checksum  = GetCksum(tempushortbuffer,20);
		tempushortbuffer[5] = checksum;
	}
	else
	{
		TRACE(_T("Found a not IP packet\n"));
	}
	return TRUE;
}

//add item to nattable
NATTABLE_ITEM*  NatTableAddItem(BYTE* EthBuffer,BOOL bDirection, USHORT type)
{

	   _ETHER_PACKET*   pEtherPacket;
	   _IP_HEADER*      pIPheader;
	   _TCP_HEADER*     pTCPheader;
	   _UDP_HEADER*     pUDPheader;
       NATTABLE_ITEM*   pResult;
	   NATTABLE_ITEM*   ptempResult;	 
	   USHORT           uSearchInterPort;
	   USHORT           uSearchOutPort;
	   USHORT             pos;
	   USHORT             index;
	   USHORT           tempSuffix;

       pEtherPacket = (_ETHER_PACKET*)EthBuffer;
	   pIPheader    = (_IP_HEADER*)(EthBuffer+14);
	   //pTCPheader   = (_TCP_HEADER*)(EthBuffer+34);      
       
	   switch (type) {
	   case 1: //it a tcp
		       TRACE(_T("ADD TCP ITEM\n"));
		       pTCPheader   = (_TCP_HEADER*)(EthBuffer+34);
               uSearchInterPort = pTCPheader->source;	   
	   //modify InHashIDTable;
	   //modify OutHashIDTable;
	   //modify nattable
       //get out port	   
	   if(pChooseportBufferheader == pChooseportBufferbottom)
		   return 0;     
       
	   
	   uSearchOutPort = *pChooseportBufferheader;	   
	   pChooseportBufferheader--;
	   

       //modify nattable
	   //USHORT  iii = uSearchOutPort - DYN_LOW_PORT;
	   
	   pResult= &nattable[swaps(uSearchOutPort) - DYN_LOW_PORT];
       memset(pResult,0,sizeof(NATTABLE_ITEM));
	   pResult->ul_GlobalIPAddress = uc_GlobalIPAddress;
	   pResult ->ul_InterIPAddress = pIPheader->saddr;
	   pResult ->ul_OutIPAddress   = pIPheader ->daddr;
	   pResult ->us_InterPort      = uSearchInterPort;
	   pResult ->us_OutPort        = uSearchOutPort;
	   pResult ->us_TimeOut        = TCPTIMEOUT;   
       pResult-> bUse              = 1;

       //get for InhashIDTable
	   pos = uSearchOutPort% 997 +1;
	   tempSuffix = InHashIDTable[pos];

	   // modify InHashIDTable
	    if(InHashIDTable[pos] == 0)
		    InHashIDTable[pos] = swaps(uSearchOutPort) - DYN_LOW_PORT;
		else
		 {
			ptempResult = &nattable[(InHashIDTable[pos])];
			tempSuffix = InHashIDTable[pos];
			while(1) 
			{
             if(ptempResult->uInSuffixNext !=0)
			 {
				 tempSuffix  = ptempResult->uInSuffixNext;
				 ptempResult = &nattable[ptempResult->uInSuffixNext];		         		 
				 continue;
			 }
			 else
			 {
				 ptempResult->uInSuffixNext = swaps(uSearchOutPort) - DYN_LOW_PORT;
                 pResult->uInSuffixParent   = tempSuffix;
				 break;
			 }
			}
		 }
   
	   //modify uOutHashIDTable
	   //get outHashID
       index = NATGetIndexOther(pIPheader->saddr,pIPheader->daddr,pTCPheader->source);
	      
	   if(OutHashIDTable[index] == 0)
		    OutHashIDTable[index] = swaps(uSearchOutPort) - DYN_LOW_PORT;
		else
		 {
			ptempResult = &nattable[(OutHashIDTable[index])];
			tempSuffix = OutHashIDTable[index];
			while(1) 
			{
             if(ptempResult->uOutSuffixNext !=0)
			 {
				 tempSuffix  = ptempResult->uOutSuffixNext;
				 ptempResult = &nattable[ptempResult->uOutSuffixNext];		         		 
				 continue;
			 }
			 else
			 {
                 ptempResult->uOutSuffixNext = swaps(uSearchOutPort) - DYN_LOW_PORT;
                 pResult->uOutSuffixParent = tempSuffix;
				 break;
			 }
			}
		 }
   	      
	   		    break;
	   case 2: //it a udp
			   break;
	   default:
		   return 0;
	   }
	   
	   return pResult;           
}

NATTABLE_ITEM* NatSearch(BYTE* EthBuffer,BOOL bDirection, USHORT type)
{
	   _ETHER_PACKET*   pEtherPacket;
	   _IP_HEADER*      pIPheader;
	   _TCP_HEADER*     pTCPheader;
	   _UDP_HEADER*     pUDPheader;
       NATTABLE_ITEM*   pResult;
	   USHORT           pos;
       USHORT           otherindex;
       NATTABLE_ITEM*  pOtherResult;

       pEtherPacket = (_ETHER_PACKET*)EthBuffer;
	   pIPheader    = (_IP_HEADER*)(EthBuffer+14);
	   
	   switch (type) {
	   case 1: //it is a tcp
		       WaitForSingleObject(access_event,INFINITE);
	           //ResetEvent(access_event);
//			   TRACE(_T("begin search item \n"));
		       pTCPheader   = (_TCP_HEADER*)(EthBuffer+34);
               if(bDirection) //out is true, in is false
			   {
				   //search for out packet
				   //get index in othertable
//				   TRACE(_T("OUT SEARCH \n"));

				   otherindex = NATGetIndexOther(pIPheader->saddr,pIPheader->daddr,pTCPheader->source);

				   if(OutHashIDTable[otherindex]<1)
				       	   return 0;
                    pOtherResult = &nattable[OutHashIDTable[otherindex]];
                   while(1)
				   {
		  
		              if(pOtherResult->us_TimeOut != 0 && pOtherResult->ul_InterIPAddress == pIPheader->saddr&&
						 pTCPheader->source == pOtherResult->us_InterPort)
					  {
						   pOtherResult->us_TimeOut = TCPTIMEOUT;
						   //SetEvent(access_event);
			               return pOtherResult;
					  }

		              if(pOtherResult->uOutSuffixNext != 0 )
					  {
		   	               pOtherResult = &nattable[pOtherResult->uOutSuffixNext];
						   TRACE(_T("found  next for out packet\n")); 
			                    continue;
					  }
		              else
					  {
			               pOtherResult = 0;
		                    break;
					  }
		  
				   }
				       //SetEvent(access_event);
	                   return  pOtherResult;
			   }
			   else
			   {
				   //search for in packet
//				   TRACE(_T("IN SEARCH \n"));
				   pos = pTCPheader->dest % 997+1;
				   if(InHashIDTable[pos]>0)
				      pResult = &nattable[(InHashIDTable[pos])] ;  
                   else
					   return 0;

                   while(1)
				   {
		  
		              if(pResult->us_TimeOut != 0 && pResult->us_OutPort == pTCPheader->dest )
					  {
						  pResult->us_TimeOut = TCPTIMEOUT;
						  //SetEvent(access_event);
			                   return pResult;
					  }

		              if(pResult->uInSuffixNext != 0)
					  {
		   	               pResult = &nattable[pResult->uInSuffixNext];
						   TRACE(_T("found  next for in packet\n")); 
			                    continue;
					  }
		              else
					  {
			               pResult = 0;
						   //TRACE(_T(""));
		                   break;
					  }
		  
				   }
				   //SetEvent(access_event);
	                   return  pResult;
			   }	
			   break;
	   case 2: //it a udp
		       break;
	   default:
		   return 0;
	   }    
			   return 0;
}

//nat packet include in and out
BOOL NatPacket(BYTE* total_EthBuffer)
{
	_ETHER_PACKET*   pEtherPacket;
	_IP_HEADER*      pIPheader;
	_TCP_HEADER*     pTCPheader;
	_UDP_HEADER*     pUDPheader;
    NATTABLE_ITEM*   pResult;
    BYTE*  EthBuffer;
    int Direction;
    EthBuffer = total_EthBuffer+2;
	pResult = 0;
    pEtherPacket = (_ETHER_PACKET*)EthBuffer;
	pIPheader    = (_IP_HEADER*)(EthBuffer+14);
	pTCPheader   = (_TCP_HEADER*)(EthBuffer+34);
	pUDPheader   = (_UDP_HEADER*)(EthBuffer+34);

    //if the packet is not a ip
	if(pEtherPacket->ETH.h_proto != 0x0008)
		{
		return FALSE;
		}
   	
	//the packet is from devinstance = 3 ?
	//need modify
 	if(total_EthBuffer[1]!=(OUTNO-0x30))
		return FALSE;

    //set direction by out or in
	if(total_EthBuffer[0] == 1)
        Direction = 1;
	else
		Direction = 0;

	if(Direction)//out is true, in is false
	{
	switch  (pIPheader->protocol) {
	case 6:
		//out
		//the packet by this computer not nat
		if(pIPheader ->saddr == uc_GlobalIPAddress)
			    return TRUE;
        
		//first find in statictable
        if(!NATSearchStatic(EthBuffer,1,1))
		{
		pResult = NatSearch(EthBuffer,1,1);
//		TRACE(_T("search end\n"));
		//Note: no judge the result of additem
		if(!pResult)
		{
			WaitForSingleObject(access_event,INFINITE);
	        ResetEvent(access_event);
			pResult = NatTableAddItem(EthBuffer,1,1);
			SetEvent(access_event);
		}
        if(!pResult)
		{
			//if go to here ,something is wrong
			TRACE(_T("addite erro\n"));
			return FALSE;
		}
		//modify src port
		pTCPheader ->source = pResult->us_OutPort;
		}
        //modify ipaddress
		pIPheader->saddr = uc_GlobalIPAddress;
		//TRACE(_T("sport =%x", pTCPheader ->source));
		//modify ip checksum
		GetIPCksum(EthBuffer,1000); 
		//modify tcp checksum
	    GetTCPCksum(EthBuffer,1000);                                                                                                                                                                                                                                                                                                                                                                                   	GetTCPCksum(EthBuffer,1000);
		break;
	case 17:
		//Note: no add for udp packet
		//the packet by this computer not nat
		if(pIPheader ->saddr == uc_GlobalIPAddress)
			    return TRUE;
		
		pIPheader->saddr = uc_GlobalIPAddress;
		//TRACE(_T("sport =%x", pTCPheader ->source));
		//modify ip checksum
		GetIPCksum(EthBuffer,1000);
        //modify tcp checksum
	    GetUDPCksum(EthBuffer,1000);		
		break;
	default:
		return FALSE;
	}
	}
	else
	{
	switch (pIPheader->protocol){
	case 6:	
		//in
		//TRACE(_T("source port = %d \n"),swaps(pTCPheader->source));
		//first find in statictable
		if(!NATSearchStatic(EthBuffer,0,1))
		{
		pResult = NatSearch(EthBuffer,0,1);
        if(pResult == 0)
		{
			TRACE(_T("IN PACKET NOT FOUND ITEM \n"));
			return FALSE;			
		}
		//modify dst ipaddress "192.168.0.11"
		//modify dst port
		pIPheader->daddr = pResult->ul_InterIPAddress;
		pTCPheader ->dest = pResult->us_InterPort;		
		}
		//modify ip checksum
//		TRACE(_T("IN PACKET begin nat \n"));
		GetIPCksum(EthBuffer,1000);
        //modify tcp checksum
		GetTCPCksum(EthBuffer,1000);
		break;
	 case 17:
		
        //if in staticudptable, modify dst ipaddress
		 //first find in statictable
		if(NATSearchStatic(EthBuffer,0,2))
		{
		    GetIPCksum(EthBuffer,1000);
            GetUDPCksum(EthBuffer,1000);
		}
		break;
	default:
		return FALSE;
	}
	}
	return TRUE;
}

void NatCfg()
{
   DYN_LOW_PORT=pnatcfg.us_LowPort;  

   DYN_HIGH_PORT=pnatcfg.us_HighPort;

   MAX_NAT_CONNECT=pnatcfg.us_MaxNatNumber;
   TCPTIMEOUT=pnatcfg.nTimeOut_TCP; 
	 //Set up nattable and inhashIDtable,outhashIDtable ,three table
	 //BYTE* temp = new BYTE[1024*ITEM_LENG];
	 //memset(temp,0,1024*ITEM_LENG);

	 //tempaddd for test
	//need modify
	 STATICTABLE      statictable;
//	 statictable.count = 3;
     statictable.count = pnatcfg.StaticTable.count;
//	 STATICTABLEITEM*  statictableitem = new STATICTABLEITEM[pnatcfg->StaticTable.count];
     
//     statictable.item = statictableitem;
	 statictable.item=pnatcfg.StaticTable.item;
/*
	 statictableitem[0].type = 1;
	 statictableitem[0].ulIpadress = 0x0b00a8c0;
	 statictableitem[0].usPort =  swaps(20);//16415;
	 statictableitem[1].type = 2;
	 statictableitem[1].ulIpadress = 0x0b00a8c0;
	 statictableitem[1].usPort =  0xba08;
	 statictableitem[2].type = 1;
	 statictableitem[2].ulIpadress = 0x0b00a8c0;
	 statictableitem[2].usPort =  300;
*/
	 //for test end

 	 StaticTCPcount = 0;
	 StaticUDPcount = 0;

	 STATICTABLEITEM*  temp;
	 temp = statictable.item;
     
	 //set static data from statictable
	 for(int j=0;j<statictable.count;j++) 
	 {
		 switch  (temp->type){
		 case 1:
			 //tcp
			 StaticTCPtable[StaticTCPcount].ulIpadress = temp->ulIpadress;
			 StaticTCPtable[StaticTCPcount].usPort     = temp->usPort;
			 StaticTCPcount++;
			 break;
		 case 2:
			 //udp
			 StaticUDPtable[StaticUDPcount].ulIpadress = temp->ulIpadress;
			 StaticUDPtable[StaticUDPcount].usPort     = temp->usPort;
			 StaticUDPcount++;
			 break;
		 default:
			 break;
		 }
		 temp++;
	 }

//     delete[] statictableitem;
	 nattable = new NATTABLE_ITEM[MAX_NAT_CONNECT];
	 memset(nattable,0,sizeof(NATTABLE_ITEM)*MAX_NAT_CONNECT);
     
	 memset(InHashIDTable,0,1000);
	 memset(OutHashIDTable,0,1000);

     USHORT    lowport;
	 lowport = DYN_LOW_PORT;
	 //set GlobalIPAddress
	 //need modifyOutGlobalAddr
	 ((BYTE*)(&uc_GlobalIPAddress))[0] = OutGlobalAddr.s_b[0];
	 ((BYTE*)(&uc_GlobalIPAddress))[1] = OutGlobalAddr.s_b[1];
	 ((BYTE*)(&uc_GlobalIPAddress))[2] = OutGlobalAddr.s_b[2];
	 ((BYTE*)(&uc_GlobalIPAddress))[3] = OutGlobalAddr.s_b[3];   
	 
	 	 
	 //set up a buffer for choose outport;
	 pChooseportBufferbottom = new USHORT[MAX_NAT_CONNECT];
	 pChooseportBufferheader = pChooseportBufferbottom;
     for(USHORT i=0;i<MAX_NAT_CONNECT;i++)
	 {
		 //Note: not add different port use for other side
		 pChooseportBufferbottom[i] = swaps(lowport +i);
		 pChooseportBufferheader   =  &pChooseportBufferbottom[i];
	 }
 
     
	 access_event = CreateEvent(0,1,1,0);
	 DWORD     dwThreadId;
	 //for timeout inspec
	 ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)NatBeginThread,NULL,0,&dwThreadId);
	 
     return ;
}

BOOL  NATableTimeInspec()
{
	//modify TimeOut
	//NATTABLE_ITEM* pTempItem     = 0;
	//NATTABLE_ITEM* pTempItemlast ;
	USHORT    temp;
    
	for(int i=1;i<MAX_NAT_CONNECT;i++)
	{
       
		 if(nattable[i].bUse == 0)
			 continue;
		 if(nattable[i].us_TimeOut == 0)
		 {
			 //modif nattable item and inhashidtable and outhashidtable
			 if(nattable[i].uInSuffixParent >0 )
			 	 nattable[nattable[i].uInSuffixParent].uInSuffixNext = nattable[i].uInSuffixNext;
			 else
			 {
				 temp = nattable[i].us_OutPort % 997 +1;
				 if(nattable[i].uInSuffixNext == 0)
					 InHashIDTable[temp] = 0;
				 else
				 {
					 InHashIDTable[temp] = nattable[i].uInSuffixNext;
					 //nattable[nattable[i].uInSuffixNext].uInSuffixParent = 0;
				 }
			 }

			 if(nattable[i].uInSuffixNext >0)
				 nattable[nattable[i].uInSuffixNext].uInSuffixParent = nattable[i].uInSuffixParent;		 

			 if(nattable[i].uOutSuffixParent >0 )
			 	 nattable[nattable[i].uOutSuffixParent].uOutSuffixNext = nattable[i].uOutSuffixNext;
			 else
			 {
				 temp = NATGetIndexOther(nattable[i].ul_InterIPAddress,
					    nattable[i].ul_OutIPAddress,nattable[i].us_InterPort);

				 if(nattable[i].uOutSuffixNext == 0)
					 OutHashIDTable[temp] = 0;
				 else
				 {
					 OutHashIDTable[temp] = nattable[i].uOutSuffixNext;
					 //nattable[nattable[i].uOutSuffixNext].uOutSuffixParent = 0;
				 }
			 }

			 if(nattable[i].uOutSuffixNext >0)
				 nattable[nattable[i].uOutSuffixNext].uOutSuffixParent = nattable[i].uOutSuffixParent;		 
			 //
			 pChooseportBufferheader++;
			 *pChooseportBufferheader = nattable[i].us_OutPort;
			 if(nattable[i].us_OutPort >2100)
                      TRACE(_T("port ERR\n"));

             memset(&nattable[i],0,sizeof(NATTABLE_ITEM));			 
			 TRACE(_T("del a item :%d\n"), i);
		 }
		 else
             nattable[i].us_TimeOut--;
	}
    
	return TRUE;
}

void ClearNatUseResoure()
{
	delete[]  nattable;
	//delete[]  nattableUDP;
	delete[]  pChooseportBufferbottom;
//	delete[]  statictableitem;
	//delete[]  pChooseportBufferbottomUDP;
	CloseHandle(access_event);
	//CloseHandle(access_event2);
	return;
}

void NatBeginThread(PVOID temp)
{
     while(1)
	 {
		 Sleep(60000);
		 WaitForSingleObject(access_event,INFINITE);
	     ResetEvent(access_event);		 
		 NATableTimeInspec();
         SetEvent(access_event);
		 //TRACE(_T("over timeinspec \n"));
		// WaitForSingleObject(access_event2,INFINITE);
	    // ResetEvent(access_event2);		 
		// NATableTimeInspecUDP();
        // SetEvent(access_event2);
	 }
}



USHORT  NATGetIndexOther(ULONG ipaddress1,ULONG ipaddress2,USHORT sport)
{
      USHORT  tempindex;
	  tempindex = 0;
     
	  tempindex = ((USHORT* )&ipaddress1)[0]+((USHORT* )&ipaddress1)[1]
				  +((USHORT* )&ipaddress2)[0]+((USHORT* )&ipaddress2)[1]
				  +sport;
	//  TRACE(_T("1:  %d \n"),((USHORT* )&ipaddress1)[0]);
	//  TRACE(_T("2:  %d \n"),((USHORT* )&ipaddress1)[1]);
	//  TRACE(_T("3:  %d \n"),((USHORT* )&ipaddress2)[0]);
	//  TRACE(_T("4:  %d \n"),((USHORT* )&ipaddress2)[1]);
//	  TRACE(_T("PROT:  %d \n"),sport);
//      TRACE(_T("INDEX:  %d \n"),tempindex);
	  return tempindex % 997;	  
}


USHORT swaps(USHORT port)
{
	USHORT lo,hi;
	USHORT host;

	lo=port&0xff;
	hi=port&0xff00;
	lo=lo<<8;
	hi=hi>>8;
	host=hi|lo;

	return host;
}

BOOL NATSearchStatic(BYTE* EthBuffer,BOOL Direction,USHORT type)
{
	_IP_HEADER*      pIPheader;
	_TCP_HEADER*     pTCPheader;
	_UDP_HEADER*     pUDPheader;
    
	pIPheader    = (_IP_HEADER*)(EthBuffer+14);
	pTCPheader   = (_TCP_HEADER*)(EthBuffer+34);
	pUDPheader   = (_UDP_HEADER*)(EthBuffer+34);
    
	switch (type) {
		/*
		=1:  TCP;
		=2:  UDP;
		*/
	case 1:
		if(Direction)
			/*
			Direction =TRUE;OUT
			Direction =FALSE;IN
			*/
		{		
		for(int i=0;i<StaticTCPcount;i++)
		{
			if(StaticTCPtable[i].usPort == pTCPheader->source)
			{
               // pIPheader->saddr = uc_GlobalIPAddress;
				TRACE(_T("STATIC NAT OUT TCP\n"));
				return TRUE;
			}
		}
		return FALSE;
		}
		else
		{
			if(pTCPheader->dest>DYN_LOW_PORT&&pTCPheader->dest<DYN_HIGH_PORT)
			          return FALSE;
			for(int j=0;j<StaticTCPcount;j++)
			{
			if(StaticTCPtable[j].usPort == pTCPheader->dest)
			{
                pIPheader->daddr = StaticTCPtable[j].ulIpadress;
				TRACE(_T("STATIC NAT IN TCP\n"));
				return TRUE;
			}
			}
		return FALSE;
		}
		break;
	case 2:
		if(!Direction)
			/*
			Direction =TRUE;OUT
			Direction =FALSE;IN
			*/
		{		
		   for(int f=0;f<StaticUDPcount;f++)
			{
			if(StaticUDPtable[f].usPort == pUDPheader->dest)
			{
                pIPheader->daddr = StaticUDPtable[f].ulIpadress;
				TRACE(_T("STATIC NAT IN UDP\n"));
				return TRUE;
			}
			}
		return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return FALSE;
}
