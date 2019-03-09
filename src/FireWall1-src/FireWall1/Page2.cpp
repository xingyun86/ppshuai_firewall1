
// Page2.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "global.h"
#include "Propsheet2.h"
#include "Page2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CDataConvert gDataConvert;
extern BOOL bhaved;
extern struct HostChar *phostchar;
extern int global_set;
/////////////////////////////////////////////////////////////////////////////
// CPage2 property page

IMPLEMENT_DYNCREATE(CPage2, CPropertyPage)

CPage2::CPage2() : CPropertyPage(CPage2::IDD)
{
	//{{AFX_DATA_INIT(CPage2)
	m_bCheckIn = FALSE;
	m_bCheckOut = FALSE;
	m_bCheckDm = FALSE;
	m_bDiscardMultcast = FALSE;
	m_bLoseOtherIp = FALSE;
	m_bLoseNonIp = FALSE;
	m_bLoseSuspectOffset = FALSE;
	m_bLoseFragementIcmp = FALSE;
	m_bLoseAttackIcmp = FALSE;
	m_bforbidIcmp = FALSE;
	//}}AFX_DATA_INIT
}

CPage2::~CPage2()
{
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2)
	DDX_Control(pDX, IDC_EDIT10, m_EditAllowIpTab);
	DDX_Control(pDX, IDC_EDIT1, m_RejIpTab);
	DDX_Check(pDX, IDC_CHECK1, m_bCheckIn);
	DDX_Check(pDX, IDC_CHECK2, m_bCheckOut);
	DDX_Check(pDX, IDC_CHECK3, m_bCheckDm);
	DDX_Check(pDX, IDC_CHECK4, m_bDiscardMultcast);
	DDX_Check(pDX, IDC_CHECK6, m_bLoseOtherIp);
	DDX_Check(pDX, IDC_CHECK5, m_bLoseNonIp);
	DDX_Check(pDX, IDC_CHECK7, m_bLoseSuspectOffset);
	DDX_Check(pDX, IDC_CHECK8, m_bLoseFragementIcmp);
	DDX_Check(pDX, IDC_CHECK9, m_bLoseAttackIcmp);
	DDX_Check(pDX, IDC_CHECK10, m_bforbidIcmp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2)
	ON_BN_CLICKED(IDC_BUTTON1, OnOk)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnCancel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2 message handlers

void CPage2::OnOk() 
{
	UpdateData();
	gDataConvert.globalFlags.bCheckInNetAttack=m_bCheckIn;
	gDataConvert.globalFlags.bforbidIcmp=m_bforbidIcmp;
    gDataConvert.globalFlags.bCheckOutNetAccack=m_bCheckOut;
    gDataConvert.globalFlags.bCheckDmAccack=m_bCheckDm;
    gDataConvert.globalFlags.bMulticast=m_bDiscardMultcast;
    gDataConvert.globalFlags.bNonIpPacket=m_bLoseNonIp;
    gDataConvert.globalFlags.bOtherIpPacket=m_bLoseOtherIp;
    gDataConvert.globalFlags.bSusPectOffset=m_bLoseSuspectOffset;
    gDataConvert.globalFlags.bFragmentIcmp=m_bLoseFragementIcmp;
    gDataConvert.globalFlags.bSusPectIcmp=m_bLoseAttackIcmp;
    gDataConvert.m_alllinechar.sGlobalFlags=gDataConvert.globalFlags;
    gDataConvert.AddIpTable(ALLOWIPTAB,m_EditAllowIpTab);
    gDataConvert.AddIpTable(REJIPTAB,m_RejIpTab);
	gDataConvert.SaveChar(m_EditAllowIpTab,FALSE,ALLOWIPTAB);
	gDataConvert.SaveChar(m_RejIpTab,FALSE,REJIPTAB);
	global_set|=0x2;
}

void CPage2::OnCancel() 
{
	// TODO: Add your control notification handler code here
	
}


BOOL CPage2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_bCheckIn=gDataConvert.globalFlags.bCheckInNetAttack;
	m_bforbidIcmp=gDataConvert.globalFlags.bforbidIcmp;
    m_bCheckOut=gDataConvert.globalFlags.bCheckOutNetAccack;
    m_bCheckDm=gDataConvert.globalFlags.bCheckDmAccack;
    m_bDiscardMultcast=gDataConvert.globalFlags.bMulticast;
    m_bLoseNonIp=gDataConvert.globalFlags.bNonIpPacket;
    m_bLoseOtherIp=gDataConvert.globalFlags.bOtherIpPacket;
    m_bLoseSuspectOffset=gDataConvert.globalFlags.bSusPectOffset;
    m_bLoseFragementIcmp=gDataConvert.globalFlags.bFragmentIcmp;
    m_bLoseAttackIcmp=gDataConvert.globalFlags.bSusPectIcmp;

    gDataConvert.DisplayFireCfg(m_EditAllowIpTab,ALLOWIPTAB);
    gDataConvert.DisplayFireCfg(m_RejIpTab,REJIPTAB);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage2::OnButton3() 
{
   CPropSheet2 	propSheet2("Ö÷»úÅäÖÃ", this, 0);
   if(propSheet2.DoModal()==IDOK)
   {
	   return;
   }	
}

void CPage2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

BOOL CPage2::OnSetActive() 
{
	m_bCheckIn=gDataConvert.globalFlags.bCheckInNetAttack;
	m_bforbidIcmp=gDataConvert.globalFlags.bforbidIcmp;
    m_bCheckOut=gDataConvert.globalFlags.bCheckOutNetAccack;
    m_bCheckDm=gDataConvert.globalFlags.bCheckDmAccack;
    m_bDiscardMultcast=gDataConvert.globalFlags.bMulticast;
    m_bLoseNonIp=gDataConvert.globalFlags.bNonIpPacket;
    m_bLoseOtherIp=gDataConvert.globalFlags.bOtherIpPacket;
    m_bLoseSuspectOffset=gDataConvert.globalFlags.bSusPectOffset;
    m_bLoseFragementIcmp=gDataConvert.globalFlags.bFragmentIcmp;
    m_bLoseAttackIcmp=gDataConvert.globalFlags.bSusPectIcmp;

    gDataConvert.DisplayFireCfg(m_EditAllowIpTab,ALLOWIPTAB);
    gDataConvert.DisplayFireCfg(m_RejIpTab,REJIPTAB);
	UpdateData(FALSE);
	return CPropertyPage::OnSetActive();
}
