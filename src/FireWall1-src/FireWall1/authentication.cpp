
#include "stdafx.h"
#include "authentication.h"
#include "outnat.h"
#include "ip.h"
#include "Page4.h"
#include "Page5.h"
#include <stdio.h>
#include <io.h>
#include "Struct.h"
#define   ETH_HEAD    14
#define   IP_HEAD     20
#define   TIMEOUT      2
#define   MAX_RIP     20
extern HANDLE global_handle;
OVERLAPPED   OverLappedread_auth,*lpOverLappedread;
extern BYTE SpecialBuffer[1500],SpecialBuffer1[1500],SpecialBuffer2[1500];
extern BOOL  ACK,ACK1,Flag2;
extern ServiceManageData TheServiceManageData;
extern UserVerifyData  TheUserVerifyData;
extern LogManage TheLogManage;
extern BYTE INNO,OUTNO;
extern USHORT logType;
extern HWND myhWnd1;
int count2=0;
DWORD  tempack,tempseq;
BOOL   Flag1=0;
BOOL openfile1=1,openfile2=1;
int tempcount1=0,tempcount2=0;
int len1,len2;
char URLBlockMsg[4096],ContentFilterMsg[4096];
Hash hashtab;
VOID WINAPI WriteCompletionRoutineZJ(DWORD dwErrorCode, 
   DWORD dwNumberOfBytesTransferred, 
   LPOVERLAPPED pOverlapped);
VOID WINAPI WriteCompletionRoutineXZ(DWORD dwErrorCode, 
   DWORD dwNumberOfBytesTransferred, 
   LPOVERLAPPED pOverlapped);
char pat1[20][20],Host[250];
struct LogView1  TheLogView;
struct UserAuth  UserList[4096];
struct IP_Addr    mask;
struct IPPacketHead*  pIPPacketHead;
struct TCPPacketHead* pTCPPacketHead;
struct URLBlockLogMsg URLBlockLog;     //URL阻断的日志
struct FilterLogMsg   ContentFilterLog;//内容过滤的日志
FILE   *URLBlockLogFile,*AuthLogFile,*FilterLogFile;
int    i1=0,i2=0,i3=0,Count[20],HValueKeyWord[20],count=0,timer=0;
static int count1=0;
unsigned int IP[MAX_RIP];
BYTE SourceIPFlag[4096];//为0，不在表中；1，在表中，未验证；2，验证通过。
/*void Init()
{
}*/
BOOL ProtoMain(BYTE* pdata)
{
//	struct IPPacketHead*  pIPPacketHead;
//	struct TCPPacketHead* pTCPPacketHead;
	char* buf,*pAuthorization,pszCode[32], pszText[32],*pBegin;
	int   Index=0;
	BOOL  ok;
	int x;
	unsigned int tempIP;

	ACK1=0;
	mask.AddrByte[0]=255;
	mask.AddrByte[1]=255;
	mask.AddrByte[2]=255;
	mask.AddrByte[3]=0;
//	buf=(char *)pdata;
	if (pdata[0]==1)
		return TRUE;
	pIPPacketHead=(struct IPPacketHead*)(pdata+16);
	if (ACK1==1)
	{
		TRACE(_T("ID=:%x \n"),pIPPacketHead->Id);
		ACK1=0;
	}/**/
	pTCPPacketHead=(struct TCPPacketHead*)(pdata+36);
//	GetHost((char *)(pdata+2));
//	if ((swaps(pTCPPacketHead->DestPort)!=80)&&(pdata[1]==(INNO-0x30)))
//		return TRUE;
//    
//	int tempIPP=swapl(pIPPacketHead->SourIP);
	if ((pdata[0]==0)&&(pdata[1]==INNO-0x30))
	{
		if (pIPPacketHead->Proto!=6)
			return TRUE;
		if (swaps(pTCPPacketHead->DestPort)!=80)
			return TRUE;
		char *tempp;
		if (!(tempp=strstr((char *)(pdata+56),"GET ")))
			return TRUE;
		GetHost((char *)(pdata+56));
		ok=Authentication(pdata+2);
		if (!ok)
		{
			pdata[0]=1;
			return FALSE;
		}
//		strcat(Host,"\r\n");
		if ((TheServiceManageData.FilterAndBlock&2)==0)
		    Flag1=FALSE;
		else
		    Flag1=URLBlock((char *)(pdata+56));//URL阻断
//		Flag1 = FALSE;
	    if (Flag1==FALSE)
		{
			if (count1==MAX_RIP)
				return TRUE;
			if (strstr(TheServiceManageData.NoFilterURLList,Host))//&&Host[0]
			{
				memcpy((void *)&tempIP,(void *)&pIPPacketHead->DestIP,4);
				for (x=0;x<count1;x++)
				{
					if (tempIP==IP[x])
					{
						x=24;
					}
				}
				if (x!=24)
				{
					IP[count1]=tempIP;
					count1++;
				}
			}
            return TRUE;
		}
		else
		{
			tempseq=pTCPPacketHead->SeqNo;
			tempack=pTCPPacketHead->AckNo;
			SendRToServer(pdata+2);
			SendAutheMsg((char *)(pdata+2),1);
//			if (pTCPPacketHead->Flag==0x11)
//			{
//			    SendRes(pdata);
//				SendACK(pdata);
//				ACK1=1;
//			}
			ACK=1;
			ACK1=1;
			memcpy((void *)&URLBlockLog.SourIP,(void *)&pIPPacketHead->SourIP,4);
	    	memcpy((void *)&URLBlockLog.DestIP,(void *)&pIPPacketHead->DestIP,4);
	    	URLBlockLog.SourPort=swaps(pTCPPacketHead->SourPort);
	    	URLBlockLog.DestPort=swaps(pTCPPacketHead->DestPort);
	     	GetLocalTime(&(URLBlockLog.Time));
			URLBlockLog.Block='Y';
			i1++;
//			return FALSE;
		}
	}
	if ((pdata[0]==0)&&(pdata[1]==OUTNO-0x30))
	{
    	if ((TheServiceManageData.FilterAndBlock&1)==0)
	    	return TRUE;
//		GetHost((char *)(pdata+2));
		TRACE(_T("HI begin\n"));
	   	ContentFilter((char *)(pdata+2));
		TRACE(_T("HI end\n"));
		SystemLog();
	    return TRUE;
	}
	SystemLog();
    return FALSE;
}


