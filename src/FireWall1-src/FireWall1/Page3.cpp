
// Page3.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "Page3.h"
#include "global.h"
#include "page4.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
FILE *pfoutDefault=NULL;
FILE *pwriteDefault=NULL;
DefaultSetData TheDefaultSetData;
extern CDataConvert gDataConvert;
extern ServiceManageData TheServiceManageData,MyServiceManageData;
/////////////////////////////////////////////////////////////////////////////
// CPage3 property page

IMPLEMENT_DYNCREATE(CPage3, CPropertyPage)

CPage3::CPage3() : CPropertyPage(CPage3::IDD)
{
	//{{AFX_DATA_INIT(CPage3)
	m_check_activex = FALSE;
	m_check_comein = FALSE;
	m_check_dns = FALSE;
	m_check_finger = FALSE;
	m_check_goout = FALSE;
	m_check_intoout1024l = FALSE;
	m_check_intoout1024s = FALSE;
	m_check_intoout80 = FALSE;
	m_check_intooutftp = FALSE;
	m_check_intoouticmp = FALSE;
	m_check_intooutigmp = FALSE;
	m_check_intooutnbt = FALSE;
	m_check_intooutsnmp = FALSE;
	m_check_intoouttelnet = FALSE;
	m_check_java = FALSE;
	m_check_outtoin1024l = FALSE;
	m_check_outtoin1024s = FALSE;
	m_check_outtoin80 = FALSE;
	m_check_outtoinftp = FALSE;
	m_check_outtoinicmp = FALSE;
	m_check_outtoinigmp = FALSE;
	m_check_outtoinnbt = FALSE;
	m_check_outtoinsnmp = FALSE;
	m_check_outtointelnet = FALSE;
	m_check_script = FALSE;
	m_radio = -1;
	//}}AFX_DATA_INIT
}

