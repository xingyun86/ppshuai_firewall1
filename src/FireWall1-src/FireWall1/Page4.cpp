
// Page4.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "Page4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern int global_set;
FILE *pfoutService=NULL;
FILE *pwriteService=NULL;
ServiceManageData MyServiceManageData;
CString m_str_key;
BOOL nook4;
/////////////////////////////////////////////////////////////////////////////
// CPage4 property page

IMPLEMENT_DYNCREATE(CPage4, CPropertyPage)

CPage4::CPage4() : CPropertyPage(CPage4::IDD)
{
	//{{AFX_DATA_INIT(CPage4)
	m_str_key="";
	m_check_activex = FALSE;
	m_check_ftp = FALSE;
	m_check_http = FALSE;
	m_check_ispro = FALSE;
	m_check_isurl = FALSE;
	m_check_java = FALSE;
	m_check_other = FALSE;
	m_check_script = FALSE;
	m_check_smtp = FALSE;
	m_check_tcp = FALSE;
	m_check_telnet = FALSE;
	m_check_udp = FALSE;
	m_edit_nofilterurl = _T("");
	m_edit_otherkey = _T("");
	m_edit_url = _T("");
	nook4=1;
	//}}AFX_DATA_INIT
}

CPage4::~CPage4()
{
	if(nook4)
	{
/*		GetNameFromFile();
		memset(MyServiceManageData.FilterKeyWord,0,200);
		strcpy(MyServiceManageData.FilterKeyWord,m_edit_otherkey);
*/
	}
}

