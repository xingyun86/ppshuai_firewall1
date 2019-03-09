
// Page6.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "Page6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
FILE *pfoutLogset=NULL;
FILE *pwriteLogset=NULL;
LogManage MyLogManage;
/////////////////////////////////////////////////////////////////////////////
// CPage6 property page

IMPLEMENT_DYNCREATE(CPage6, CPropertyPage)

CPage6::CPage6() : CPropertyPage(CPage6::IDD)
{
	//{{AFX_DATA_INIT(CPage6)
	m_check_incommingdos = FALSE;
	m_check_incommingport = FALSE;
	m_check_incommingip = FALSE;
	m_check_incommingtype = FALSE;
	m_check_islog = 1;
	m_check_logip = FALSE;
	m_check_logipport = FALSE;
	m_check_logkind = FALSE;
	m_check_logtime = FALSE;
	m_check_logurl = FALSE;
	m_check_outgoingdos = FALSE;
	m_check_outgoingip = FALSE;
	m_check_outgoingport = FALSE;
	m_check_outgoingtype = FALSE;
	m_check_prolog = FALSE;
	m_check_inbroad = FALSE;
	m_check_inlength = FALSE;
	m_check_inmore = FALSE;
	m_check_inoffset = FALSE;
	m_check_outbroad = FALSE;
	m_check_outlength = FALSE;
	m_check_outmore = FALSE;
	m_check_outoffset = FALSE;
	m_check_route = FALSE;
	//}}AFX_DATA_INIT
}

CPage6::~CPage6()
{
}

void CPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage6)
	DDX_Check(pDX, IDC_CHECK_incommingDOS, m_check_incommingdos);
	DDX_Check(pDX, IDC_CHECK_incommingPort, m_check_incommingport);
	DDX_Check(pDX, IDC_CHECK_incommingIP, m_check_incommingip);
	DDX_Check(pDX, IDC_CHECK_incommingType, m_check_incommingtype);
	DDX_Check(pDX, IDC_CHECK_ISLOG, m_check_islog);
	DDX_Check(pDX, IDC_CHECK_LOGIP, m_check_logip);
	DDX_Check(pDX, IDC_CHECK_LOGIPPORT, m_check_logipport);
	DDX_Check(pDX, IDC_CHECK_LOGKIND, m_check_logkind);
	DDX_Check(pDX, IDC_CHECK_LOGTIME, m_check_logtime);
	DDX_Check(pDX, IDC_CHECK_LOGURL, m_check_logurl);
	DDX_Check(pDX, IDC_CHECK_outgoingDOS, m_check_outgoingdos);
	DDX_Check(pDX, IDC_CHECK_outgoingIP, m_check_outgoingip);
	DDX_Check(pDX, IDC_CHECK_outgoingPort, m_check_outgoingport);
	DDX_Check(pDX, IDC_CHECK_outgoingType, m_check_outgoingtype);
	DDX_Check(pDX, IDC_CHECK_PROLOG, m_check_prolog);
	DDX_Check(pDX, IDC_CHECK_InBroad, m_check_inbroad);
	DDX_Check(pDX, IDC_CHECK_InLength, m_check_inlength);
	DDX_Check(pDX, IDC_CHECK_InMore, m_check_inmore);
	DDX_Check(pDX, IDC_CHECK_InOffset, m_check_inoffset);
	DDX_Check(pDX, IDC_CHECK_OutBroad, m_check_outbroad);
	DDX_Check(pDX, IDC_CHECK_OutLength, m_check_outlength);
	DDX_Check(pDX, IDC_CHECK_OutMore, m_check_outmore);
	DDX_Check(pDX, IDC_CHECK_OutOffset, m_check_outoffset);
	DDX_Check(pDX, IDC_CHECK_SORCE_ROUTE, m_check_route);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage6, CPropertyPage)
	//{{AFX_MSG_MAP(CPage6)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_CHECK_incommingDOS, OnCHECKincommingDOS)
	ON_BN_CLICKED(IDC_CHECK_incommingIP, OnCHECKincommingIP)
	ON_BN_CLICKED(IDC_CHECK_incommingPort, OnCHECKincommingPort)
	ON_BN_CLICKED(IDC_CHECK_incommingType, OnCHECKincommingType)
	ON_BN_CLICKED(IDC_CHECK_ISLOG, OnCheckIslog)
	ON_BN_CLICKED(IDC_CHECK_LOGIP, OnCheckLogip)
	ON_BN_CLICKED(IDC_CHECK_LOGIPPORT, OnCheckLogipport)
	ON_BN_CLICKED(IDC_CHECK_LOGKIND, OnCheckLogkind)
	ON_BN_CLICKED(IDC_CHECK_LOGTIME, OnCheckLogtime)
	ON_BN_CLICKED(IDC_CHECK_LOGURL, OnCheckLogurl)
	ON_BN_CLICKED(IDC_CHECK_outgoingDOS, OnCHECKoutgoingDOS)
	ON_BN_CLICKED(IDC_CHECK_outgoingIP, OnCHECKoutgoingIP)
	ON_BN_CLICKED(IDC_CHECK_outgoingPort, OnCHECKoutgoingPort)
	ON_BN_CLICKED(IDC_CHECK_outgoingType, OnCHECKoutgoingType)
	ON_BN_CLICKED(IDC_CHECK_PROLOG, OnCheckProlog)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_CHECK_InBroad, OnCHECKInBroad)
	ON_BN_CLICKED(IDC_CHECK_OutBroad, OnCHECKOutBroad)
	ON_BN_CLICKED(IDC_CHECK_InLength, OnCHECKInLength)
	ON_BN_CLICKED(IDC_CHECK_OutLength, OnCHECKOutLength)
	ON_BN_CLICKED(IDC_CHECK_InOffset, OnCHECKInOffset)
	ON_BN_CLICKED(IDC_CHECK_OutOffset, OnCHECKOutOffset)
	ON_BN_CLICKED(IDC_CHECK_InMore, OnCHECKInMore)
	ON_BN_CLICKED(IDC_CHECK_OutMore, OnCHECKOutMore)
	ON_BN_CLICKED(IDC_CHECK_SORCE_ROUTE, OnCheckSorceRoute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage6 message handlers
DWORD CPage6::GetDataFromFile()
{
	char command[40];
	char *pbuf;
	pfoutLogset=fopen("logset.txt","r");
	if(pfoutLogset==NULL)return 0;
	while(feof(pfoutLogset)==0)
	{
		ReadCommand(pfoutLogset,command);
		switch(GetCommand(command))
		{
		case REQUIRELOG:
			ReadCommand(pfoutLogset,command);
			sscanf(command,"%d",&MyLogManage.RequireLog);
			if(MyLogManage.RequireLog&0x01)
				m_check_islog=TRUE;
			else m_check_islog=FALSE;
			break;
		case ISLOGIP:
			ReadCommand(pfoutLogset,command);
			sscanf(command,"%d",&MyLogManage.TheIpFilter.isLogIp);
			if(MyLogManage.TheIpFilter.isLogIp&0x01)
				m_check_logip=TRUE;
			else m_check_logip=FALSE;
			break;
		case IPOPTIONS:
			ReadCommand(pfoutLogset,command);
			sscanf(command,"%d",&MyLogManage.TheIpFilter.Options);
			if(MyLogManage.TheIpFilter.Options&0x01)
				m_check_inbroad=1;
			else m_check_inbroad=0;
			if(MyLogManage.TheIpFilter.Options&0x02)
				m_check_outbroad=TRUE;
			else m_check_outbroad=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x04)
				m_check_incommingport=1;
			else m_check_incommingport=0;
			if(MyLogManage.TheIpFilter.Options&0x08)
				m_check_outgoingport=TRUE;
			else m_check_outgoingport=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x10)
				m_check_inlength=1;
			else m_check_inlength=0;
			if(MyLogManage.TheIpFilter.Options&0x20)
				m_check_outlength=1;
			else m_check_outlength=0;
			if(MyLogManage.TheIpFilter.Options&0x40)
				m_check_incommingtype=TRUE;
			else m_check_incommingtype=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x80)
				m_check_outgoingtype=TRUE;
			else m_check_outgoingtype=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x100)
				m_check_incommingip=TRUE;
			else m_check_incommingip=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x200)
				m_check_outgoingip=TRUE;
			else m_check_outgoingip=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x1000)
				m_check_inoffset=TRUE;
			else m_check_inoffset=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x2000)
				m_check_outoffset=TRUE;
			else m_check_outoffset=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x4000)
				m_check_inmore=TRUE;
			else m_check_inmore=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x8000)
				m_check_outmore=TRUE;
			else m_check_outmore=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x10000)
				m_check_incommingdos=TRUE;
			else m_check_incommingdos=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x20000)
				m_check_outgoingdos=TRUE;
			else m_check_outgoingdos=FALSE;
			if(MyLogManage.TheIpFilter.Options&0x40000)
				m_check_route=TRUE;
			else m_check_route=FALSE;
			break;
		case ISLOGPROTOCAL:
			ReadCommand(pfoutLogset,command);
			sscanf(command,"%d",&MyLogManage.TheProCommentsLog.isLogProtocal);
			m_check_prolog=MyLogManage.TheProCommentsLog.isLogProtocal;
			break;
		case PROOPTIONS:
			ReadCommand(pfoutLogset,command);
			sscanf(command,"%d",&MyLogManage.TheProCommentsLog.Options);
			if(MyLogManage.TheProCommentsLog.Options&0x01)
				m_check_logipport=TRUE;
			else m_check_logipport=FALSE;
			if(MyLogManage.TheProCommentsLog.Options&0x02)
				m_check_logkind=TRUE;
			else m_check_logkind=FALSE;
			if(MyLogManage.TheProCommentsLog.Options&0x04)
				m_check_logurl=TRUE;
			else m_check_logurl=FALSE;
			if(MyLogManage.TheProCommentsLog.Options&0x08)
				m_check_logtime=TRUE;
			else m_check_logtime=FALSE;
			break;	
		}
	}
	if(pfoutLogset)fclose(pfoutLogset);
	return 1;	
}

