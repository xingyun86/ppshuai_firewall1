
// Page5.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "Page5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
FILE *pfoutUserver=NULL;
FILE *pwriteUserver=NULL;
UserVerifyData MyUserVerifyData;
char tmpnamepass[9182];
/////////////////////////////////////////////////////////////////////////////
// CPage5 property page
BOOL nook;
IMPLEMENT_DYNCREATE(CPage5, CPropertyPage)

CPage5::CPage5() : CPropertyPage(CPage5::IDD)
{
	//{{AFX_DATA_INIT(CPage5)
	m_check_isuserver = FALSE;
	m_edit_passwd = _T("");
	m_edit_time = 0;
	m_edit_user = _T("");
	//}}AFX_DATA_INIT
	memset(tmpnamepass,0,9182);
	nook=1;
}

CPage5::~CPage5()
{
//	OnButtonOk();
		
/*	if(nook)
	{
		GetNameFromFile();
		memset(MyUserVerifyData.UserList,0,9182);
		strcpy(MyUserVerifyData.UserList,tmpnamepass);

	}
	else MessageBox(MyUserVerifyData.UserList);
	*/
}

void CPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage5)
	DDX_Check(pDX, IDC_CHECK_ISUSERVER, m_check_isuserver);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_edit_passwd);
	DDX_Text(pDX, IDC_EDIT_TIME, m_edit_time);
	DDV_MinMaxInt(pDX, m_edit_time, 1, 55);
	DDX_Text(pDX, IDC_EDIT_USER, m_edit_user);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage5, CPropertyPage)
	//{{AFX_MSG_MAP(CPage5)
	ON_BN_CLICKED(IDC_BUTTON_ADDUSER, OnButtonAdduser)
	ON_BN_CLICKED(IDC_BUTTON_DELUSER, OnButtonDeluser)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_CHECK_ISUSERVER, OnCheckIsuserver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage5 message handlers
DWORD CPage5::GetDataFromFile()
{
	int iiii=0;
	char command[40];
	char *pbuf;
	pfoutUserver=fopen("userverify.txt","r");
	if(pfoutUserver==NULL)return 0;
	while(feof(pfoutUserver)==0)
	{
		ReadCommand(pfoutUserver,command);
		switch(GetCommand(command))
		{
		case ISUSERVERIFY:
			ReadCommand(pfoutUserver,command);
			sscanf(command,"%d",&MyUserVerifyData.IsVerify);
			if(MyUserVerifyData.IsVerify&0x01)
				m_check_isuserver=TRUE;
			else m_check_isuserver=FALSE;
			break;
		case TIME:
			ReadCommand(pfoutUserver,command);
			sscanf(command,"%d",&MyUserVerifyData.timeRefesh);
			m_edit_time=MyUserVerifyData.timeRefesh;
			break;
		case USERNAMEANDPASSWD:
			ReadCommand(pfoutUserver,command);
			
			while(GetCommand(command)!=INITIAL_END_SEGMENT) {
				if(iiii++==0)strcpy(tmpnamepass,command);
				else strcat(tmpnamepass,command);
				SendDlgItemMessage(IDC_LIST_USERPAS, LB_ADDSTRING, 0, (LPARAM)command);
					ReadCommand(pfoutUserver,command);
			}
			break;	
		}
	}
	return 1;

}

void CPage5::ReadCommand(FILE *pf, char *pbuf)
{
	while(feof(pf)==0) {
		*pbuf=fgetc(pf);
		if(*pbuf=='\n') break;
		pbuf++;
	}
	*pbuf--=0;
	if(*pbuf=='\r')*pbuf=0;
}

WORD CPage5::GetCommand(char *pbuf)
{
	char buf[50];
	
	if(*pbuf==0) return INITIAL_END_SEGMENT;
	sscanf(pbuf,"%s",buf);
	if(strstr(buf,"[isUserVerify]")!=NULL) return ISUSERVERIFY;
	else if(strstr(buf,"[TIME]")!=NULL) return TIME;
	else if(strstr(buf,"[UserNameandPasswd]")!=NULL)return USERNAMEANDPASSWD;
	else if(strstr(buf,"[jl]")!=NULL) return INITIAL_END_SEGMENT;
	else return COMMAND_UNKNOWN;
}

void CPage5::OnButtonAdduser() 
{
	UpdateData(TRUE);
	//现在还没有进行唯一用户检查//jl1102
	if(m_edit_user=="")
		AfxMessageBox("please input the username");
	if(m_edit_passwd=="")
		AfxMessageBox("please input the password");
	char temp[200];
	memset(temp,0,200);
	CString strtemp="";
	strtemp=m_edit_user+':'+m_edit_passwd;
	strcpy(temp,_T(strtemp));
	SendDlgItemMessage(IDC_LIST_USERPAS, LB_ADDSTRING, 0, (LPARAM)temp);	
}