void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage4)
	DDX_Control(pDX, IDC_EDIT_URL, m_edit_urlct);
	DDX_Control(pDX, IDC_EDIT_OTHERKEY, m_edit_otherkeyct);
	DDX_Control(pDX, IDC_EDIT_NOFILTERURL, m_edit_nofilterurlct);
	DDX_Check(pDX, IDC_CHECK_ACTIVEX, m_check_activex);
	DDX_Check(pDX, IDC_CHECK_FTP, m_check_ftp);
	DDX_Check(pDX, IDC_CHECK_HTTP, m_check_http);
	DDX_Check(pDX, IDC_CHECK_ISPRO, m_check_ispro);
	DDX_Check(pDX, IDC_CHECK_ISURL, m_check_isurl);
	DDX_Check(pDX, IDC_CHECK_JAVA, m_check_java);
	DDX_Check(pDX, IDC_CHECK_OTHER, m_check_other);
	DDX_Check(pDX, IDC_CHECK_SCRIPT, m_check_script);
	DDX_Check(pDX, IDC_CHECK_SMTP, m_check_smtp);
	DDX_Check(pDX, IDC_CHECK_TCP, m_check_tcp);
	DDX_Check(pDX, IDC_CHECK_TELNET, m_check_telnet);
	DDX_Check(pDX, IDC_CHECK_UDP, m_check_udp);
	DDX_Text(pDX, IDC_EDIT_NOFILTERURL, m_edit_nofilterurl);
	DDX_Text(pDX, IDC_EDIT_OTHERKEY, m_edit_otherkey);
	DDX_Text(pDX, IDC_EDIT_URL, m_edit_url);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage4, CPropertyPage)
	//{{AFX_MSG_MAP(CPage4)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_CHECK_ACTIVEX, OnCheckActivex)
	ON_BN_CLICKED(IDC_CHECK_HTTP, OnCheckHttp)
	ON_BN_CLICKED(IDC_CHECK_ISPRO, OnCheckIspro)
	ON_BN_CLICKED(IDC_CHECK_ISURL, OnCheckIsurl)
	ON_BN_CLICKED(IDC_CHECK_JAVA, OnCheckJava)
	ON_BN_CLICKED(IDC_CHECK_OTHER, OnCheckOther)
	ON_BN_CLICKED(IDC_CHECK_SCRIPT, OnCheckScript)
	ON_BN_CLICKED(IDC_CHECK_TCP, OnCheckTcp)
	ON_BN_CLICKED(IDC_CHECK_SMTP, OnCheckSmtp)
	ON_BN_CLICKED(IDC_CHECK_TELNET, OnCheckTelnet)
	ON_BN_CLICKED(IDC_CHECK_UDP, OnCheckUdp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage4 message handlers

void CPage4::OnButtonOk() 
{
	nook4=0;
	UpdateData(1);
	if((m_edit_nofilterurl.GetLength()>2)&&(m_edit_url.GetLength()>2)&&m_check_isurl)
	{
		if(GetBestData())
		{
			AfxMessageBox("URL 有冲突，请重新配置!");
			return;
		}
	}


	m_str_key="";
	if(m_check_java)m_str_key+="applet\r\n";
	if(m_check_activex)m_str_key+="object\r\n";
	if(m_check_script)m_str_key+="script\r\n";
	

	global_set|=0x4;
	
	//下面上尚有BUG，对EDIT框的回车没有很好自动修正
	char tempbuf[200];
	memset(tempbuf,0,10);
	int len;
	len=m_edit_otherkey.GetLength();
	if(len>2)
	{

		strcpy(tempbuf,_T(m_edit_otherkey));

		if(tempbuf[len-1]!='\n')
		{
			m_edit_otherkey+="\r\n";
		}
		
	}
	else m_edit_otherkey="";
	UpdateData(TRUE);
	if(m_check_other)m_str_key+=m_edit_otherkey;
	memset(tempbuf,0,200);
	len=m_edit_url.GetLength();
	if(len>0)
	{

		strcpy(tempbuf,_T(m_edit_url));

		if(tempbuf[len-1]!='\n')
		{
			m_edit_url+="\r\n";
		}
	}
	memset(tempbuf,0,200);
	len=m_edit_nofilterurl.GetLength();
	if(len>0)
	{

		strcpy(tempbuf,_T(m_edit_nofilterurl));

		if(tempbuf[len-1]!='\n')
		{
			m_edit_nofilterurl+="\r\n";
		}
	}
/*	GetDlgItem(IDC_EDIT_URL)->UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_NOFILTERURL)->UpdateData(TRUE);
	*/
	
	strcpy(MyServiceManageData.FilterKeyWord,m_str_key);
	strcpy(MyServiceManageData.BlockURLList,m_edit_url);
	strcpy(MyServiceManageData.NoFilterURLList,m_edit_nofilterurl);
	pwriteService=fopen("service.txt","w");
	if(pwriteService)
	{
		fprintf(pwriteService,_T("[FilterAndBlock]\n"));
		fprintf(pwriteService,"%d\n",MyServiceManageData.FilterAndBlock);
		fprintf(pwriteService,"\n");
		fprintf(pwriteService,_T("[Port]\n"));
		fprintf(pwriteService,"%d\n",MyServiceManageData.Port);
		fprintf(pwriteService,"\n");
		fprintf(pwriteService,_T("[TranProtocal]\n"));
		fprintf(pwriteService,"%d\n",MyServiceManageData.TranProtocal);
		fprintf(pwriteService,"\n");
		fprintf(pwriteService,_T("[FilterKeyWords]\n"));
	//	fprintf(pwriteService,"%s\n",MyServiceManageData.FilterKeyWord);
		fprintf(pwriteService,"%s\n",_T(m_str_key));
	//	fprintf(pwriteService,"\r\n");
		fprintf(pwriteService,_T("[TCPFilterBit]\n"));
		fprintf(pwriteService,"%d\n",MyServiceManageData.TCPFilterBit);
		fprintf(pwriteService,"\n");
		fprintf(pwriteService,_T("[BlockUrl]\n"));
		fprintf(pwriteService,"%s\n",_T(m_edit_url));
	//	fprintf(pwriteService,"\r\n");
		fprintf(pwriteService,_T("[NoFilterURL]\r\n"));
		fprintf(pwriteService,"%s\n",_T(m_edit_nofilterurl));
	//	fprintf(pwriteService,"\r\n");
		fprintf(pwriteService,_T("[jl]\n"));
		if(pwriteService!=NULL)
			fclose(pwriteService);	
	}
}

void CPage4::OnCheckActivex() 
{
	if(m_check_activex)
	{
		m_check_activex=FALSE;
		MyServiceManageData.TCPFilterBit&=0xFFFFFFfb;
	}
	else
	{
		m_check_activex=TRUE;
		MyServiceManageData.TCPFilterBit|=0x04;
	}		
}

void CPage4::OnCheckHttp() 
{
	if(m_check_http)
	{
		m_check_http=FALSE;
		MyServiceManageData.Port&=0xFFFFFFfe;
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_JAVA),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_HTTP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SCRIPT),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_ACTIVEX),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_FILTERURL),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_NOFILTERURL),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OTHER),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_HIDE);
		
	}
	else
	{
		m_check_http=TRUE;
		MyServiceManageData.Port|=0x01;
		if(m_check_other)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_SHOWNORMAL);
			::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_SHOWNORMAL);
		}
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_JAVA),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_HTTP),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SCRIPT),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_ACTIVEX),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_FILTERURL),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_NOFILTERURL),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OTHER),SW_SHOWNORMAL);
	}
			
}

