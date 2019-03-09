
// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog


COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

BOOL COptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	TabCtrlItem.pszText =_T("网络策略");
	m_Tab.InsertItem(0, &TabCtrlItem);
	TabCtrlItem.pszText = "规则库";
	m_Tab.InsertItem( 1, &TabCtrlItem );
	TabCtrlItem.pszText = "缺省配置";
	m_Tab.InsertItem( 2, &TabCtrlItem );
    TabCtrlItem.pszText="服务策略";
	m_Tab.InsertItem(3,&TabCtrlItem);
    TabCtrlItem.pszText="用户验证";
	m_Tab.InsertItem(3,&TabCtrlItem);
	 TabCtrlItem.pszText="日志记录";
	m_Tab.InsertItem(3,&TabCtrlItem);
	 TabCtrlItem.pszText="状态监视";
	m_Tab.InsertItem(3,&TabCtrlItem);	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
    int nSelIndex = m_Tab.GetCurSel();

	switch(nSelIndex)
	{
	case 0:
		
		break;
	case 1:
		
		break;
	case 2:
		
		break;
	case 3:
		
		break;
	case 4:
		
		break;
	case 5:
		
		break;
	}
	
	*pResult = 0;
}
