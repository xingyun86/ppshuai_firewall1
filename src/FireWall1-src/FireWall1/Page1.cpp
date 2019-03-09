
// Page1.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "global.h"
#include "Page1.h"
#include "authentication.h"
#include <share.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CDataConvert gDataConvert;
extern FILE * pfoutDeviceInit;
static int NetNo;
CString strNetName[8];
NetCardProp myNetCardProp[8];
static FILE *pfoutDevice;
FILE *pwriteDevice;
char tmpin[5];//用于存取网卡设备号
char tmpout[5];
char tmpdm[5];
BYTE INNO;
BYTE OUTNO;
BYTE DMNO;
char tmpinsel[5];//用于选取网卡设备号
char tmpoutsel[5];
char tmpdmsel[5];
BOOL com2=0,com3=0,com4=0,nook1;
extern int global_set;
extern char NETMASK[20];
/////////////////////////////////////////////////////////////////////////////
// CPage1 property page

IMPLEMENT_DYNCREATE(CPage1, CPropertyPage)

CPage1::CPage1() : CPropertyPage(CPage1::IDD)
{
	//{{AFX_DATA_INIT(CPage1)
	m_OutNetIp = _T("");
	m_OutNetMask = _T("");
	m_InNetIp = _T("");
	m_InNetMask = _T("");
	m_DmIp = _T("");
	m_DmMask = _T("");
	m_UdpTimeOut = 0;
	m_PortEnd = 0;
	m_MaxPortNum = 0;
	m_PortBegin = 0;
	m_TcpTimeOut = 0;
	//}}AFX_DATA_INIT
	for(int i=0;i<8;i++)
		memset(&myNetCardProp[i],0,sizeof(NetCardProp));
	NetNo=0;
	memset(tmpinsel,0,5);
	memset(tmpoutsel,0,5);
	memset(tmpdmsel,0,5);
	nook1=1;
}