void CPage4::OnCheckIspro() 
{
	if(m_check_ispro==TRUE)
	{
		m_check_ispro=FALSE;
		MyServiceManageData.FilterAndBlock&=0xFFFFFFFE;
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_TCP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_UDP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_HTTP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_HTTP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_FTP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SMTP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_TELNET),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_JAVA),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SCRIPT),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_ACTIVEX),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OTHER),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_FILTERURL),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_NOFILTERURL),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_HIDE);
	}
	else
	{
		m_check_ispro=TRUE;
		MyServiceManageData.FilterAndBlock|=0x01;
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_TCP),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_UDP),SW_SHOWNORMAL);
		if(m_check_tcp)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_HTTP),SW_SHOWNORMAL);
			if(m_check_http)
			{
				::ShowWindow(::GetDlgItem(*this,IDC_STATIC_HTTP),SW_SHOWNORMAL);
				::ShowWindow(::GetDlgItem(*this,IDC_CHECK_JAVA),SW_SHOWNORMAL);
				::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SCRIPT),SW_SHOWNORMAL);
				::ShowWindow(::GetDlgItem(*this,IDC_CHECK_ACTIVEX),SW_SHOWNORMAL);
				::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OTHER),SW_SHOWNORMAL);
				::ShowWindow(::GetDlgItem(*this,IDC_STATIC_FILTERURL),SW_SHOWNORMAL);
				::ShowWindow(::GetDlgItem(*this,IDC_EDIT_NOFILTERURL),SW_SHOWNORMAL);
				if(m_check_other)
				{
					::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_SHOWNORMAL);
					::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_SHOWNORMAL);
				}
			}
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_FTP),SW_SHOWNORMAL);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SMTP),SW_SHOWNORMAL);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_TELNET),SW_SHOWNORMAL);
		}
	}

	
}

void CPage4::OnCheckIsurl() 
{
	// TODO: Add your control notification handler code here
	if(m_check_isurl)
	{
		m_check_isurl=FALSE;
		MyServiceManageData.FilterAndBlock&=0xFFFFFFFD;
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_URL),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_URL),SW_HIDE);
	}
	else
	{
		m_check_isurl=TRUE;
		MyServiceManageData.FilterAndBlock|=0x2;
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_URL),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_URL),SW_SHOWNORMAL);
	}	
}

void CPage4::OnCheckJava() 
{
	if(m_check_java)
	{
		m_check_java=FALSE;
		MyServiceManageData.TCPFilterBit&=0xFFFFFFfe;
	}
	else
	{
		m_check_java=TRUE;
		MyServiceManageData.TCPFilterBit|=0x01;
	}	
}

void CPage4::OnCheckOther() 
{
	// TODO: Add your control notification handler code here
	if(m_check_other)
	{
		m_check_other=FALSE;
		MyServiceManageData.TCPFilterBit&=0xFFFFFFF7;
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_HIDE);
	}
	else
	{
		m_check_other=TRUE;
		MyServiceManageData.TCPFilterBit|=0x08;
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_SHOWNORMAL);
	}	
}

void CPage4::OnCheckScript() 
{
	if(m_check_script)
	{
		m_check_script=FALSE;
		MyServiceManageData.TCPFilterBit&=0xFFFFFFfd;
	}
	else
	{
		m_check_script=TRUE;
		MyServiceManageData.TCPFilterBit|=0x02;
	}		
}