WORD swaps(WORD net)
{
	WORD lo,hi;
	WORD host;

	lo=net&0xff;
	hi=net&0xff00;
	lo=lo<<8;
	hi=hi>>8;
	host=hi|lo;

	return host;
}
DWORD swapl(DWORD net)
{
	DWORD b1,b2,b3,b4;
	DWORD host;

	b1=net&0xff;
	b2=net&0xff00;
	b3=net&0xff0000;
	b4=net&0xff000000;

	b1=b1<<24;
	b2=b2<<8;
	b3=b3>>8;
	b4=b4>>24;

	host=b1|b2|b3|b4;

	return host;
}

void SendAutheMsg(char* pdata, int Flag)
{
//	struct IPPacketHead*  pIPPacketHead;
//	struct TCPPacketHead* pTCPPacketHead;
	struct EtherPacketHead* pEtherPacketHead;
	struct Ether_Addr* tempEther=new Ether_Addr;
	struct IP_Addr* tempIP=new IP_Addr;
	WORD   tempPort;
	DWORD  tempNO;
	int    TCPLen;
	WORD   Ttlen;
	char   len[1600];
	int    length;
//	DWORD   nbyteWrite;

//	if (Flag==1)
//	    SendRToServer((BYTE *)pdata);
	pEtherPacketHead=(struct EtherPacketHead*)(pdata);
	pIPPacketHead=(struct IPPacketHead*)(pdata+14);
	pTCPPacketHead=(struct TCPPacketHead*)(pdata+34);
	memcpy((void *)tempEther,(void *)&pEtherPacketHead->DestEther,6);
	memcpy((void *)&pEtherPacketHead->DestEther,(void *)&pEtherPacketHead->SourEther,6);
	memcpy((void *)&pEtherPacketHead->SourEther,(void *)tempEther,6);
	memcpy((void *)tempIP,(void *)&pIPPacketHead->SourIP,4);
	memcpy((void *)&pIPPacketHead->SourIP,(void *)&pIPPacketHead->DestIP,4);
	memcpy((void *)&pIPPacketHead->DestIP,(void *)tempIP,4);
	delete tempEther;
	delete tempIP;
	tempPort=pTCPPacketHead->DestPort;
	pTCPPacketHead->DestPort=pTCPPacketHead->SourPort;
	pTCPPacketHead->SourPort=tempPort;
	tempNO=swapl(pTCPPacketHead->SeqNo);
//	tempNO=pTCPPacketHead->SeqNo;
	pTCPPacketHead->SeqNo=pTCPPacketHead->AckNo;
	TCPLen=swaps(pIPPacketHead->TtlLen)-40;
//	TCPLen = 0-56;
	pTCPPacketHead->AckNo=swapl(tempNO+TCPLen);
//	pTCPPacketHead->AckNo=tempNO+TCPLen;
	if (Flag2==1)
	{
		pTCPPacketHead->Flag=0x19;
//		Flag2=0;
	}
	else
	    pTCPPacketHead->Flag=0x18;//
//	pTCPPacketHead->WndSize=0x00;
	memset(pdata+54,0,1000-54);
/*	pIPPacketHead->TtlLen=swaps(40);
	pIPPacketHead->ChkSum=0;
	pIPPacketHead->ChkSum=GetCksum((USHORT *)(pdata+16), 20);
	pTCPPacketHead->ChkSum=0;
	GetTCPCksum((BYTE *)(pdata+2), 0);
	pdata[0]=1;
//	memset(&OverLappedread_auth,0,sizeof(OVERLAPPED));
//	lpOverLappedread=&OverLappedread_auth;

	length=WriteFile(global_handle,pdata,56,&nbyteWrite,NULL);
	length=GetLastError();*/
	if (Flag==0)
	{
		strcpy(pdata+54,"HTTP/1.1 401 Access Denied\r\nTOP FireWall  Content-type: image/jpeg\r\nWWW-Authenticate: basic realm=\"JutAg\"\r\nContent-length: 30\r\n\r\n");//身份验证
    	strcpy(len, "HTTP/1.1 401 Access Denied\r\nTOP FireWall  Content-type: image/jpeg\r\nWWW-Authenticate: basic realm=\"JutAg\"\r\nContent-length: 30\r\n\r\n");
	}
	else
	{
		strcpy(pdata+54,"HTTP/1.1 200 OK\r\nContent-type: image/jpeg\r\nContent-length: 120\r\n\r\n<html><head></head><body><form><input type='button' name='WindowButton'value='OK'onclick='Refresh()'></form></body></html>");
	    strcpy(len,"HTTP/1.1 200 OK\r\nContent-type: image/jpeg\r\nContent-length: 120\r\n\r\n<html><head></head><body><form><input type='button' name='WindowButton'value='OK'onclick='Refresh()'></form></body></html>");
		pTCPPacketHead->Flag=0x22;
	}
//	strcpy(pdata+54,"HTTP/1.1 401 Unauthorized\r\nContent-type: image/jpeg\r\nWWW-Authenticate: basic realm=\"JutAg\"\r\nContent-length: 30\r\n\r\n");//身份验证
//	strcpy(len, "HTTP/1.1 401 Unauthorized\r\nContent-type: image/jpeg\r\nWWW-Authenticate: basic realm=\"JutAg\"\r\nContent-length: 30\r\n\r\n");
//	pdata[0]=0;
	Ttlen=(WORD)(strlen(len));
	Ttlen+=40;
	pIPPacketHead->TtlLen=swaps(Ttlen);
//	pTCPPacketHead->Flag=0x18;
	pIPPacketHead->ChkSum=0;
	pIPPacketHead->ChkSum=GetCksum((USHORT *)(pdata+14), 20);
	pTCPPacketHead->ChkSum=0;
	GetTCPCksum((BYTE *)(pdata), 0);
//	WriteBuffer((BYTE *)pdata);
}
void Unbase64(char* pszText, char* pszCode)
{
	int nLen, i;
	BYTE bySixBits0, bySixBits1, bySixBits2, bySixBits3, c;

	nLen = strlen(pszCode);
	for(i=0; i<nLen; )
	{
		bySixBits0 = GetSixBits(pszCode, i);
		if(i<nLen)
			bySixBits1 = GetSixBits(pszCode, i);
		if(i<nLen)
			bySixBits2 = GetSixBits(pszCode, i);
		if(i<nLen)
			bySixBits3 = GetSixBits(pszCode, i);

		c = bySixBits0<<2 | bySixBits1>>4; 
		*pszText = c; 
		pszText++;
		c = bySixBits1<<4 | bySixBits2>>2;
		*pszText = c; 
		pszText++;
		c = bySixBits2<<6 | bySixBits3;
		*pszText = c;
		pszText++;
	}
	*pszText = NULL;

}