void CPage6::ReadCommand(FILE *pf, char *pbuf)
{
	while(feof(pf)==0) {
		*pbuf=fgetc(pf);
		if(*pbuf=='\n') break;
		pbuf++;
	}
	*pbuf--=0;
	if(*pbuf=='\r')*pbuf=0;
}

DWORD CPage6::GetCommand(char *pbuf)
{
	char buf[50];
	
	if(*pbuf==0) return INITIAL_END_SEGMENT;
	sscanf(pbuf,"%s",buf);
	if(strstr(buf,"[RequireLog]")!=NULL) return REQUIRELOG;
	else if(strstr(buf,"[isLogIp]")!=NULL) return ISLOGIP;
	else if(strstr(buf,"[ipOptions]")!=NULL)return IPOPTIONS;
	else if(strstr(buf,"[isLogProtocal]")!=NULL)return ISLOGPROTOCAL;
	else if(strstr(buf,"[proOptions]")!=NULL)return PROOPTIONS;
	else if(strstr(buf,"[jl]")!=NULL) return INITIAL_END_SEGMENT;
	else return COMMAND_UNKNOWN;
}
void CPage6::OnButtonOk() 
{
	
	pwriteLogset=fopen("logset.txt","w");
	if(pwriteLogset)
	{
		fprintf(pwriteLogset,_T("[RequireLog]\n"));
		fprintf(pwriteLogset,"%d\n",MyLogManage.RequireLog);
		fprintf(pwriteLogset,"\n");
		fprintf(pwriteLogset,_T("[isLogIp]\n"));
		fprintf(pwriteLogset,"%d\n",MyLogManage.TheIpFilter.isLogIp);
		fprintf(pwriteLogset,"\n");
		fprintf(pwriteLogset,_T("[ipOptions]\n"));
		fprintf(pwriteLogset,"%d\n",MyLogManage.TheIpFilter.Options);
		fprintf(pwriteLogset,"\n");
		fprintf(pwriteLogset,_T("[isLogProtocal]\n"));
		fprintf(pwriteLogset,"%d\n",MyLogManage.TheProCommentsLog.isLogProtocal);
		fprintf(pwriteLogset,"\n");
		fprintf(pwriteLogset,_T("[proOptions]\n"));
		fprintf(pwriteLogset,"%d\n",MyLogManage.TheProCommentsLog.Options);
		fprintf(pwriteLogset,"\n");
		fprintf(pwriteLogset,_T("[jl]\n"));
		if(pwriteLogset!=NULL)
			fclose(pwriteLogset);	
	}
}
void CPage6::ShowInitWindow()
{
	if(pfoutLogset)
	{
		if(!m_check_logip)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InBroad),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutBroad),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InLength),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutLength),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InOffset),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutOffset),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InMore),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutMore),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_IPCHOOSE),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingPort),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingPort),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingType),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingType),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingDOS),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingDOS),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingIP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingIP),SW_HIDE);
		
		}
		if(!m_check_prolog)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_PROCHOOSE),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGIPPORT),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGKIND),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGURL),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGTIME),SW_HIDE);
		}
	}

}