void CPage4::OnCheckTcp() 
{
	// TODO: Add your control notification handler code here
	if(m_check_tcp)
	{
		m_check_tcp=FALSE;
		MyServiceManageData.TranProtocal&=0xFFFFFFfe;
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_HTTP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_FTP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SMTP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_TELNET),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_HTTP),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_JAVA),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SCRIPT),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_ACTIVEX),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OTHER),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_FILTERURL),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_NOFILTERURL),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_HIDE);
		::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_HIDE);
		
	}
	else
	{
		m_check_tcp=TRUE;
		MyServiceManageData.TranProtocal|=0x01;
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_HTTP),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_FTP),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SMTP),SW_SHOWNORMAL);
		::ShowWindow(::GetDlgItem(*this,IDC_CHECK_TELNET),SW_SHOWNORMAL);
		if(m_check_http)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_HTTP),SW_SHOWNORMAL);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_JAVA),SW_SHOWNORMAL);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SCRIPT),SW_SHOWNORMAL);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_ACTIVEX),SW_SHOWNORMAL);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OTHER),SW_SHOWNORMAL);
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_FILTERURL),SW_SHOWNORMAL);
			::ShowWindow(::GetDlgItem(*this,IDC_EDIT_NOFILTERURL),SW_SHOWNORMAL);
			if(m_check_other)
			{
				::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_SHOWNORMAL);
				::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_SHOWNORMAL);
			}
		}
	}
	
}
WORD CPage4::GetDataFromFile()
{
	int totlen=0;
	int len;
	char command[40];
	char *pbuf;
	pfoutService=fopen("service.txt","r");
	if(pfoutService==NULL)return 0;
	while(feof(pfoutService)==0)
	{
		ReadCommand(pfoutService,command);
		switch(GetCommand(command))
		{
		case FILTERANDBLOCK:
			ReadCommand(pfoutService,command);
			sscanf(command,"%d",&MyServiceManageData.FilterAndBlock);
			if(MyServiceManageData.FilterAndBlock&0x01)
				m_check_ispro=TRUE;
			else m_check_ispro=FALSE;
			if(MyServiceManageData.FilterAndBlock&0x02)
				m_check_isurl=TRUE;
			else m_check_isurl=FALSE;
			break;
		case TRANPROTOCAL:
			ReadCommand(pfoutService,command);
			sscanf(command,"%d",&MyServiceManageData.TranProtocal);
			if(MyServiceManageData.TranProtocal&0x1)
			{
				MyServiceManageData.TranProtocal|=0x01;
				m_check_tcp=TRUE;
			}
			else m_check_tcp=FALSE;
			break;
		case PORT:
			ReadCommand(pfoutService,command);
			sscanf(command,"%d",&MyServiceManageData.Port);
			if(MyServiceManageData.Port&0x1)
			{
				MyServiceManageData.Port|=0x1;
				m_check_http=TRUE;
			}
			else
			{
				MyServiceManageData.Port&=0xfe;
				m_check_http=FALSE;
			}
			break;
		case FILTERKEYWORDS:
			ReadCommand(pfoutService,command);
			while(GetCommand(command)!=INITIAL_END_SEGMENT) {
					m_edit_otherkey+=command;
					m_edit_otherkey+="\r\n";
					ReadCommand(pfoutService,command);
			}
	/*		m_edit_otherkey.TrimRight("\r\n");
			
			len=m_edit_otherkey.Find("\r");
			while(len!=-1)
			{
				totlen=m_edit_otherkey.GetLength();
				m_edit_otherkey=m_edit_otherkey.Right(totlen-len-2);
				len=m_edit_otherkey.Find("\r");
			}
			m_edit_otherkey+="\r\n";
			*/
			len=m_edit_otherkey.Find("applet\r\n");
			if(len!=-1)
			{
				totlen=m_edit_otherkey.GetLength();
				m_edit_otherkey=m_edit_otherkey.Right(totlen-6);
			}
			len=m_edit_otherkey.Find("object\r\n");
			if(len!=-1)
			{
				totlen=m_edit_otherkey.GetLength();
				m_edit_otherkey=m_edit_otherkey.Right(totlen-9);
			}
			len=m_edit_otherkey.Find("script\r\n");
			if(len!=-1)
			{
				totlen=m_edit_otherkey.GetLength();
				m_edit_otherkey=m_edit_otherkey.Right(totlen-8);
			}
			break;
		case TCPFILTERBIT:
			ReadCommand(pfoutService,command);
			sscanf(command,"%d",&MyServiceManageData.TCPFilterBit);
			if(MyServiceManageData.TCPFilterBit&0x01)
				m_check_java=1;
			else m_check_java=0;
			if(MyServiceManageData.TCPFilterBit&0x02)
				m_check_script=1;
			else m_check_script=0;
			if(MyServiceManageData.TCPFilterBit&0x04)
				m_check_activex=1;
			else m_check_activex=0;
			if(MyServiceManageData.TCPFilterBit&0x08)
				m_check_other=1;
			else m_check_other=0;
			break;
		case BLOCKURL:
			ReadCommand(pfoutService,command);
			while(GetCommand(command)!=INITIAL_END_SEGMENT) {
					m_edit_url+=command;
					m_edit_url+="\r\n";
					ReadCommand(pfoutService,command);
			}
			break;
		case NOFILTERURL:
			ReadCommand(pfoutService,command);
			while(GetCommand(command)!=INITIAL_END_SEGMENT) {
					m_edit_nofilterurl+=command;
					m_edit_nofilterurl+="\r\n";
					ReadCommand(pfoutService,command);
			}
			break;	
		}
	}
	if(pfoutService)fclose(pfoutService);
	return 1;

}
void CPage4::ReadCommand(FILE *pf, char *pbuf)
{
	while(feof(pf)==0) {
		*pbuf=fgetc(pf);
		if(*pbuf=='\n') break;
		pbuf++;
	}
	*pbuf--=0;
	if(*pbuf=='\r')*pbuf=0;
}