void CPage5::OnButtonDeluser() 
{
	int index;
	index=SendDlgItemMessage(IDC_LIST_USERPAS, LB_GETCURSEL, 0, 0);
	SendDlgItemMessage(IDC_LIST_USERPAS, LB_DELETESTRING,(WPARAM) index,0);	
}

void CPage5::OnButtonOk() 
{
	nook=0;
	UpdateData(TRUE);
	MyUserVerifyData.timeRefesh=m_edit_time;
	pwriteUserver=fopen("userverify.txt","w");
	fprintf(pwriteUserver,_T("[isUserVerify]\n"));
	fprintf(pwriteUserver,"%d\n",MyUserVerifyData.IsVerify);
	fprintf(pwriteUserver,"\n");
	fprintf(pwriteUserver,_T("[TIME]\n"));
	fprintf(pwriteUserver,"%d\n",MyUserVerifyData.timeRefesh);
	fprintf(pwriteUserver,"\n");
	int count;
	count=SendDlgItemMessage(IDC_LIST_USERPAS, LB_GETCOUNT,0,0);
	fprintf(pwriteUserver,_T("[UserNameandPasswd]\n"));
	for(int i=0;i<count;i++)
	{
//		LPCTSTR lpszBuffer;
//		lpszBuffer=new char[100];
		char lpszBuffer[25];
		memset(lpszBuffer,0,25);
		SendDlgItemMessage(IDC_LIST_USERPAS, LB_GETTEXT,(WPARAM)i,(LPARAM)lpszBuffer);
		fprintf(pwriteUserver,"%s\n",lpszBuffer);
		if (i==0)strcpy(MyUserVerifyData.UserList,lpszBuffer);
		else strcat(MyUserVerifyData.UserList,lpszBuffer);
		strcat(MyUserVerifyData.UserList,"\r\n");
		
//		delete []lpszBuffer;
	}

	fprintf(pwriteUserver,"\n");
	fprintf(pwriteUserver,_T("[jl]\n"));
	if(pwriteUserver!=NULL)
		fclose(pwriteUserver);	
}

void CPage5::OnCheckIsuserver() 
{
	if(m_check_isuserver)
	{
		m_check_isuserver=FALSE;
		MyUserVerifyData.IsVerify=FALSE;
	}
	else
	{
		m_check_isuserver=TRUE;
		MyUserVerifyData.IsVerify=TRUE;
	}	
}

BOOL CPage5::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	GetData();
	UpdateData(FALSE);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage5::GetNameFromFile()
{
	int iiii=0;
	char command[40];
	char *pbuf;
	pfoutUserver=fopen("userverify.txt","r");
	if(pfoutUserver==NULL)return ;
	while(feof(pfoutUserver)==0)
	{
		ReadCommand(pfoutUserver,command);
		switch(GetCommand(command))
		{
		case ISUSERVERIFY:
			ReadCommand(pfoutUserver,command);
			sscanf(command,"%d",&MyUserVerifyData.IsVerify);
			if(MyUserVerifyData.IsVerify&0x01)
				m_check_isuserver=TRUE;
			else m_check_isuserver=FALSE;
			break;
		case TIME:
			ReadCommand(pfoutUserver,command);
			sscanf(command,"%d",&MyUserVerifyData.timeRefesh);
			m_edit_time=MyUserVerifyData.timeRefesh;
			break;
		case USERNAMEANDPASSWD:
			ReadCommand(pfoutUserver,command);
			
			while(GetCommand(command)!=INITIAL_END_SEGMENT) {
				if(iiii++==0)strcpy(tmpnamepass,command);
				else strcat(tmpnamepass,command);
				strcat(tmpnamepass,"\r\n");
				ReadCommand(pfoutUserver,command);
			}
			break;	
		}
	}
	if(pfoutUserver)fclose(pfoutUserver);
	return ;
}

void CPage5::GetData()
{
	if(MyUserVerifyData.IsVerify&0x01)
		m_check_isuserver=TRUE;
	else m_check_isuserver=FALSE;
	///////////////////////
	m_edit_time=MyUserVerifyData.timeRefesh;
	///////////////////
	strcpy(tmpnamepass,MyUserVerifyData.UserList);
	CString str=tmpnamepass,str1;
	int ii=str.Find("\r\n");
	char t1[30];
	memset(t1,0,30);
	while(ii>1)
	{
		str1=str.Left(ii);
		strcpy(t1,str1.GetBuffer(ii));
		SendDlgItemMessage(IDC_LIST_USERPAS, LB_ADDSTRING, 0, (LPARAM)t1);
		str=str.Right(str.GetLength ()-ii-2);
		ii=str.Find("\r\n");
		memset(t1,0,30);
	}
}
