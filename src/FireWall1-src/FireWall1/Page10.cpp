
// Page10.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "global.h"
#include "Page10.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CDataConvert gDataConvert;
extern BOOL bhaved;
extern struct HostChar *phostchar;
/////////////////////////////////////////////////////////////////////////////
// CPage10 property page

IMPLEMENT_DYNCREATE(CPage10, CPropertyPage)

CPage10::CPage10() : CPropertyPage(CPage10::IDD)
{

	//{{AFX_DATA_INIT(CPage10)
	//}}AFX_DATA_INIT
}

CPage10::~CPage10()
{
}

void CPage10::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage10)
	DDX_Control(pDX, IDC_EDIT15, m_TcpDstOut);
	DDX_Control(pDX, IDC_EDIT13, m_TcpSrcOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage10, CPropertyPage)
	//{{AFX_MSG_MAP(CPage10)
	ON_BN_CLICKED(IDC_BUTTON3, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage10 message handlers

void CPage10::OnOk() 
{
	gDataConvert.SetPortList(m_TcpDstOut,TCPDSTOUT);
	gDataConvert.SetPortList(m_TcpSrcOut,TCPSRCOUT);

	gDataConvert.SaveChar(m_TcpDstOut,TRUE,TCPDSTOUT);
	gDataConvert.SaveChar(m_TcpSrcOut,TRUE,TCPSRCOUT);

}

BOOL CPage10::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
    if(bhaved)
	{
      gDataConvert.DisplayHost(phostchar,TCPSRCOUT,m_TcpSrcOut);
      gDataConvert.DisplayHost(phostchar,TCPDSTOUT,m_TcpDstOut);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage10::OnSetActive() 
{
    if(bhaved)
	{
      gDataConvert.DisplayHost(phostchar,TCPSRCOUT,m_TcpSrcOut);
      gDataConvert.DisplayHost(phostchar,TCPDSTOUT,m_TcpDstOut);
	}
	UpdateData(FALSE);
	return CPropertyPage::OnSetActive();
}