WORD CPage4::GetCommand(char *pbuf)
{
	char buf[50];
	
	if(*pbuf==0) return INITIAL_END_SEGMENT;
	sscanf(pbuf,"%s",buf);
	if(strstr(buf,"[NoFilterURL]")!=NULL) return NOFILTERURL;
	else if(strstr(buf,"[FilterAndBlock]")!=NULL) return FILTERANDBLOCK;
	else if(strstr(buf,"[TranProtocal]")!=NULL)return TRANPROTOCAL;
	else if(strstr(buf,"[Port]")!=NULL) return PORT;
	else if(strstr(buf,"[FilterKeyWords]")!=NULL) return FILTERKEYWORDS;
	else if(strstr(buf,"[TCPFilterBit]")!=NULL) return TCPFILTERBIT;
	else if(strstr(buf,"[BlockUrl]")!=NULL)return BLOCKURL;
	else if(buf[0]=='#') return COMMENT;
	else if(strstr(buf,"[jl]")!=NULL) return INITIAL_END_SEGMENT;
	else return COMMAND_UNKNOWN;
}

void CPage4::OnCheckSmtp() 
{
	// TODO: Add your control notification handler code here
	
}

void CPage4::OnCheckTelnet() 
{
	// TODO: Add your control notification handler code here
	
}

void CPage4::OnCheckUdp() 
{
	// TODO: Add your control notification handler code here
	
}
void CPage4::ShowInitWindow()
{
//	if(pfoutService)
	{
		if(!m_check_ispro)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_TCP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_UDP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_HTTP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_HTTP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_FTP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SMTP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_TELNET),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_JAVA),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SCRIPT),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_ACTIVEX),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OTHER),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_FILTERURL),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_EDIT_NOFILTERURL),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_HIDE);
		}
		if(!m_check_isurl)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_URL),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_EDIT_URL),SW_HIDE);
		}
		if(!m_check_tcp)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_HTTP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_FTP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SMTP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_TELNET),SW_HIDE);
		}
		if(!m_check_http)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_JAVA),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_HTTP),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_SCRIPT),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_ACTIVEX),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_FILTERURL),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_EDIT_NOFILTERURL),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_CHECK_OTHER),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_HIDE);
		}
		if(!m_check_other)
		{
			::ShowWindow(::GetDlgItem(*this,IDC_STATIC_OTHERKEY),SW_HIDE);
			::ShowWindow(::GetDlgItem(*this,IDC_EDIT_OTHERKEY),SW_HIDE);
		}
	}
}

