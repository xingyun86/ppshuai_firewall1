
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FireWall1.h"
#include "Struct.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern BOOL m_bhide;

//extern	CListCtrl m_listView;
//extern	CListCtrl m_listView1;
extern BOOL flag ;

#define MAXITEMNUMBER 10
extern USHORT logType;
char myBuf[140];

//extern int number;
//#define MAXITEMNUMBER 10
//extern LV_ITEM lvItem;
//extern LV_ITEM lvItem1;
//extern CListCtrl m_listView;
//extern CListCtrl m_listView1;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_STOP_LOG, OnStopLog)
	ON_UPDATE_COMMAND_UI(ID_STOP_LOG, OnUpdateStopLog)
	ON_MESSAGE(MYWN_NOTIFYICON,MyTaskBarProc)
	ON_WM_DESTROY()
    ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(M_LOGVIEW,OnLogView)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	/*
	HICON hicon=AfxGetApp()->LoadIcon(IDI_ICON1);
	char c[10] = "·À»ðÇ½";
	((CFireWall1App*)AfxGetApp())->MyTaskAddIcon(AfxGetMainWnd()->GetSafeHwnd(), IDI_ICON1, hicon, c);
	*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers




void CMainFrame::OnStopLog() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnUpdateStopLog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
BOOL CMainFrame::MyTaskBarDelIcon(HWND hwnd, UINT uID)
{
	BOOL res;
	NOTIFYICONDATA tnid;
	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hwnd;
	tnid.uID = uID;
	res = Shell_NotifyIcon(NIM_DELETE,&tnid);
	return res;
	
}
BOOL CMainFrame::MyTaskChangeIcon(HWND hwnd, UINT uID)
{
	BOOL res;
	HICON hicon;
	hicon=LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(m_bhide?IDI_ICON2:IDI_ICON1));
	NOTIFYICONDATA tnid;
	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hwnd;
	tnid.uID = uID;
	tnid.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tnid.uCallbackMessage = MYWN_NOTIFYICON;
	tnid.hIcon = hicon;
	char lpszTip[10] = "·À»ðÇ½";
	if(lpszTip)
		lstrcpyn(tnid.szTip,lpszTip,sizeof(tnid.szTip));
	else
		tnid.szTip[0] = '\0';
	res = Shell_NotifyIcon(NIM_MODIFY,&tnid);
	if(hicon)
		DestroyIcon(hicon);
	return res;
}
LRESULT CMainFrame::MyTaskBarProc(WPARAM wParam, LPARAM lParam)
{
	UINT uID = (UINT)wParam;
	UINT uMsg = (UINT)lParam;

	if(uID == IDI_ICON1)
	{
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
            MyTaskChangeIcon(m_hWnd,IDI_ICON1);
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			m_bhide=(m_bhide?FALSE:TRUE);
			break;
		case WM_RBUTTONUP:
			{
				CMenu menu;
				menu.LoadMenu(IDR_MENU_ICON);
				CPoint point;
				GetCursorPos(&point);

				////--------
				SetForegroundWindow();
				////--------

				menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON, 
													point.x, point.y, this, NULL);	
				////--------
			}
			break;
		default:
			break;

		}
	}
    return 1L;
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
    MyTaskBarDelIcon(m_hWnd,IDI_ICON1);  	
}
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID==61472)
		ShowWindow(SW_HIDE);
    else
		CFrameWnd::OnSysCommand(nID,lParam);
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
