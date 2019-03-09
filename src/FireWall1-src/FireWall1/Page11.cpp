
// Page11.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include  "Struct.h"
#include  "global.h"
#include "Page11.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CDataConvert gDataConvert;
extern BOOL bhaved;
extern struct HostChar *phostchar;
/////////////////////////////////////////////////////////////////////////////
// CPage11 property page

IMPLEMENT_DYNCREATE(CPage11, CPropertyPage)

CPage11::CPage11() : CPropertyPage(CPage11::IDD)
{
	//{{AFX_DATA_INIT(CPage11)
	//}}AFX_DATA_INIT
}

CPage11::~CPage11()
{
}

void CPage11::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage11)
	DDX_Control(pDX, IDC_EDIT19, m_UdpDstOut);
	DDX_Control(pDX, IDC_EDIT17, m_UdpSrcOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage11, CPropertyPage)
	//{{AFX_MSG_MAP(CPage11)
	ON_BN_CLICKED(IDC_BUTTON1, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage11 message handlers



void CPage11::OnOk() 
{
	gDataConvert.SetPortList(m_UdpDstOut,UDPDSTOUT);
	gDataConvert.SetPortList(m_UdpSrcOut,UDPSRCOUT);

	gDataConvert.SaveChar(m_UdpDstOut,TRUE,UDPDSTOUT);
	gDataConvert.SaveChar(m_UdpSrcOut,TRUE,UDPSRCOUT);

}

BOOL CPage11::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    if(bhaved)
	{
      gDataConvert.DisplayHost(phostchar,UDPSRCOUT,m_UdpSrcOut);
      gDataConvert.DisplayHost(phostchar,UDPDSTOUT,m_UdpDstOut);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage11::OnSetActive() 
{
    if(bhaved)
	{
      gDataConvert.DisplayHost(phostchar,UDPSRCOUT,m_UdpSrcOut);
      gDataConvert.DisplayHost(phostchar,UDPDSTOUT,m_UdpDstOut);
	}
	UpdateData(FALSE);
	return CPropertyPage::OnSetActive();
}