BOOL CPage4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetData();
	UpdateData(FALSE);
	ShowInitWindow();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage4::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetData();
	if(MyServiceManageData.TCPFilterBit&0x1)
		m_check_java=1;
	else m_check_java=0;
	if(MyServiceManageData.TCPFilterBit&0x2)
		m_check_script=1;
	else m_check_script=0;
	if(MyServiceManageData.TCPFilterBit&0x4)
		m_check_activex=1;
	else m_check_activex=0;
	UpdateData(0);
	return CPropertyPage::OnSetActive();
}

void CPage4::GetNameFromFile()
{
	int totlen=0;
	int len;
	char command[40];
	char *pbuf;
	pfoutService=fopen("service.txt","r");
	if(pfoutService==NULL)return;
	while(feof(pfoutService)==0)
	{
		ReadCommand(pfoutService,command);
		switch(GetCommand(command))
		{
		case FILTERANDBLOCK:
			ReadCommand(pfoutService,command);
			sscanf(command,"%d",&MyServiceManageData.FilterAndBlock);
			if(MyServiceManageData.FilterAndBlock&0x01)
				m_check_ispro=TRUE;
			else m_check_ispro=FALSE;
			if(MyServiceManageData.FilterAndBlock&0x02)
				m_check_isurl=TRUE;
			else m_check_isurl=FALSE;
			break;
		case TRANPROTOCAL:
			ReadCommand(pfoutService,command);
			sscanf(command,"%d",&MyServiceManageData.TranProtocal);
			if(MyServiceManageData.TranProtocal&0x1)
			{
				MyServiceManageData.TranProtocal|=0x01;
				m_check_tcp=TRUE;
			}
			else m_check_tcp=FALSE;
			break;
		case PORT:
			ReadCommand(pfoutService,command);
			sscanf(command,"%d",&MyServiceManageData.Port);
			if(MyServiceManageData.Port&0x1)
			{
				MyServiceManageData.Port|=0x1;
				m_check_http=TRUE;
			}
			else
			{
				MyServiceManageData.Port&=0xfe;
				m_check_http=FALSE;
			}
			break;
		case FILTERKEYWORDS:
			ReadCommand(pfoutService,command);
			while(GetCommand(command)!=INITIAL_END_SEGMENT) {
					m_edit_otherkey+=command;
					m_edit_otherkey+="\r\n";
			//		MessageBox(m_edit_otherkey);
					ReadCommand(pfoutService,command);
			}
	/*		m_edit_otherkey.TrimRight("\r\n");
			
			len=m_edit_otherkey.Find("\r");
			while(len!=-1)
			{
				totlen=m_edit_otherkey.GetLength();
				m_edit_otherkey=m_edit_otherkey.Right(totlen-len-2);
				len=m_edit_otherkey.Find("\r");
			}
			m_edit_otherkey+="\r\n";
			*/
			len=m_edit_otherkey.Find("applet\r\n");
			if(len!=-1)
			{
				totlen=m_edit_otherkey.GetLength();
				m_edit_otherkey=m_edit_otherkey.Right(totlen-6);
			}
			len=m_edit_otherkey.Find("object\r\n");
			if(len!=-1)
			{
				totlen=m_edit_otherkey.GetLength();
				m_edit_otherkey=m_edit_otherkey.Right(totlen-9);
			}
			len=m_edit_otherkey.Find("script\r\n");
			if(len!=-1)
			{
				totlen=m_edit_otherkey.GetLength();
				m_edit_otherkey=m_edit_otherkey.Right(totlen-8);
			}
			break;
		case TCPFILTERBIT:
			ReadCommand(pfoutService,command);
			sscanf(command,"%d",&MyServiceManageData.TCPFilterBit);
			if(MyServiceManageData.TCPFilterBit&0x01)
				m_check_java=1;
			else m_check_java=0;
			if(MyServiceManageData.TCPFilterBit&0x02)
				m_check_script=1;
			else m_check_script=0;
			if(MyServiceManageData.TCPFilterBit&0x04)
				m_check_activex=1;
			else m_check_activex=0;
			if(MyServiceManageData.TCPFilterBit&0x08)
				m_check_other=1;
			else m_check_other=0;
			break;
		case BLOCKURL:
			ReadCommand(pfoutService,command);
			while(GetCommand(command)!=INITIAL_END_SEGMENT) {
					m_edit_url+=command;
					m_edit_url+="\r\n";
					ReadCommand(pfoutService,command);
			}
			break;
		case NOFILTERURL:
			ReadCommand(pfoutService,command);
			while(GetCommand(command)!=INITIAL_END_SEGMENT) {
					m_edit_nofilterurl+=command;
					m_edit_nofilterurl+="\r\n";
					ReadCommand(pfoutService,command);
			}
			break;	
		}
	}
	if(pfoutService)fclose(pfoutService);
	return ;
}