BYTE GetSixBits(char* pszStr, int& i)
{
	char c;

	while(1)
	{
		if(i>=strlen(pszStr))break;
		c = pszStr[i++];
		if(isupper(c)) return c-'A';
		else if(islower(c)) return c-'a'+26;
		else if(isdigit(c)) return c-'0'+52;
		else if(c=='+') return 62;
		else if(c=='/') return 63;
		else if(c=='=') return 0;
	}
	return 0;
}

int SearchSourceIP(struct IPPacketHead* pIPHead)
{
	//struct IPAddr* tempIPAddr=new IPAddr;
	int  index;

	//memcpy((void *)tempIPAddr,(void *)&pIPHead->SourIP,4);
	index=((pIPHead->SourIP.AddrByte[2])&(255-mask.AddrByte[2]))*256
		 +((pIPHead->SourIP.AddrByte[3])&(255-mask.AddrByte[3]));
	if (UserList[index].PassTimes==0)
	{
		SourceIPFlag[index]=0;
//		delete tempIPAddr;//jl1106
		return index;
	}
	if ((UserList[index].PassTimes)&1==1)
	{
		SourceIPFlag[index]=2;
//		delete tempIPAddr;//jl1106
		return index;
	}
	SourceIPFlag[index]=1;
//	delete tempIPAddr;//jl1106
	return index;
}

