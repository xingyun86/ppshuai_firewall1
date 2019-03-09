
// Page12.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "Struct.h"
#include "global.h"
#include "Page12.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CDataConvert gDataConvert;
extern BOOL bhaved;
extern struct HostChar *phostchar;
// CPage12 property page

IMPLEMENT_DYNCREATE(CPage12, CPropertyPage)

CPage12::CPage12() : CPropertyPage(CPage12::IDD)
{
	//{{AFX_DATA_INIT(CPage12)
	//}}AFX_DATA_INIT
}

CPage12::~CPage12()
{
}

void CPage12::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage12)
	DDX_Control(pDX, IDC_EDIT4, m_IcmpTypeRejOut);
	DDX_Control(pDX, IDC_EDIT3, m_IcmpTypeRejIn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage12, CPropertyPage)
	//{{AFX_MSG_MAP(CPage12)
	ON_BN_CLICKED(IDC_BUTTON1, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage12 message handlers



void CPage12::OnOk() 
{
gDataConvert.SetPortList(m_IcmpTypeRejIn,ICMPTYPEIN);
gDataConvert.SetPortList(m_IcmpTypeRejOut,ICMPTYPEOUT);

gDataConvert.SaveChar(m_IcmpTypeRejIn,TRUE,ICMPTYPEIN);
gDataConvert.SaveChar(m_IcmpTypeRejOut,TRUE,ICMPTYPEOUT);

}

BOOL CPage12::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
    if(bhaved)
	{
      gDataConvert.DisplayHost(phostchar,ICMPTYPEIN,m_IcmpTypeRejIn);
      gDataConvert.DisplayHost(phostchar,ICMPTYPEOUT,m_IcmpTypeRejOut);
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage12::OnSetActive() 
{
    if(bhaved)
	{
      gDataConvert.DisplayHost(phostchar,ICMPTYPEIN,m_IcmpTypeRejIn);
      gDataConvert.DisplayHost(phostchar,ICMPTYPEOUT,m_IcmpTypeRejOut);
	}
	UpdateData(FALSE);
	return CPropertyPage::OnSetActive();
}