void CPage6::OnCHECKincommingDOS() 
{
	if(m_check_incommingdos)
	{
		m_check_incommingdos=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFEFFFF;
	}
	else
	{
		m_check_incommingdos=TRUE;
		MyLogManage.TheIpFilter.Options|=0x10000;
	}	
}

void CPage6::OnCHECKincommingIP() 
{
	if(m_check_incommingip)
	{
		m_check_incommingip=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFFFeff;
	}
	else
	{
		m_check_incommingip=TRUE;
		MyLogManage.TheIpFilter.Options|=0x100;
	}	
}

void CPage6::OnCHECKincommingPort() 
{
	if(m_check_incommingport)
	{
		m_check_incommingport=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFFFFFb;
	}
	else
	{
		m_check_incommingport=TRUE;
		MyLogManage.TheIpFilter.Options|=0x04;
	}	
}

void CPage6::OnCHECKincommingType() 
{
	if(m_check_incommingtype)
	{
		m_check_incommingtype=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFFFFBF;
	}
	else
	{
		m_check_incommingtype=TRUE;
		MyLogManage.TheIpFilter.Options|=0x40;
	}	
}

void CPage6::OnCheckIslog() 
{
	// TODO: Add your control notification handler code here
	
}

void CPage6::OnCheckLogip() 
{
	if(m_check_logip)
	{
		m_check_logip=FALSE;
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_IPCHOOSE),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InBroad),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutBroad),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InLength),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutLength),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InOffset),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutOffset),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InMore),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutMore),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingPort),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingPort),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingType),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingType),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingDOS),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingDOS),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingIP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingIP),SW_HIDE);
	}
	else
	{
		m_check_logip=TRUE;
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_IPCHOOSE),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InBroad),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutBroad),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InLength),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutLength),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InOffset),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutOffset),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_InMore),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OutMore),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingPort),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingPort),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingType),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingType),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingDOS),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingDOS),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_incommingIP),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_outgoingIP),SW_SHOWNORMAL);
	}
	MyLogManage.TheIpFilter.isLogIp=m_check_logip;
			
}

void CPage6::OnCheckLogipport() 
{
	if(m_check_logipport)
	{
		m_check_logipport=FALSE;
		MyLogManage.TheProCommentsLog.Options&=0xfffffffe;
	}
	else
	{
		m_check_logipport=TRUE;
		MyLogManage.TheProCommentsLog.Options|=0x1;
	}	
}

void CPage6::OnCheckLogkind() 
{
	if(m_check_logkind)
	{
		m_check_logkind=FALSE;
		MyLogManage.TheProCommentsLog.Options&=0xfffffffd;
	}
	else
	{
		m_check_logkind=TRUE;
		MyLogManage.TheProCommentsLog.Options|=0x2;
	}	
}

void CPage6::OnCheckLogtime() 
{
	if(m_check_logtime)
	{
		m_check_logtime=FALSE;
		MyLogManage.TheProCommentsLog.Options&=0xfffffff7;
	}
	else
	{
		m_check_logtime=TRUE;
		MyLogManage.TheProCommentsLog.Options|=0x8;
	}	
}