CPage1::~CPage1()
{
/*	if(nook1)
	{
		INNO=tmpin[0];
//		TRACE(tmpin);
		OUTNO=tmpout[0];
		DMNO=tmpdm[0];
	}
	*/
	

}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1)
	DDX_Control(pDX, IDC_EDIT15, m_StaticNatMap_tcp);
	DDX_Control(pDX, IDC_EDIT16, m_StaticNatMap_udp);
	DDX_Control(pDX, IDC_COMBO4, m_SelAdaptDm);
	DDX_Control(pDX, IDC_COMBO3, m_SelAdaptInNet);
	DDX_Control(pDX, IDC_COMBO2, m_SelAdaptOutNet);
	DDX_Text(pDX, IDC_EDIT3, m_OutNetIp);
	DDX_Text(pDX, IDC_EDIT4, m_OutNetMask);
	DDX_Text(pDX, IDC_EDIT5, m_InNetIp);
	DDX_Text(pDX, IDC_EDIT6, m_InNetMask);
	DDX_Text(pDX, IDC_EDIT7, m_DmIp);
	DDX_Text(pDX, IDC_EDIT8, m_DmMask);
	DDX_Text(pDX, IDC_EDIT10, m_UdpTimeOut);
	DDX_Text(pDX, IDC_EDIT11, m_PortEnd);
	DDX_Text(pDX, IDC_EDIT12, m_MaxPortNum);
	DDX_Text(pDX, IDC_EDIT2, m_PortBegin);
	DDX_Text(pDX, IDC_EDIT9, m_TcpTimeOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1 message handlers

void CPage1::OnButton1() 
{
	nook1=0;
 UpdateData(1);
 strcpy(NETMASK,m_InNetMask.GetBuffer(m_InNetMask.GetLength ()));
 if(!GetBestData())return;
 strcpy(gDataConvert.m_alllinechar.FireWallCfg.csIntraIp,m_InNetIp.GetBuffer(m_InNetIp.GetLength())); 
 strcpy(gDataConvert.m_alllinechar.FireWallCfg.csIntraMask,m_InNetMask.GetBuffer(m_InNetMask.GetLength()));
 strcpy(gDataConvert.m_alllinechar.FireWallCfg.csMiddleIp,m_DmIp.GetBuffer(m_DmIp.GetLength()));
 strcpy(gDataConvert.m_alllinechar.FireWallCfg.csMiddleMask,m_DmMask.GetBuffer(m_DmMask.GetLength()));
 strcpy(gDataConvert.m_alllinechar.FireWallCfg.csOutsideIp,m_OutNetIp.GetBuffer(m_OutNetIp.GetLength()));
 strcpy(gDataConvert.m_alllinechar.FireWallCfg.csOutsideMask,m_OutNetMask.GetBuffer(m_OutNetMask.GetLength()));
 gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nHighPort=m_PortEnd;
 gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nLowPort=m_PortBegin;
 gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nMaxNatNumber=m_MaxPortNum;
 gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nTimeOut_TCP=m_TcpTimeOut;
 gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nTimeOut_UDP=m_UdpTimeOut;
 gDataConvert.SaveChar(m_StaticNatMap_tcp,FALSE,MAPSTATIC_TCP);
 gDataConvert.SaveChar(m_StaticNatMap_udp,FALSE,MAPSTATIC_UDP);
 gDataConvert.InitFireWall();
	//存于文件，以便重新启动后可以直接显示网卡
 	if(com3) strcpy(tmpin,tmpinsel);
	if(com4) strcpy(tmpdm,tmpdmsel);
	if(com2) strcpy(tmpout,tmpoutsel);
	pwriteDevice=fopen("device.txt","w");
	if(pwriteDevice)
	{
		fprintf(pwriteDevice,_T("[InNet]\n"));
		fprintf(pwriteDevice,tmpin);
		fprintf(pwriteDevice,"\n");
		fprintf(pwriteDevice,"\n");
		fprintf(pwriteDevice,_T("[OutNet]\n"));
		fprintf(pwriteDevice,tmpout);
		fprintf(pwriteDevice,"\n");
		fprintf(pwriteDevice,"\n");
		fprintf(pwriteDevice,_T("[DM]\n"));
		fprintf(pwriteDevice,tmpdm);
		fprintf(pwriteDevice,"\n");
		fprintf(pwriteDevice,"\n");
		fprintf(pwriteDevice,_T("[INNETMASK]\n"));
		fprintf(pwriteDevice,NETMASK);
		fprintf(pwriteDevice,"\n");
		fprintf(pwriteDevice,"\n");
		fprintf(pwriteDevice,_T("[jl]\n"));
		INNO=tmpin[0];
		OUTNO=tmpout[0];
		DMNO=tmpdm[0];
	}
	if(pwriteDevice!=NULL)
		int errrr=fclose(pwriteDevice);
	global_set|=0x1;

}

BOOL CPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	GetNetCardName();
    m_PortEnd=gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nHighPort;
    m_PortBegin=gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nLowPort;
    m_MaxPortNum=gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nMaxNatNumber;
    m_TcpTimeOut=gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nTimeOut_TCP;
    m_UdpTimeOut=gDataConvert.m_alllinechar.FireWallCfg.sNatCfg.nTimeOut_UDP;
    gDataConvert.DisplayFireCfg(m_StaticNatMap_tcp,MAPSTATIC_TCP);
    gDataConvert.DisplayFireCfg(m_StaticNatMap_udp,MAPSTATIC_UDP);
//	GetDataFromFile();
//	if(pfoutDeviceInit)
	{
		int aa;
		char tmp[50];
		
		memset(tmp,0,50);
		strcpy(tmp,"ImSampMP");
		if(tmpin[0]!=0)
		{
		//	AfxMessageBox(tmpin);
			strcat(tmp,tmpin);
			aa=m_SelAdaptInNet.SelectString(-1,tmp);
			m_InNetIp=myNetCardProp[aa].IP;
			m_InNetMask=myNetCardProp[aa].IPMASK;
		}
		memset(tmp,0,50);
		strcpy(tmp,"ImSampMP");
		if(tmpout[0]!=0)
		{
			strcat(tmp,tmpout);
			aa=m_SelAdaptOutNet.SelectString(-1,tmp);
			m_OutNetIp=myNetCardProp[aa].IP;
			m_OutNetMask=myNetCardProp[aa].IPMASK;
		}
		memset(tmp,0,50);
		strcpy(tmp,"ImSampMP");
		if(tmpdm[0]!=0)
		{
			strcat(tmp,tmpdm);
			aa=m_SelAdaptDm.SelectString(-1,tmp);
			m_DmIp=myNetCardProp[aa].IP;
			m_DmMask=myNetCardProp[aa].IPMASK;
		}
		UpdateData(FALSE);
	
	}
//	if(pfoutDevice)
//		int errrr=fclose(pfoutDevice);
		
    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage1::GetNetCardName()
{
	HKEY			SoftwareKey;
	HKEY			MicrosoftKey;
	HKEY			WindowsNTKey;
	HKEY			CurrentVersionKey;
	HKEY			NetworkCardsKey;
	static HKEY			NetCardNoKey;
	int				no=0;
//	char *			pStr=new char [20];
//	char *			strtemp=new char [20];
	char            pStr[35];
	char            strtemp[35];
    LONG            Status;
	BOOL			enumover=1;
    DWORD           RegType;
    ULONG BufferSize;
	BufferSize=256;//128;
//	pStr=(char *)malloc(20);
//	strtemp=(char *)malloc(20);
	/////////////////////////////////
	//取得网卡名字
	/////////////////////////////////
	Status = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            TEXT("SOFTWARE"),
            0,
            KEY_READ,
            &SoftwareKey
            );

    if (Status == ERROR_SUCCESS)
    {

        Status = RegOpenKeyEx(
                SoftwareKey,
                TEXT("Microsoft"),
                0,
                KEY_READ,
                &MicrosoftKey
                );

        if (Status == ERROR_SUCCESS)
        {

            Status = RegOpenKeyEx(
                    MicrosoftKey,
                    TEXT("Windows NT"),
                    0,
                    KEY_READ,
                    &WindowsNTKey
                    );

            if (Status == ERROR_SUCCESS)
            {

                Status = RegOpenKeyEx(
                        WindowsNTKey,
                        TEXT("CurrentVersion"),
                        0,
                        KEY_READ,
                        &CurrentVersionKey
                        );

                if (Status == ERROR_SUCCESS)
                {

                    Status = RegOpenKeyEx(
                            CurrentVersionKey,
                            TEXT("NetworkCards"),
                            0,
                            KEY_READ,
                            &NetworkCardsKey
                            );


                    if (Status == ERROR_SUCCESS)
                    {
						while(1)
						{
							memset((LPTSTR)&strtemp,0,20);
							Status=RegEnumKey(
								NetworkCardsKey,
								no++,
								(LPTSTR)&strtemp,
								5);
							if(Status==ERROR_NO_MORE_ITEMS)break;
							if(Status==ERROR_SUCCESS)
							{
								Status=RegOpenKeyEx(
									NetworkCardsKey,
									TEXT((LPTSTR)&strtemp),
									0,
									KEY_READ,
									&NetCardNoKey);
								if(Status==ERROR_SUCCESS)
								{
									Status=RegQueryValueEx(
										NetCardNoKey,
										TEXT("ProductName"),
										0,
										&RegType,
										(LPBYTE) (LPTSTR)&pStr,
										&BufferSize);
									
									
									if(Status==ERROR_SUCCESS)
									{
										BufferSize=256;
										
										if(_stricmp((LPTSTR)&pStr,"ImSampMP")==0)//"DE22X")==0)ImSampMP
										{
											memset((LPTSTR)&pStr,0,20);
											Status=RegQueryValueEx(
												NetCardNoKey,
												TEXT("ServiceName"),
												0,
												&RegType,
												(LPBYTE) (LPTSTR)&pStr,
												&BufferSize);
											if(Status==ERROR_SUCCESS)
											{
												int bbb;
												m_SelAdaptOutNet.InsertString(NetNo,(LPTSTR)&pStr);//
												bbb=m_SelAdaptInNet.InsertString(NetNo,(LPTSTR)&pStr);
												m_SelAdaptDm.InsertString(NetNo,(LPTSTR)&pStr);
												strcpy((char *)myNetCardProp[NetNo].name,(char *)(LPTSTR)&pStr);
												int aaa;
												aaa=strlen((char *)myNetCardProp[NetNo].name);
												myNetCardProp[NetNo].DeviceNo=atoi((char *)(LPTSTR)&pStr+aaa-1);
												
												//	myNetCardProp[NetNo].DeviceNo=atoi((char *)(myNetCardProp[NetNo].name+aaa-1));
												GetNetCardIP((LPBYTE)(LPTSTR)&pStr);
												NetNo++;
											}
										}
									}
									
								}
								RegCloseKey(NetCardNoKey);
								NetCardNoKey=NULL;
							}
							
						}
						RegCloseKey(NetworkCardsKey);
					}
					RegCloseKey(CurrentVersionKey);
				}
				RegCloseKey(WindowsNTKey);
			}
			RegCloseKey(MicrosoftKey);
		}
		RegCloseKey(SoftwareKey);
	}
//	delete [](strtemp);
//	delete [](pStr);
}


