
// httpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "DmDlg.h"
#include "global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CDataConvert gDataConvert;
/////////////////////////////////////////////////////////////////////////////
// CDmDlg property page

IMPLEMENT_DYNCREATE(CDmDlg, CPropertyPage)

CDmDlg::CDmDlg() : CPropertyPage(CDmDlg::IDD)
{
	//{{AFX_DATA_INIT(CDmDlg)
	//}}AFX_DATA_INIT
}

CDmDlg::~CDmDlg()
{
}

void CDmDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDmDlg)
	DDX_Control(pDX, IDC_EDIT4, m_AllowIp);
	DDX_Control(pDX, IDC_EDIT3, m_RejIp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDmDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CDmDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDmDlg message handlers

void CDmDlg::OnButton1() 
{
   	gDataConvert.AddIpTable(OUTDM,m_AllowIp);
    gDataConvert.AddIpTable(INDM,m_RejIp);
	gDataConvert.SaveChar(m_AllowIp,FALSE,OUTDM);
	gDataConvert.SaveChar(m_RejIp,FALSE,INDM);
}

BOOL CDmDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
    gDataConvert.DisplayFireCfg(m_AllowIp,OUTDM);
    gDataConvert.DisplayFireCfg(m_RejIp,INDM);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