void SendRToServer(BYTE* buf)
{
	int    TCPLen;
	DWORD  tempNO;
	struct IPPacketHead  *pIPHead;
	struct TCPPacketHead *pTCPHead;
	struct EtherPacketHead* pEtherPacketHead,*pEtherHead;
//	SpecialBuffer=new BYTE[56];

	memcpy(SpecialBuffer+2,buf,54);
	pEtherPacketHead=(struct EtherPacketHead*)(buf);
	pIPPacketHead=(struct IPPacketHead*)(buf+14);
	pTCPPacketHead=(struct TCPPacketHead*)(buf+34);
	pEtherHead=(struct EtherPacketHead*)(SpecialBuffer+2);
	pIPHead=(struct IPPacketHead*)(SpecialBuffer+16);
	pTCPHead=(struct TCPPacketHead*)(SpecialBuffer+36);
	SpecialBuffer[0]=1;
//	SpecialBuffer[1]=2;
	memset(SpecialBuffer+56,0,1500-56);
/*	memcpy((void *)&pEtherHead->DestEther,(void *)&pEtherPacketHead->SourEther,6);
	memcpy((void *)&pEtherHead->SourEther,(void *)&pEtherPacketHead->DestEther,6);
	memcpy((void *)&pIPHead->SourIP,(void *)&pIPPacketHead->DestIP,4);
	memcpy((void *)&pIPHead->DestIP,(void *)&pIPPacketHead->SourIP,4);
	pTCPHead->SourPort=pTCPPacketHead->DestPort;
	pTCPHead->DestPort=pTCPPacketHead->SourPort;*/
	pIPHead->TtlLen=swaps(40);
	pTCPHead->AckNo=tempack;
	pTCPHead->SeqNo=tempseq;
/*	tempNO=swapl(pTCPPacketHead->SeqNo);
	pTCPHead->SeqNo=pTCPPacketHead->AckNo;
	TCPLen=swaps(pIPPacketHead->TtlLen)-40;
	pTCPHead->AckNo=swapl(tempNO+TCPLen);*/
	pTCPHead->Flag=0x04;
//	pTCPHead->WndSize=0;
	pIPHead->ChkSum=0;
	pIPHead->ChkSum=GetCksum((USHORT *)(SpecialBuffer+16), 20);
	pTCPHead->ChkSum=0;
	GetTCPCksum((SpecialBuffer+2), 0);
	ACK=1;
//	delete []SpecialBuffer;//jl1106
}
BOOL URLBlock(char *buffer)
{
	char *pBegin,ch;
//	struct IPAddr Host,URL;

//	memset(Host,0,250);
	if (pBegin=strstr(buffer,"Host: "))
	{
//		GetHost(buffer);
		memcpy(URLBlockLog.URL,Host,250);
		if (strstr(TheServiceManageData.BlockURLList,Host))
			return TRUE;
		return FALSE;
	}
	return FALSE;
}
void ContentFilter(char *buffer)
{
	struct IPPacketHead* pIPHead;
	struct TCPPacketHead* pTCPHead;
	struct EtherPacketHead* pEtherHead;
	int    length;
	int    len=0;

	pIPHead=(struct IPPacketHead*)(buffer+14);
	pTCPHead=(struct TCPPacketHead*)(buffer+34);
	length=swaps(pIPHead->TtlLen)-40;
	if (pIPHead->Proto!=6)
		return;
	if ((TheServiceManageData.TranProtocal&1)==0)//不进行TCP
		return;
	if (swaps(pTCPHead->SourPort)!=80)
		return;
	if ((TheServiceManageData.Port&1)==0)//不进行HTTP
		return;
/*	if (pTCPHead->AckNo==pTCPHead->SeqNo)
	{
		memset(buffer,0,54);
		return;
	}
	*/
	if (length>10)
	{
		int tempIP,y=0;
		memcpy((void *)&tempIP,(void *)&pIPPacketHead->SourIP,4);
		TRACE(pat1[0]);
		TRACE(_T("count1=%d\n"),count1);
		for (y=0;y<count1;y++)
		{
			if(count1==1)
				TRACE(_T("Y=%d,count1=%d\n"),y,count1);
			if (tempIP==IP[y])
				return;
		}
		
		SearchFilter(buffer+54,length);
		if ((Count[0]==0)&&(Count[1]==0)&&(Count[2]==0))
			return;
		memcpy((void *)&ContentFilterLog.SourIP,(void *)&pIPPacketHead->SourIP,4);
		memcpy((void *)&ContentFilterLog.DestIP,(void *)&pIPPacketHead->DestIP,4);
		ContentFilterLog.SourPort=swaps(pTCPPacketHead->SourPort);
		ContentFilterLog.DestPort=swaps(pTCPPacketHead->DestPort);
		GetLocalTime(&(ContentFilterLog.Time));
		if (Count[0]!=0)
			strcpy(ContentFilterLog.KeyWord[0],pat1[0]);
		if (Count[1]!=0)
			strcpy(ContentFilterLog.KeyWord[1],pat1[1]);
		if (Count[2]!=0)
			strcpy(ContentFilterLog.KeyWord[2],pat1[2]);
//		if (Count[0]+Count[2]+Count[3]!=0)
		i2++;
		memset(Count,0,80);
	}
	return;
}