void CPage1::GetNetCardIP(LPBYTE pStr)
{
    HKEY            SystemKey;
    HKEY            ControlSetKey;
    HKEY            ServicesKey;
    HKEY            NdisPerfKey;
    HKEY            ParametersKey;
	static HKEY			TcpipKey;
    LONG            Status;
	BYTE			Data;
//	HKEY           
	static HANDLE			hkeytmp;
//	LPTSTR			ptemp;
//	ptemp=new char [20];
	 BYTE            ptemp[35];
	memset(ptemp,0,35);
    DWORD           RegType;
	DWORD           RegType1;
    DWORD BufferSize;
	BufferSize=128;
	char	text[35];
	memset(text,0,35);

    Status = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            TEXT("SYSTEM"),
            0,
            KEY_READ,
            &SystemKey
            );

    if (Status == ERROR_SUCCESS)
    {

        Status = RegOpenKeyEx(
                SystemKey,
                TEXT("CurrentControlSet"),
                0,
                KEY_READ,
                &ControlSetKey
                );

        if (Status == ERROR_SUCCESS)
        {

            Status = RegOpenKeyEx(
                    ControlSetKey,
                    TEXT("Services"),
                    0,
                    KEY_READ,
                    &ServicesKey
                    );

            if (Status == ERROR_SUCCESS)
            {

                Status = RegOpenKeyEx(
                        ServicesKey,
                        (LPCTSTR)pStr,
                        0,
                        KEY_READ,
                        &NdisPerfKey
                        );

                if (Status == ERROR_SUCCESS)
                {

                    Status = RegOpenKeyEx(
                            NdisPerfKey,
                            TEXT("Parameters"),
                            0,
                            KEY_READ,
                            &ParametersKey
                            );


                    if (Status == ERROR_SUCCESS)
                    {
						Status=RegOpenKeyEx(
							ParametersKey,
							TEXT("Tcpip"),
							0,
							KEY_READ,
							&TcpipKey);
				//		memcpy((void *)&hkeytmp,(void*)TcpipKey,sizeof(HKEY));
						hkeytmp=TcpipKey;
						if(Status == ERROR_SUCCESS)
						{

							 Status = RegQueryValueEx(
									TcpipKey,
									TEXT("EnableDHCP"),
									NULL,
									&RegType,
									&Data,
									&BufferSize
									);
					//		 Status=RegCloseKey(TcpipKey);
							 
							 BufferSize=256;
							 if(Data==0)//不允许DHCP
							 {
						/*		 Status=RegOpenKeyEx(
										ParametersKey,
										TEXT("Tcpip"),
										0,
										KEY_READ,
										&TcpipKey);
										*/
									memset(ptemp,0,20);
						/*			Status=RegQueryValue(
										TcpipKey,
										TEXT("IPAddress"),
										(char *)(LPBYTE)ptemp,
										(LONG *)&BufferSize);
										*/
									strcpy(text,"IPAddress");
									Status = RegQueryValueEx(
										TcpipKey,
										(LPTSTR)text,//TEXT("SubnetMask"),//IPAddress
										NULL,
										&RegType,
										(LPBYTE)ptemp,
										&BufferSize
										);
							//		TcpipKey=(HKEY)hkeytmp;
							//		memcpy((void*)TcpipKey,(void *)&(hkeytmp),sizeof(HKEY));	
									strcpy((char *)myNetCardProp[NetNo].IP,(char *)ptemp);
									BufferSize=256;
									memset(ptemp,0,20);
						/*			Status=RegCloseKey(TcpipKey);
									Status=RegOpenKeyEx(
										ParametersKey,
										TEXT("Tcpip"),
										0,
										KEY_READ,
										&TcpipKey);
										*/
									Status = RegQueryValueEx(
										TcpipKey,
										TEXT("SubnetMask"),
										NULL,
										&RegType1,
										(LPBYTE)ptemp,
										&BufferSize
										);
									strcpy((char *)myNetCardProp[NetNo].IPMASK,(char *)ptemp);
							 }
							 else if(Data==1)//允许DHCP
							 {
								 BufferSize=256;
								 memset(ptemp,0,20);
								 Status = RegQueryValueEx(
										TcpipKey,
										TEXT("DhcpIPAddress"),
										NULL,
										&RegType,
										(LPBYTE)&ptemp,
										&BufferSize
										);
								 strcpy((char *)myNetCardProp[NetNo].IP,(char *)ptemp);
								 BufferSize=256;
								 memset(ptemp,0,20);
								 Status = RegQueryValueEx(
										TcpipKey,
										TEXT("DhcpSubnetMask"),
										NULL,
										&RegType1,
										(LPBYTE)&ptemp,
										&BufferSize
										);
								 strcpy((char *)myNetCardProp[NetNo].IPMASK,(char *)ptemp);
							 }
	                        RegCloseKey(TcpipKey);
						}
						RegCloseKey(ParametersKey);
                    }

                    RegCloseKey(NdisPerfKey);
                }

                RegCloseKey(ServicesKey);
            }

            RegCloseKey(ControlSetKey);
        }

        RegCloseKey(SystemKey);
    }


    return ;

}

