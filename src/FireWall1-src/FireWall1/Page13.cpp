
// Page13.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "global.h"
#include "Page13.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
  extern CDataConvert gDataConvert;
  BOOL bhaved;
  struct HostChar *phostchar;
/////////////////////////////////////////////////////////////////////////////
// CPage13 property page

IMPLEMENT_DYNCREATE(CPage13, CPropertyPage)

CPage13::CPage13() : CPropertyPage(CPage13::IDD)
{
	//{{AFX_DATA_INIT(CPage13)
	m_HostIp = _T("");
	//}}AFX_DATA_INIT
	phostchar=new struct HostChar;
    m_bSel=FALSE;
}

CPage13::~CPage13()
{
	delete phostchar;
}

void CPage13::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage13)
	DDX_Control(pDX, IDC_LIST1, m_HostList);
	DDX_Text(pDX, IDC_EDIT1, m_HostIp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage13, CPropertyPage)
	//{{AFX_MSG_MAP(CPage13)
	ON_BN_CLICKED(IDC_BUTTON1, OnOk)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage13 message handlers

void CPage13::OnOk() 
{
  UpdateData();
  memset(phostchar,0,sizeof(struct HostChar));
  gDataConvert.SetHostIp(m_HostIp,bhaved,phostchar);
  if(!m_bSel)
     m_HostList.AddString(m_HostIp);
  UpdateData(FALSE);
  m_bSel=FALSE;
//  if(!bhaved)breset=TRUE;//表示连续设置
}

BOOL CPage13::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	int len=gDataConvert.m_alllinechar.CurHostNum;
	CString *str=new CString[len];
	for(int i=0;i<len;i++)
	{
	  str[i]=(CString)gDataConvert.m_alllinechar.Hostchar[i].HostIp;
	  m_HostList.AddString(str[i].GetBuffer(str[i].GetLength()));
	}
	delete []str;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage13::OnDelete() 
{
  UpdateData();
  gDataConvert.DeleteHost(m_HostIp);
  int index=m_HostList.GetCurSel();
  m_HostList.DeleteString(index);
  m_HostIp="";
  UpdateData(FALSE);

}

void CPage13::OnSelchangeList1() 
{
	int index=m_HostList.GetCurSel();
	if(index!=LB_ERR)
	{
		m_HostList.GetText(index,m_HostIp);
	}
    UpdateData(FALSE);
	m_bSel=TRUE;
		

}

BOOL CPage13::OnSetActive() 
{
	
	return CPropertyPage::OnSetActive();
}