void CPage6::OnCheckLogurl() 
{
	if(m_check_logurl)
	{
		m_check_logurl=FALSE;
		MyLogManage.TheProCommentsLog.Options&=0xfffffffb;
	}
	else
	{
		m_check_logurl=TRUE;
		MyLogManage.TheProCommentsLog.Options|=0x4;
	}	
}

void CPage6::OnCHECKoutgoingDOS() 
{
	// TODO: Add your control notification handler code here
	if(m_check_outgoingdos)
	{
		m_check_outgoingdos=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFDFFFF;
	}
	else
	{
		m_check_outgoingdos=TRUE;
		MyLogManage.TheIpFilter.Options|=0x20000;
	}	
}

void CPage6::OnCHECKoutgoingIP() 
{
	if(m_check_outgoingip)
	{
		m_check_outgoingip=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFFFdFF;
	}
	else
	{
		m_check_outgoingip=TRUE;
		MyLogManage.TheIpFilter.Options|=0x200;
	}	
}

void CPage6::OnCHECKoutgoingPort() 
{
	if(m_check_outgoingport)
	{
		m_check_outgoingport=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFFFFF7;
	}
	else
	{
		m_check_outgoingport=TRUE;
		MyLogManage.TheIpFilter.Options|=0x08;
	}	
}

void CPage6::OnCHECKoutgoingType() 
{
	if(m_check_outgoingtype)
	{
		m_check_outgoingtype=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFFFF7f;
	}
	else
	{
		m_check_outgoingtype=TRUE;
		MyLogManage.TheIpFilter.Options|=0x80;
	}	
}

void CPage6::OnCheckProlog() 
{
	if(m_check_prolog)
	{
		m_check_prolog=FALSE;
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_PROCHOOSE),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGIPPORT),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGKIND),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGURL),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGTIME),SW_HIDE);
	}
	else
	{
		m_check_prolog=TRUE;
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_PROCHOOSE),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGIPPORT),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGKIND),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGURL),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_LOGTIME),SW_SHOWNORMAL);
	}
	MyLogManage.TheProCommentsLog.isLogProtocal=m_check_prolog;	
}



BOOL CPage6::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_check_islog=1;
	UpdateData(1);

	GetData();
	UpdateData(FALSE);
	ShowInitWindow();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage6::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPage6::OnCHECKInBroad() 
{
	// TODO: Add your control notification handler code here
	if(m_check_inbroad)
	{
		m_check_inbroad=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFFFFFE;
	}
	else
	{
		m_check_inbroad=1;
		MyLogManage.TheIpFilter.Options|=0x01;
	}

}

void CPage6::OnCHECKOutBroad() 
{
	// TODO: Add your control notification handler code here
	if(m_check_outbroad)
	{
		m_check_outbroad=FALSE;
		MyLogManage.TheIpFilter.Options&=0xFFFFFFFd;
	}
	else
	{
		m_check_outbroad=1;
		MyLogManage.TheIpFilter.Options|=0x02;
	}
}

void CPage6::OnCHECKInLength() 
{
	// TODO: Add your control notification handler code here
	if(m_check_inlength)
	{
		m_check_inlength=0;
		MyLogManage.TheIpFilter.Options&=0xFFFFFFEF;
	}
	else
	{
		m_check_inlength=1;
		MyLogManage.TheIpFilter.Options|=0x10;
	}

}

void CPage6::OnCHECKOutLength() 
{
	// TODO: Add your control notification handler code here
	if(m_check_outlength)
	{
		m_check_outlength=0;
		MyLogManage.TheIpFilter.Options&=0xFFFFFFdF;
	}
	else
	{
		m_check_outlength=1;
		MyLogManage.TheIpFilter.Options|=0x20;
	}	
}

void CPage6::OnCHECKInOffset() 
{
	// TODO: Add your control notification handler code here
	if(m_check_inoffset)
	{
		m_check_inoffset=0;
		MyLogManage.TheIpFilter.Options&=0xFFFFEFFF;
	}
	else
	{
		m_check_inoffset=1;
		MyLogManage.TheIpFilter.Options|=0x1000;
	}

}