void CPage1::OnSelchangeCombo2() 
{
	// TODO: Add your control notification handler code here
	int item=m_SelAdaptOutNet.GetCurSel();
	memset(tmpoutsel,0,5);
	itoa(myNetCardProp[item].DeviceNo,tmpoutsel,10);
//	OUTNO=tmpoutsel[0];
	myNetCardProp[item].Flag=1;
	m_OutNetIp=myNetCardProp[item].IP;
	m_OutNetMask=myNetCardProp[item].IPMASK;
	com2=1;
	UpdateData(FALSE);

		
}

void CPage1::OnSelchangeCombo4() 
{
	// TODO: Add your control notification handler code here
	int item=m_SelAdaptDm.GetCurSel();
	memset(tmpdmsel,0,5);
	itoa(myNetCardProp[item].DeviceNo,tmpdmsel,10);
//	DMNO=tmpdmsel[0];
	myNetCardProp[item].Flag=2;
	m_DmIp=myNetCardProp[item].IP;
	m_DmMask=myNetCardProp[item].IPMASK;
	com4=1;
	UpdateData(FALSE);
}

void CPage1::OnSelchangeCombo3() 
{
	// TODO: Add your control notification handler code here
	int item=m_SelAdaptInNet.GetCurSel();
	memset(tmpinsel,0,5);
	itoa(myNetCardProp[item].DeviceNo,tmpinsel,10);
//	INNO=tmpinsel[0];
	myNetCardProp[item].Flag=0;
	m_InNetIp=myNetCardProp[item].IP;
	m_InNetMask=myNetCardProp[item].IPMASK;
	UpdateData(FALSE);
	com3=1;
}