CPage3::~CPage3()
{
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3)
	DDX_Check(pDX, IDC_CHECK_ACTIVEX, m_check_activex);
	DDX_Check(pDX, IDC_CHECK_COMEIN, m_check_comein);
	DDX_Check(pDX, IDC_CHECK_DNS, m_check_dns);
	DDX_Check(pDX, IDC_CHECK_FINGER, m_check_finger);
	DDX_Check(pDX, IDC_CHECK_GOOUT, m_check_goout);
	DDX_Check(pDX, IDC_CHECK_INtoOUT1024L, m_check_intoout1024l);
	DDX_Check(pDX, IDC_CHECK_INtoOUT1024S, m_check_intoout1024s);
	DDX_Check(pDX, IDC_CHECK_INtoOUT80, m_check_intoout80);
	DDX_Check(pDX, IDC_CHECK_INtoOUTFTP, m_check_intooutftp);
	DDX_Check(pDX, IDC_CHECK_INtoOUTICMP, m_check_intoouticmp);
	DDX_Check(pDX, IDC_CHECK_INtoOUTIGMP, m_check_intooutigmp);
	DDX_Check(pDX, IDC_CHECK_INtoOUTNBT, m_check_intooutnbt);
	DDX_Check(pDX, IDC_CHECK_INtoOUTSNMP, m_check_intooutsnmp);
	DDX_Check(pDX, IDC_CHECK_INtoOUTTELNET, m_check_intoouttelnet);
	DDX_Check(pDX, IDC_CHECK_JAVA, m_check_java);
	DDX_Check(pDX, IDC_CHECK_OUTtoIN1024L, m_check_outtoin1024l);
	DDX_Check(pDX, IDC_CHECK_OUTtoIN1024S, m_check_outtoin1024s);
	DDX_Check(pDX, IDC_CHECK_OUTtoIN80, m_check_outtoin80);
	DDX_Check(pDX, IDC_CHECK_OUTtoINFTP, m_check_outtoinftp);
	DDX_Check(pDX, IDC_CHECK_OUTtoINICMP, m_check_outtoinicmp);
	DDX_Check(pDX, IDC_CHECK_OUTtoINIGMP, m_check_outtoinigmp);
	DDX_Check(pDX, IDC_CHECK_OUTtoINNBT, m_check_outtoinnbt);
	DDX_Check(pDX, IDC_CHECK_OUTtoINSNMP, m_check_outtoinsnmp);
	DDX_Check(pDX, IDC_CHECK_OUTtoINTELNET, m_check_outtointelnet);
	DDX_Check(pDX, IDC_CHECK_SCRIPT, m_check_script);
	DDX_Radio(pDX, IDC_RADIO_HIGH, m_radio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_RADIO_HIGH, OnRadioHigh)
	ON_BN_CLICKED(IDC_RADIO_LOW, OnRadioLow)
	ON_BN_CLICKED(IDC_RADIO_MIDDLE, OnRadioMiddle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3 message handlers

void CPage3::OnButtonOk() 
{
	if(TheDefaultSetData.Priority&0x3)//表示高中
	{
		TheServiceManageData.TCPFilterBit|=0x7;
		MyServiceManageData.TCPFilterBit|=0x7;
	}
	else
	{
		TheServiceManageData.TCPFilterBit&=0xFFFFFFF8;
		MyServiceManageData.TCPFilterBit&=0xFFFFFFF8;
	}
		
    gDataConvert.globalFlags=m_globFlags;
	gDataConvert.m_alllinechar.sGlobalFlags=m_globFlags;
	pwriteDefault=fopen("default.txt","w");
	fprintf(pwriteDefault,_T("[Priority]\n"));
	fprintf(pwriteDefault,"%d\n",TheDefaultSetData.Priority);
	fprintf(pwriteDefault,"\n");
	fprintf(pwriteDefault,_T("[jl]\n"));
	if(pwriteDefault!=NULL)
		fclose(pwriteDefault);	
}

void CPage3::OnRadioHigh() 
{
	// TODO: Add your control notification handler code here
	m_radio=0;
	TheDefaultSetData.Priority=0x01;
	m_check_intoout80 = TRUE;
	m_check_activex = FALSE;
	m_check_comein = TRUE;
	m_check_dns = FALSE;
	m_check_finger = FALSE;
	m_check_goout = TRUE;
	m_check_intoout1024l = TRUE;
	m_check_intoout1024s = FALSE;
	m_check_intooutftp = FALSE;
	m_check_intoouticmp = 1;
	m_check_intooutigmp = 1;
	m_check_intooutnbt = FALSE;
	m_check_intooutsnmp = FALSE;
	m_check_intoouttelnet = FALSE;
	m_check_java = FALSE;
	m_check_outtoin1024l = FALSE;
	m_check_outtoin1024s = FALSE;
	m_check_outtoin80 =TRUE;
	m_check_outtoinftp = FALSE;
	m_check_outtoinicmp = 1;
	m_check_outtoinigmp = 1;
	m_check_outtoinnbt = FALSE;
	m_check_outtoinsnmp = FALSE;
	m_check_outtointelnet = FALSE;
	m_check_script = FALSE;	
	TheDefaultSetData.CheckINorOUTNetCard=0x3;
	TheDefaultSetData.PortocalComments=0x0;
	TheDefaultSetData.Pro=0xf;
	TheDefaultSetData.TCPProtocal=0x1003;
	TheDefaultSetData.UDPProtocal=0;
	memset(&m_globFlags,0,sizeof(struct GlobalFlags));
    m_globFlags.bFragmentIcmp=TRUE;
    m_globFlags.bSusPectIcmp=TRUE;
    m_globFlags.bCheckDmAccack=TRUE;
    m_globFlags.bCheckInNetAttack=TRUE;//IGMP
    m_globFlags.bCheckOutNetAccack=TRUE;
    m_globFlags.bSusPectOffset=TRUE;
    m_globFlags.bMulticast=TRUE;
	m_globFlags.bNonIpPacket=TRUE;
	m_globFlags.bOtherIpPacket=TRUE;
	m_globFlags.bforbidIcmp=TRUE;
	UpdateData(FALSE);	
}

void CPage3::OnRadioLow() 
{
	m_radio=2;
	TheDefaultSetData.Priority=0x04;
	m_check_intoout80 = TRUE;
	m_check_activex = TRUE;
	m_check_comein = TRUE;
	m_check_dns = TRUE;
	m_check_finger = FALSE;
	m_check_goout = FALSE;
	m_check_intoout1024l = TRUE;
	m_check_intoout1024s = TRUE;
	m_check_intooutftp = TRUE;
	m_check_intoouticmp = 0;
	m_check_intooutigmp = 0;
	m_check_intooutnbt = TRUE;
	m_check_intooutsnmp = TRUE;
	m_check_intoouttelnet = TRUE;
	m_check_java = TRUE;
	m_check_outtoin1024l = TRUE;
	m_check_outtoin1024s = TRUE;
	m_check_outtoin80 = TRUE;
	m_check_outtoinftp = TRUE;
	m_check_outtoinicmp = TRUE;
	m_check_outtoinigmp = TRUE;
	m_check_outtoinnbt = TRUE;
	m_check_outtoinsnmp = TRUE;
	m_check_outtointelnet = TRUE;
	m_check_script = TRUE;
	TheDefaultSetData.CheckINorOUTNetCard=0x2;
	TheDefaultSetData.PortocalComments=0x7;
	TheDefaultSetData.Pro=0xc;
	TheDefaultSetData.TCPProtocal=0xFFFFFBFF;
	TheDefaultSetData.UDPProtocal=0;
	memset(&m_globFlags,0,sizeof(struct GlobalFlags));
    m_globFlags.bFragmentIcmp=TRUE;
    m_globFlags.bSusPectIcmp=TRUE;
    m_globFlags.bCheckDmAccack=TRUE;
    m_globFlags.bCheckOutNetAccack=TRUE;
    m_globFlags.bSusPectOffset=TRUE;
	UpdateData(FALSE);	
}

void CPage3::OnRadioMiddle() 
{
	m_radio=1;
	TheDefaultSetData.Priority=0x02;
	m_check_intoout80 = TRUE;
	m_check_activex = FALSE;
	m_check_comein = TRUE;
	m_check_dns = FALSE;
	m_check_finger = FALSE;
	m_check_goout = TRUE;
	m_check_intoout1024l = TRUE;
	m_check_intoout1024s = TRUE;
	m_check_intooutftp = TRUE;
	m_check_intoouticmp = FALSE;
	m_check_intooutigmp = FALSE;
	m_check_intooutnbt = TRUE;
	m_check_intooutsnmp = TRUE;
	m_check_intoouttelnet = TRUE;
	m_check_java = FALSE;
	m_check_outtoin1024l = FALSE;
	m_check_outtoin1024s = TRUE;
	m_check_outtoin80 = TRUE;
	m_check_outtoinftp = TRUE;
	m_check_outtoinicmp = 1;
	m_check_outtoinigmp = 1;
	m_check_outtoinnbt = TRUE;
	m_check_outtoinsnmp = TRUE;
	m_check_outtointelnet = TRUE;
	m_check_script = FALSE;	
	TheDefaultSetData.CheckINorOUTNetCard=0x3;
	TheDefaultSetData.PortocalComments=0x0;
	TheDefaultSetData.Pro=0xc;
	TheDefaultSetData.TCPProtocal=0xFFFFDBFF;
	TheDefaultSetData.UDPProtocal=0;
	memset(&m_globFlags,0,sizeof(struct GlobalFlags));
    m_globFlags.bFragmentIcmp=TRUE;
    m_globFlags.bSusPectIcmp=TRUE;
    m_globFlags.bCheckDmAccack=TRUE;
    m_globFlags.bCheckInNetAttack=TRUE;//IGMP
    m_globFlags.bCheckOutNetAccack=TRUE;
    m_globFlags.bSusPectOffset=TRUE;
    m_globFlags.bMulticast=TRUE;
	UpdateData(FALSE);	
}

BOOL CPage3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	BOOL click=FALSE;
	// TODO: Add extra initialization here
//	GetDataFromFile();
	if(TheDefaultSetData.Priority==0x1)
	{
		m_radio=0;
		OnRadioHigh();
	}
	if(TheDefaultSetData.Priority==0x2)
	{
		m_radio=1;
		OnRadioMiddle();
	}
	if(TheDefaultSetData.Priority==0x4)
	{
		m_radio=2;
		OnRadioLow();
	}
	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
DWORD CPage3::GetDataFromFile()
{
	char command[40];
	char *pbuf;
	pfoutDefault=fopen("default.txt","r");
	if(pfoutDefault==NULL)return 0;
	while(feof(pfoutDefault)==0)
	{
		ReadCommand(pfoutDefault,command);
		switch(GetCommand(command))
		{
		case PRIORITY:
			ReadCommand(pfoutDefault,command);
			sscanf(command,"%d",&TheDefaultSetData.Priority);
			if(TheDefaultSetData.Priority==0x01)
				m_radio=0;
			if(TheDefaultSetData.Priority==0x02)
				m_radio=1;
			if(TheDefaultSetData.Priority==0x04)
				m_radio=2;
			break;
		case CHECKINOROUTNETCARD:
				break;
		case PRO:
			break;
		case TCPPROTOCAL:
			break;
		case PORTOCALCOMMENTS:
			break;
		case UDPPROTOCAL:
			break;
		}
	}
	return 1;

}

void CPage3::ReadCommand(FILE *pf, char *pbuf)
{
	while(feof(pf)==0) {
		*pbuf=fgetc(pf);
		if(*pbuf=='\n') break;
		pbuf++;
	}
	*pbuf--=0;
	if(*pbuf=='\r')*pbuf=0;
}

DWORD CPage3::GetCommand(char *pbuf)
{
	char buf[50];
	if(*pbuf==0) return INITIAL_END_SEGMENT;
	sscanf(pbuf,"%s",buf);
	if(strstr(buf,"[Priority]")!=NULL) return PRIORITY;
	else if(strstr(buf,"[CheckINorOUTNetCard]")!=NULL) return CHECKINOROUTNETCARD;
	else if(strstr(buf,"[Pro]")!=NULL)return PRO;
	else if(strstr(buf,"[TCPProtocal]")!=NULL) return TCPPROTOCAL;
	else if(strstr(buf,"[PortocalComments]")!=NULL) return PORTOCALCOMMENTS;
	else if(strstr(buf,"[UDPProtocal]")!=NULL) return UDPPROTOCAL;
	else if(strstr(buf,"[jl]")!=NULL) return INITIAL_END_SEGMENT;
	else return COMMAND_UNKNOWN;
}