void CPage6::OnCHECKOutOffset() 
{
	// TODO: Add your control notification handler code here
	if(m_check_outoffset)
	{
		m_check_outoffset=0;
		MyLogManage.TheIpFilter.Options&=0xFFFFdFFF;
	}
	else
	{
		m_check_outoffset=1;
		MyLogManage.TheIpFilter.Options|=0x2000;
	}	
}

void CPage6::OnCHECKInMore() 
{
	// TODO: Add your control notification handler code here
	if(m_check_inmore)
	{
		m_check_inmore=0;
		MyLogManage.TheIpFilter.Options&=0xFFFFbFFF;
	}
	else
	{
		m_check_inmore=1;
		MyLogManage.TheIpFilter.Options|=0x4000;
	}	
}

void CPage6::OnCHECKOutMore() 
{
	// TODO: Add your control notification handler code here
	if(m_check_outmore)
	{
		m_check_outmore=0;
		MyLogManage.TheIpFilter.Options&=0xFFFF7FFF;
	}
	else
	{
		m_check_outmore=1;
		MyLogManage.TheIpFilter.Options|=0x8000;
	}	
}

void CPage6::OnCheckSorceRoute() 
{
	if(m_check_route)
	{
		m_check_route=0;
		MyLogManage.TheIpFilter.Options&=0xFFBFFFF;
	}
	else
	{
		m_check_route=1;
		MyLogManage.TheIpFilter.Options|=0x40000;
	}	
}

void CPage6::GetData()
{
	if(MyLogManage.RequireLog&0x01)
		m_check_islog=TRUE;
	else m_check_islog=FALSE;
	if(MyLogManage.TheIpFilter.isLogIp&0x01)m_check_logip=TRUE;
	else m_check_logip=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x01)
		m_check_inbroad=1;
	else m_check_inbroad=0;
	if(MyLogManage.TheIpFilter.Options&0x02)
		m_check_outbroad=TRUE;
	else m_check_outbroad=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x04)
		m_check_incommingport=1;
	else m_check_incommingport=0;
	if(MyLogManage.TheIpFilter.Options&0x08)
		m_check_outgoingport=TRUE;
	else m_check_outgoingport=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x10)
		m_check_inlength=1;
	else m_check_inlength=0;
	if(MyLogManage.TheIpFilter.Options&0x20)
		m_check_outlength=1;
	else m_check_outlength=0;
	if(MyLogManage.TheIpFilter.Options&0x40)
		m_check_incommingtype=TRUE;
	else m_check_incommingtype=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x80)
		m_check_outgoingtype=TRUE;
	else m_check_outgoingtype=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x100)
		m_check_incommingip=TRUE;
	else m_check_incommingip=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x200)
		m_check_outgoingip=TRUE;
	else m_check_outgoingip=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x1000)
		m_check_inoffset=TRUE;
	else m_check_inoffset=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x2000)
		m_check_outoffset=TRUE;
	else m_check_outoffset=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x4000)
		m_check_inmore=TRUE;
	else m_check_inmore=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x8000)
		m_check_outmore=TRUE;
	else m_check_outmore=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x10000)
		m_check_incommingdos=TRUE;
	else m_check_incommingdos=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x20000)
		m_check_outgoingdos=TRUE;
	else m_check_outgoingdos=FALSE;
	if(MyLogManage.TheIpFilter.Options&0x40000)
		m_check_route=TRUE;
	else m_check_route=FALSE;
	m_check_prolog=MyLogManage.TheProCommentsLog.isLogProtocal;
	if(MyLogManage.TheProCommentsLog.Options&0x01)
		m_check_logipport=TRUE;
	else m_check_logipport=FALSE;
	if(MyLogManage.TheProCommentsLog.Options&0x02)
		m_check_logkind=TRUE;
	else m_check_logkind=FALSE;
	if(MyLogManage.TheProCommentsLog.Options&0x04)
		m_check_logurl=TRUE;
	else m_check_logurl=FALSE;
	if(MyLogManage.TheProCommentsLog.Options&0x08)
		m_check_logtime=TRUE;
	else m_check_logtime=FALSE;
}

BOOL CPage6::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetData();
	UpdateData(0);
	return CPropertyPage::OnSetActive();
}