int CPage4::GetBestData()
{
	char *nofilter;
	char tmpurl[50];
	
	nofilter=m_edit_url.GetBuffer(m_edit_url.GetLength());

	int ff=0;
	CString str="";
	CString strbest=m_edit_nofilterurl;
	int ffff=strbest.Find("\r\n");
	while(ffff>0)
	{
		memset(tmpurl,0,50);
		str=strbest.Left(ffff+2);
		strbest=strbest.Right(strbest.GetLength()-ffff-2);
		strcpy(tmpurl,str.GetBuffer(ffff+2));
		if(strstr(nofilter,tmpurl))ff++;
		ffff=strbest.Find("\r\n");
	//	if(strbest
	}
	if(ff){
		memset(tmpurl,0,50);
		sprintf(tmpurl,"阻断URL和不进行协议内容过滤URL有%d处冲突,请重新配置",ff);
		
	}
	return ff;
//	m_edit_nofilterurl
//	m_edit_url
}

void CPage4::GetData()
{
	int totlen;
	if(MyServiceManageData.FilterAndBlock&0x01)
		m_check_ispro=TRUE;
	else m_check_ispro=FALSE;
	if(MyServiceManageData.FilterAndBlock&0x02)
		m_check_isurl=TRUE;
	else m_check_isurl=FALSE;
	if(MyServiceManageData.TranProtocal&0x1)
	{
//		MyServiceManageData.TranProtocal|=0x01;
		m_check_tcp=TRUE;
	}
	else m_check_tcp=FALSE;
	if(MyServiceManageData.Port&0x1)
	{
//		MyServiceManageData.Port|=0x1;
		m_check_http=TRUE;
	}
	else
	{
//		MyServiceManageData.Port&=0xfe;
		m_check_http=FALSE;
	}
	m_edit_otherkey=MyServiceManageData.FilterKeyWord;
	int len=m_edit_otherkey.Find("applet\r\n");
	if(len!=-1)
	{
		totlen=m_edit_otherkey.GetLength();
		m_edit_otherkey=m_edit_otherkey.Right(totlen-6);
	}
	len=m_edit_otherkey.Find("object\r\n");
	if(len!=-1)
	{
		totlen=m_edit_otherkey.GetLength();
		m_edit_otherkey=m_edit_otherkey.Right(totlen-9);
	}
	len=m_edit_otherkey.Find("script\r\n");
	if(len!=-1)
	{
		totlen=m_edit_otherkey.GetLength();
		m_edit_otherkey=m_edit_otherkey.Right(totlen-8);
	}
	if(m_edit_otherkey.GetLength()<2)m_edit_otherkey="";
	//////////////////
	if(MyServiceManageData.TCPFilterBit&0x01)
		m_check_java=1;
	else m_check_java=0;
	if(MyServiceManageData.TCPFilterBit&0x02)
		m_check_script=1;
	else m_check_script=0;
	if(MyServiceManageData.TCPFilterBit&0x04)
		m_check_activex=1;
	else m_check_activex=0;
	if(MyServiceManageData.TCPFilterBit&0x08)
		m_check_other=1;
	else m_check_other=0;
	////////////////
	m_edit_url=MyServiceManageData.BlockURLList;
	///////////////////
	m_edit_nofilterurl=MyServiceManageData.NoFilterURLList;
	////////////////////////
}
