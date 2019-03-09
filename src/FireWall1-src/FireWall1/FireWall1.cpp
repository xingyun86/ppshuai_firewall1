
// FireWall1.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FireWall1.h"

#include "MainFrm.h"
#include "FireWall1Doc.h"
#include "FireWall1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
FILE *pfoutBigSet=NULL;
BOOL m_bhide;
int global_set;//bit 0,=1表示网络策略OK；
				//bit 1,=1,表示包过滤策略OK；
				//bit 2,=1,表示服务策略OK；
/////////////////////////////////////////////////////////////////////////////
// CFireWall1App

BEGIN_MESSAGE_MAP(CFireWall1App, CWinApp)
	//{{AFX_MSG_MAP(CFireWall1App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFireWall1App construction

CFireWall1App::CFireWall1App()
{
	// TODO: add construction code here,
	global_set=0;
	bFound=FALSE;
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFireWall1App object

CFireWall1App theApp;

/////////////////////////////////////////////////////////////////////////////
// CFireWall1App initialization

BOOL CFireWall1App::InitInstance()
{
	 hMutexOneInstance = CreateMutex(NULL,TRUE,_T("PreventSecondInstance"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
        bFound = TRUE;
    if(hMutexOneInstance) 
        ReleaseMutex(hMutexOneInstance);
    if (bFound == TRUE) return FALSE;
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFireWall1Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CFireWall1View));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	HICON hicon=LoadIcon(IDI_ICON1);
	char c[10] = "防火墙";
	MyTaskAddIcon(AfxGetMainWnd()->GetSafeHwnd(), IDI_ICON1, hicon, c);
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->UpdateWindow();
	m_bhide=TRUE;
	char str[15];
	memset(str,0,15);
	pfoutBigSet=fopen("bigset.txt","r");
	if(pfoutBigSet)
	{
		ReadCommand(pfoutBigSet,str);
		sscanf(str,"%d",&global_set);
	}
	::SetWindowText(AfxGetMainWnd()->GetSafeHwnd(),"托普防火墙");
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CFireWall1App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CFireWall1App message handlers


void CFireWall1App::ReadCommand(FILE *pf, char *pbuf)
{
	while(feof(pf)==0) {
		*pbuf=fgetc(pf);
		if(*pbuf=='\n') break;
		pbuf++;
	}
	*pbuf--=0;
	if(*pbuf=='\r')*pbuf=0;
}

void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
BOOL CFireWall1App::MyTaskAddIcon(HWND hwnd, UINT uID, HICON hicon, LPSTR lpszTip)
{
	BOOL res;
	NOTIFYICONDATA tnid;
	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hwnd;
	tnid.uID = uID;
	tnid.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tnid.uCallbackMessage = MYWN_NOTIFYICON;
	tnid.hIcon = hicon;
	if(lpszTip)
		lstrcpyn(tnid.szTip,lpszTip,sizeof(tnid.szTip));
	else
		tnid.szTip[0] = '\0';

	res = Shell_NotifyIcon(NIM_ADD,&tnid);
	if(hicon)
		DestroyIcon(hicon);
	return res;
}