void SearchFilter(char *text,int len)
{
	int y,z,count=0,hashval=0,comp=0;
	char temp[4];
	for (z=0; z<len; z++)
	{
		if (text[z]=='<')
			comp=1;
		if (comp==1)
		{
		    if ((text[z]==' ')||(text[z]==13)||(text[z]==10))
			{
			    if (count<8)
				{
			        hashval+=count;
			        hashtab.pois[hashval][hashtab.number[hashval]]=z-count;
			        hashtab.number[hashval]++;
				}
				hashval=0;
		        count=0;
			    comp=0;
			}
		    else
			{
//			    temp[count]=text[z];
			    hashval+=(text[z]&95);
			    count++;
			}
		}
	}
	for (int Loop1=0;Loop1<count2;Loop1++)
	{
	for (y=0; y<hashtab.number[HValueKeyWord[Loop1]]; y++)
	{
		z=0;
		while (z<7)
		{
			if ((text[hashtab.pois[HValueKeyWord[Loop1]][y]+z]&95)!=(pat1[0][z]&95))
				z=9;
			z++;
		}
		if (z!=10)
		{
			Count[Loop1]++;
			temp[0]=text[hashtab.pois[HValueKeyWord[Loop1]][y]+1];
			temp[1]=text[hashtab.pois[HValueKeyWord[Loop1]][y]+2];
			text[hashtab.pois[HValueKeyWord[Loop1]][y]+1]=text[hashtab.pois[HValueKeyWord[Loop1]][y]+3];
			text[hashtab.pois[HValueKeyWord[Loop1]][y]+2]=text[hashtab.pois[HValueKeyWord[Loop1]][y]+4];
			text[hashtab.pois[HValueKeyWord[Loop1]][y]+3]=temp[0];
			text[hashtab.pois[HValueKeyWord[Loop1]][y]+4]=temp[1];
		}
	}
	}
/*	for (y=0; y<hashtab.number[474]; y++)
	{
		z=0;
		while (z<7)
		{
			if ((text[hashtab.pois[474][y]+z]&95)!=(pat1[1][z]&95))
				z=9;
			z++;
		}
		if (z!=10)
		{
			Count[1]++;
			temp[0]=text[hashtab.pois[474][y]+1];
			temp[1]=text[hashtab.pois[474][y]+2];
			text[hashtab.pois[474][y]+1]=text[hashtab.pois[474][y]+3];
			text[hashtab.pois[474][y]+2]=text[hashtab.pois[474][y]+4];
			text[hashtab.pois[474][y]+3]=temp[0];
			text[hashtab.pois[474][y]+4]=temp[1];
		}
	}
	for (y=0; y<hashtab.number[504]; y++)
	{
		z=0;
		while (z<7)
		{
			if ((text[hashtab.pois[504][y]+z]&95)!=(pat1[2][z]&95))
				z=9;
			z++;
		}
		if (z!=10)
		{
			Count[2]++;
			temp[0]=text[hashtab.pois[504][y]+1];
			temp[1]=text[hashtab.pois[504][y]+2];
			text[hashtab.pois[504][y]+1]=text[hashtab.pois[504][y]+3];
			text[hashtab.pois[504][y]+2]=text[hashtab.pois[504][y]+4];
			text[hashtab.pois[504][y]+3]=temp[0];
			text[hashtab.pois[504][y]+4]=temp[1];
		}
	}
	}*/
	return;
}
void SetTimeout()
{
	for (int i=0; i<4096; i++)
	{
		if (SourceIPFlag[i]==2)
		{
		    UserList[i].PassTimes+=8;
		    if ((UserList[i].PassTimes&56)>>3==TheUserVerifyData.timeRefesh)//判断是否超时;
			{
			    UserList[i].PassTimes=0;//如超时将该项清0;
				SourceIPFlag[i]=0;
			}
		}
	}
	if (timer==TheUserVerifyData.timeRefesh)
	{
		count1=0;
		timer=0;
	}
	else
	{
		timer++;
	}
}
void TimeBeginThread()
{
	while(1)
	{

		Sleep(60000);
		SetTimeout();
	}
}
void SendRes(BYTE *buf)
{
	int    TCPLen;
	DWORD  tempNO;
	struct IPPacketHead  *pIPHead;
	struct TCPPacketHead *pTCPHead;
	struct EtherPacketHead* pEtherPacketHead,*pEtherHead;
//	SpecialBuffer=new BYTE[56];

	memcpy(SpecialBuffer1+2,buf,54);
	pEtherPacketHead=(struct EtherPacketHead*)(buf);
	pIPPacketHead=(struct IPPacketHead*)(buf+14);
	pTCPPacketHead=(struct TCPPacketHead*)(buf+34);
	pEtherHead=(struct EtherPacketHead*)(SpecialBuffer1+2);
	pIPHead=(struct IPPacketHead*)(SpecialBuffer1+16);
	pTCPHead=(struct TCPPacketHead*)(SpecialBuffer1+36);
	SpecialBuffer1[0]=1;
//	SpecialBuffer1[1]=2;
	memset(SpecialBuffer1+56,0,1500-56);
/*	memcpy((void *)&pEtherHead->DestEther,(void *)&pEtherPacketHead->SourEther,6);
	memcpy((void *)&pEtherHead->SourEther,(void *)&pEtherPacketHead->DestEther,6);
	memcpy((void *)&pIPHead->SourIP,(void *)&pIPPacketHead->DestIP,4);
	memcpy((void *)&pIPHead->DestIP,(void *)&pIPPacketHead->SourIP,4);
	pTCPHead->SourPort=pTCPPacketHead->DestPort;
	pTCPHead->DestPort=pTCPPacketHead->SourPort;*/
	pIPHead->TtlLen=swaps(40);
	tempNO=swapl(pTCPPacketHead->SeqNo);
//	pTCPHead->SeqNo=pTCPPacketHead->AckNo;
//	tempNO=pTCPPacketHead->SeqNo;
//	pTCPPacketHead->SeqNo=pTCPPacketHead->AckNo;
	TCPLen=swaps(pIPPacketHead->TtlLen)-40;
//	TCPLen = 0-56;
	pTCPHead->SeqNo=swapl(tempNO+TCPLen);
//	pTCPHead->SeqNo=pTCPPacketHead->SeqNo;
//	pTCPPacketHead->AckNo=swapl(tempNO+TCPLen);
/*	tempNO=swapl(pTCPPacketHead->SeqNo);
	pTCPHead->SeqNo=pTCPPacketHead->AckNo;
	TCPLen=swaps(pIPPacketHead->TtlLen)-40;
	pTCPHead->AckNo=swapl(tempNO+TCPLen);*/
/*	if (pTCPPacketHead->Flag==0x11)
	    pTCPHead->Flag=0x10;
	else*/
	pTCPHead->Flag=0x11;
//	pTCPHead->WndSize=0;
	pIPHead->ChkSum=0;
	pIPHead->ChkSum=GetCksum((USHORT *)(SpecialBuffer1+16), 20);
	pTCPHead->ChkSum=0;
	GetTCPCksum((SpecialBuffer1+2), 0);
	ACK=1;
}
void SendACK(BYTE *buf)
{
//	int    TCPLen;
//	DWORD  tempNO;
//	struct IPPacketHead  *pIPHead;
//	struct TCPPacketHead *pTCPHead;
	struct EtherPacketHead* pEtherPacketHead;
	struct Ether_Addr* tempEther=new Ether_Addr;
	struct IP_Addr* tempIP=new IP_Addr;
	WORD   tempPort;
	DWORD  tempNO;
	int    TCPLen;
	WORD   Ttlen;
	int    length;

	pEtherPacketHead=(struct EtherPacketHead*)(buf);
	pIPPacketHead=(struct IPPacketHead*)(buf+14);
	pTCPPacketHead=(struct TCPPacketHead*)(buf+34);
	memcpy((void *)tempEther,(void *)&pEtherPacketHead->DestEther,6);
	memcpy((void *)&pEtherPacketHead->DestEther,(void *)&pEtherPacketHead->SourEther,6);
	memcpy((void *)&pEtherPacketHead->SourEther,(void *)tempEther,6);
	memcpy((void *)tempIP,(void *)&pIPPacketHead->SourIP,4);
	memcpy((void *)&pIPPacketHead->SourIP,(void *)&pIPPacketHead->DestIP,4);
	memcpy((void *)&pIPPacketHead->DestIP,(void *)tempIP,4);
	delete tempEther;
	delete tempIP;
	tempPort=pTCPPacketHead->DestPort;
	pTCPPacketHead->DestPort=pTCPPacketHead->SourPort;
	pTCPPacketHead->SourPort=tempPort;
    tempNO=swapl(pTCPPacketHead->SeqNo);
//	tempNO=pTCPPacketHead->SeqNo;
	pTCPPacketHead->SeqNo=pTCPPacketHead->AckNo;
	TCPLen=swaps(pIPPacketHead->TtlLen)-40;
//	TCPLen = 0-56;
	pTCPPacketHead->AckNo=swapl(tempNO+TCPLen+1);
	pTCPPacketHead->Flag=0x10;
//	pTCPPacketHead->WndSize=0;
	pIPPacketHead->ChkSum=0;
	pIPPacketHead->ChkSum=GetCksum((USHORT *)(buf+14), 20);
	pTCPPacketHead->ChkSum=0;
	GetTCPCksum((buf), 0);
//	ACK1=1;
}
void SystemLog()
{
	if (TheLogManage.TheProCommentsLog.isLogProtocal==FALSE)
		return;

	if (openfile1)
	{
		memset(URLBlockMsg,0,4096);
		openfile1=FALSE;
	}
	if (openfile2)
	{
		memset(ContentFilterMsg,0,4096);
		openfile2=FALSE;
	}
	if (i1!=tempcount1)
	{
		if (((TheLogManage.TheProCommentsLog.Options)&8)==8)
		{
			sprintf(TheLogView.time,"%d:%d:%d",URLBlockLog.Time.wHour,URLBlockLog.Time.wMinute,URLBlockLog.Time.wSecond);
			len1=strlen(URLBlockMsg);
			sprintf(URLBlockMsg+len1,"%d年%d月%d日%d:%d:%d    "
				,URLBlockLog.Time.wYear,URLBlockLog.Time.wMonth,URLBlockLog.Time.wDay,URLBlockLog.Time.wHour,URLBlockLog.Time.wMinute,URLBlockLog.Time.wSecond);
		}
		if (((TheLogManage.TheProCommentsLog.Options)&1)==1)
		{
			sprintf(TheLogView.src,"%d.%d.%d.%d:%d",URLBlockLog.SourIP.AddrByte[0],URLBlockLog.SourIP.AddrByte[1],URLBlockLog.SourIP.AddrByte[2],URLBlockLog.SourIP.AddrByte[3],URLBlockLog.SourPort);
		    sprintf(TheLogView.dst,"%d.%d.%d.%d:%d",URLBlockLog.DestIP.AddrByte[0],URLBlockLog.DestIP.AddrByte[1],URLBlockLog.DestIP.AddrByte[2],URLBlockLog.DestIP.AddrByte[3],URLBlockLog.DestPort);
			len1=strlen(URLBlockMsg);
			sprintf(URLBlockMsg+len1,"源地址:%d.%d.%d.%d:%d   目的地址:%d.%d.%d.%d:%d   "
				,URLBlockLog.SourIP.AddrByte[0],URLBlockLog.SourIP.AddrByte[1],URLBlockLog.SourIP.AddrByte[2],URLBlockLog.SourIP.AddrByte[3],URLBlockLog.SourPort
				,URLBlockLog.DestIP.AddrByte[0],URLBlockLog.DestIP.AddrByte[1],URLBlockLog.DestIP.AddrByte[2],URLBlockLog.DestIP.AddrByte[3],URLBlockLog.DestPort);
		}
		if (((TheLogManage.TheProCommentsLog.Options)&4)==4)
		{
			sprintf(TheLogView.URL,"%s",URLBlockLog.URL);
			len1=strlen(URLBlockMsg);
			sprintf(URLBlockMsg+len1,"请求的URL:%s"
				,URLBlockLog.URL);
		}
		len1=strlen(URLBlockMsg);
		sprintf(URLBlockMsg+len1,"\r\n");
		sprintf(TheLogView.demo,"内网入站");
		if (logType==2)
	//	PostMessage(AfxGetMainWnd()->GetSafeHwnd(),M_LOGVIEW,(WPARAM)&TheLogView,(LPARAM)113);
            PostMessage(myhWnd1,M_LOGVIEW,(WPARAM)&TheLogView,(LPARAM)113);
		tempcount1=i1;
		memset(&URLBlockLog,0,(sizeof(struct URLBlockLogMsg)));
		if (tempcount1==20)
		{
			URLBlockLogFile=fopen("URLBlockLog.log","a");
			fprintf(URLBlockLogFile,URLBlockMsg);
			fclose(URLBlockLogFile);
			openfile1=TRUE;
			tempcount1=0;
			i1=0;
			memset(URLBlockMsg,0,4096);
		}
	}
	if (i2!=tempcount2)
	{
		if (((TheLogManage.TheProCommentsLog.Options)&8)==8)
		{
			sprintf(TheLogView.time,"%d:%d:%d",ContentFilterLog.Time.wHour,ContentFilterLog.Time.wMinute,ContentFilterLog.Time.wSecond);
			len2=strlen(ContentFilterMsg);
			sprintf(ContentFilterMsg+len2,"%d年%d月%d日%d:%d:%d  "
				,ContentFilterLog.Time.wYear,ContentFilterLog.Time.wMonth,ContentFilterLog.Time.wDay,ContentFilterLog.Time.wHour,ContentFilterLog.Time.wMinute,ContentFilterLog.Time.wSecond);
		}
		if (((TheLogManage.TheProCommentsLog.Options)&1)==1)
		{
			sprintf(TheLogView.src,"%d.%d.%d.%d:%d",ContentFilterLog.SourIP.AddrByte[0],ContentFilterLog.SourIP.AddrByte[1],ContentFilterLog.SourIP.AddrByte[2],ContentFilterLog.SourIP.AddrByte[3],ContentFilterLog.SourPort);
			sprintf(TheLogView.dst,"%d.%d.%d.%d:%d",ContentFilterLog.DestIP.AddrByte[0],ContentFilterLog.DestIP.AddrByte[1],ContentFilterLog.DestIP.AddrByte[2],ContentFilterLog.DestIP.AddrByte[3],ContentFilterLog.DestPort);
			len2=strlen(ContentFilterMsg);
			sprintf(ContentFilterMsg+len2,"源地址:%d.%d.%d.%d:%d   目的地址:%d.%d.%d.%d:%d"
				,ContentFilterLog.SourIP.AddrByte[0],ContentFilterLog.SourIP.AddrByte[1],ContentFilterLog.SourIP.AddrByte[2],ContentFilterLog.SourIP.AddrByte[3],ContentFilterLog.SourPort
				,ContentFilterLog.DestIP.AddrByte[0],ContentFilterLog.DestIP.AddrByte[1],ContentFilterLog.DestIP.AddrByte[2],ContentFilterLog.DestIP.AddrByte[3],ContentFilterLog.DestPort);
		}
		if (((TheLogManage.TheProCommentsLog.Options)&2)==2)
		{
			sprintf(TheLogView.keyword,"%s  %s  %s",ContentFilterLog.KeyWord[0],ContentFilterLog.KeyWord[1],ContentFilterLog.KeyWord[2]);
			len2=strlen(ContentFilterMsg);
			sprintf(ContentFilterMsg+len2,"含关键字:%s  %s  %s"
				,ContentFilterLog.KeyWord[0],ContentFilterLog.KeyWord[1],ContentFilterLog.KeyWord[2]);
		}		
		len2=strlen(ContentFilterMsg);
		sprintf(ContentFilterMsg+len2,"\r\n");
		sprintf(TheLogView.demo,"外网入站");
		if (logType==2)
//		PostMessage(AfxGetMainWnd()->GetSafeHwnd(),M_LOGVIEW,(WPARAM)&TheLogView,(LPARAM)114);
		PostMessage(myhWnd1,M_LOGVIEW,(WPARAM)&TheLogView,(LPARAM)114);
		memset(&ContentFilterLog,0,(sizeof(struct FilterLogMsg)));
		tempcount2=i2;
		if (tempcount2=20)
		{
			FilterLogFile=fopen("ContentFilterLog.log","a");
			fprintf(FilterLogFile,ContentFilterMsg);
			fclose(FilterLogFile);
			openfile2=TRUE;
			tempcount2=0;
		    i2=0;
			memset(ContentFilterMsg,0,4096);
		}
	}
}
void ProtoManInit()
{
	int xx=0,yy=0,zz=1;
	memset(pat1[0],0,512);
	memset(pat1[1],0,512);
	memset(pat1[2],0,512);
	pat1[0][0]='<';
	while(TheServiceManageData.FilterKeyWord[xx]!=0)
	{
		if ((TheServiceManageData.FilterKeyWord[xx]!='\r')&&(TheServiceManageData.FilterKeyWord[xx]!='\n'))
		{
			pat1[yy][zz]=TheServiceManageData.FilterKeyWord[xx];
			zz++;
			xx++;
		}
		else
		{
			if (TheServiceManageData.FilterKeyWord[xx+2]!=0)
			{
			    yy++;
				pat1[yy][0]='<';
			}
			zz=1;
			xx+=2;
		}
	}
	count2=yy+1;
	zz=0;
	memset(HValueKeyWord,0,20);
	for (xx=0;xx<count2;xx++)
	{
		while (pat1[xx][zz])//((pat1[xx][zz]!='\r')&&(pat1[xx][zz]!='\n'))
		{
			HValueKeyWord[xx]+=(pat1[xx][zz]&95);
			zz++;
		}
		HValueKeyWord[xx]+=zz;
		zz=0;
	}
}
BOOL Authentication(BYTE *pdata)
{
	char* buf,*pAuthorization,pszCode[32], pszText[32],*pBegin;
	int   Index=0;

	ACK1=0;
/*	mask.AddrByte[0]=255;
	mask.AddrByte[1]=255;
	mask.AddrByte[2]=255;
	mask.AddrByte[3]=0;*/
	buf=(char *)pdata;
	if (TheUserVerifyData.IsVerify==0)
	    return TRUE;
	pIPPacketHead=(struct IPPacketHead*)(pdata+14);
//	if (pIPPacketHead->Proto!=6)
//		return TRUE;
	pTCPPacketHead=(struct TCPPacketHead*)(pdata+34);
//	if (swaps(pTCPPacketHead->DestPort)!=80)
//		return TRUE;
	if (ACK1==1)
	{
		TRACE(_T("ID=:%x \n"),pIPPacketHead->Id);
		ACK1=0;
	}/**/
//    
//	int tempIPP=swapl(pIPPacketHead->SourIP);
	Index=SearchSourceIP(pIPPacketHead);//查找源IP地址的列表
	//Index = 2;
	//TRACE(_T("SourceIPFlag[Index]=:%d \n"),SourceIPFlag[Index]);
	//TRACE(_T("Index=:%d \n"),Index);
//	SourceIPFlag[Index] = 2;
	if (SourceIPFlag[Index]==0)//没有验证
	{
		
		if (pTCPPacketHead->Flag==0x11)
		{
			SendACK(pdata);
			TRACE(_T("CLIENT CLOSE\n"));
			ACK1=1;	
//			Flag2=0;
			return FALSE;
		}
		/**/
		if(pBegin=strstr(buf+54,"GET"))
		{
//			pAuthorization=strstr(buf+54,"Authorization: Basic");
//		    if (!pAuthorization)
//			{
			tempseq=pTCPPacketHead->SeqNo;
			tempack=pTCPPacketHead->AckNo;
			SendRToServer(pdata);
			//TRACE(_T("CLOSE server\n"));
		    SendAutheMsg(buf, 0);//发送验证信息
//			TRACE(_T("send authe\n"));
			SendRes(pdata);
//			Flag2=0;
			ACK=1;
			ACK1=1;
			UserList[Index].PassTimes+=2;			
//			SourceIPFlag[Index]=1;
			return FALSE;
		}
		else
		{
			ACK1=0;
			return TRUE;
		}
	}
	if (SourceIPFlag[Index]==1)//正在验证
	{
		TRACE(_T("正在验证\n"));
			pAuthorization=strstr(buf+54,"Authorization: Basic");
			if (!pAuthorization)
			{
				if (strstr(buf+54,"GET"))
				{
					tempseq=pTCPPacketHead->SeqNo;
			        tempack=pTCPPacketHead->AckNo;
        			SendRToServer(pdata);
	        	    SendAutheMsg(buf, 0);//发送验证信息
//					if (pTCPPacketHead->Flag==0x11)
//					{
					    SendRes(pdata);
//						SendACK(pdata);
						ACK1=1;
//					}
//	        		UserList[Index].PassTimes+=2;
        			ACK=1;
		        	return FALSE;
				}
				if (pTCPPacketHead->Flag==0x11)
				{
		    	    SendACK(pdata);
					//Flag2=0;
			        ACK1=1;	
			        return FALSE;
				}
				return TRUE;
			}
			else
			{
				int i=0;
				while (((pAuthorization+21)[i]!=' ')&&((pAuthorization+21)[i]!='\r')&&((pAuthorization+21)[i]!='\n'))
					i++;
				if (i>30)
					goto die;
				sscanf(pAuthorization+21, "%s", pszCode);
				memset(pszText,0,32);
		        Unbase64(pszText, pszCode);
				strcat(pszText,"\r\n");
//				memcpy(AuthLog[i2].UserPaswd,pszText,32);
//				memcpy((void *)&AuthLog[i2].SourIP,(void *)&pIPPacketHead->SourIP,4);
//				GetLocalTime(&(AuthLog[i2].Time));
		        if(strstr(TheUserVerifyData.UserList,pszText))
				{
			        buf[0]=1;
//			        SourceIPFlag=2;
					UserList[Index].PassTimes=1;
//			        SendAutheMsg(buf, 1);
					ACK=0;
//					SourceIPFlag[Index]=2;
//					AuthLog[i2].RightUser='Y';
//					i2++;
					return TRUE;
				}
		        else
				{
die:;
			    	if ((UserList[Index].PassTimes)==6)
					{
						UserList[Index].PassTimes=0;
					}
					else
						UserList[Index].PassTimes+=2;
					tempseq=pTCPPacketHead->SeqNo;
				    tempack=pTCPPacketHead->AckNo;
				    SendRToServer(pdata);
					Flag2=1;
			        SendAutheMsg(buf, 0);					
/*					if (pTCPPacketHead->Flag==0x11)
					{
					    if (Flag2==0)
					        SendRes(pdata);
					    Flag2=1;
						SendACK(pdata);
						ACK1=1;
					}*/
					ACK=1;
					ACK1=1;
//					AuthLog[i2].RightUser='N';
//					i2++;
					return FALSE;
				}
//				return;
			}
	}
	else//验证通过
	{
		UserList[Index].PassTimes=1;
		return TRUE;
	}
}
void GetHost(char *buffer)
{
	char *pBegin;

	memset(Host,0,250);
	if (pBegin=strstr(buffer,"Host: "))
	{
		int i=0;
		while ((pBegin[6+i]!='\r')&&(i<250))
		{
			Host[i]=pBegin[6+i];
			i++;
		}
//		memcpy(URLBlockLog[i1].URL,Host,250);
		strcat(Host,"\r\n");
//		if (strstr(TheServiceManageData.BlockURLList,Host))
//			return TRUE;
//		return FALSE;
	}
}
void SendSpecialBuffer()
{
	OVERLAPPED Overlapped;

	memset(&Overlapped,0,sizeof(OVERLAPPED));
	if (ACK==1)
		{
			TRACE(_T("ACK==1\n"));
		if (Flag2==0)
			{
			SpecialBuffer1[0]=0;
			SpecialBuffer1[1]=(INNO-0x30);
			//实际，tempdata[aa][0]=1;
		    WriteFileEx(global_handle,
				SpecialBuffer1,//TCP数据0字节的响应包
					//其中，SpecialBuffer[0]=1，
				56,
				&Overlapped,
				WriteCompletionRoutineZJ);/**/
		//count++;
			}
		SpecialBuffer[0]=0;
		    SpecialBuffer[1]=(OUTNO-0x30);
		    WriteFileEx(global_handle,
				SpecialBuffer,//TCP数据0字节的响应包
					//其中，SpecialBuffer[0]=1，
				56,
				&Overlapped,
				WriteCompletionRoutineXZ);
				/**/
		ACK=0;
		Flag2=0;
		}
}
VOID WINAPI WriteCompletionRoutineZJ(DWORD dwErrorCode, 
   DWORD dwNumberOfBytesTransferred, 
   LPOVERLAPPED pOverlapped)
{
	memset(SpecialBuffer1,0,1500);
}
VOID WINAPI WriteCompletionRoutineXZ(DWORD dwErrorCode, 
   DWORD dwNumberOfBytesTransferred, 
   LPOVERLAPPED pOverlapped)
{
	memset(SpecialBuffer,0,1500);
}