WORD CPage1::GetDataFromFile()
{
	char command[40];
	char *pbuf;
	pfoutDevice=NULL;
	pfoutDevice=fopen("device.txt","r");
	if(pfoutDevice==NULL)return 0;
	while(feof(pfoutDevice)==0)
	{
		ReadCommand(pfoutDevice,command);
		switch(GetCommand(command))
		{
		case INNET:
			ReadCommand(pfoutDevice,command);
			strcpy(tmpin,command);
			INNO=tmpin[0];
//		TRACE(tmpin);
		
		
			break;
		case OUTNET:
			ReadCommand(pfoutDevice,command);
			strcpy(tmpout,command);
			OUTNO=tmpout[0];
			break;
		case DM:
			ReadCommand(pfoutDevice,command);
			strcpy(tmpdm,command);
			DMNO=tmpdm[0];
			break;
		case INNETMASK:
			ReadCommand(pfoutDevice,command);
			strcpy(NETMASK,command);
			break;
		}
	}
	return 1;
}

void CPage1::ReadCommand(FILE *pf, char *pbuf)
{
	while(feof(pf)==0) {
		*pbuf=fgetc(pf);
		if(*pbuf=='\n') break;
		pbuf++;
	}
	*pbuf--=0;
	if(*pbuf=='\r')*pbuf=0;
}

WORD CPage1::GetCommand(char *pbuf)
{
	char buf[50];
	
	if(*pbuf==0) return INITIAL_END_SEGMENT;
	sscanf(pbuf,"%s",buf);
	if(strstr(buf,"[InNet]")!=NULL) return INNET;
	else if(strstr(buf,"[OutNet]")!=NULL) return OUTNET;
	else if(strstr(buf,"[DM]")!=NULL)return DM;
	else if(buf[0]=='#') return COMMENT;
	else if(strstr(buf,"[jl]")!=NULL) return INITIAL_END_SEGMENT;
	else return COMMAND_UNKNOWN;
}


int CPage1::GetBestData()
{
	if(m_OutNetIp==m_InNetIp)
	{
		MessageBox("外网和内网有冲突,请重新选择");
		return 0;
	}
	if(m_OutNetIp==m_DmIp)
	{
		MessageBox("外网和DM有冲突，请重新选择");
		return 0;
	}
	if(m_InNetIp==m_DmIp)
	{
		MessageBox("内网和DM有冲突，请重新选择");
		return 0;
	}
	return 1;
}
