
// PropSheet2.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "PropSheet2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropSheet2

IMPLEMENT_DYNAMIC(CPropSheet2, CPropertySheet)

CPropSheet2::CPropSheet2(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page13);
    AddPage(&m_page10);
	AddPage(&m_page11);
    AddPage(&m_page12);
}

CPropSheet2::CPropSheet2(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page13);
    AddPage(&m_page10);
	AddPage(&m_page11);
    AddPage(&m_page12);
}

CPropSheet2::~CPropSheet2()
{
}


BEGIN_MESSAGE_MAP(CPropSheet2, CPropertySheet)
	//{{AFX_MSG_MAP(CPropSheet2)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSheet2 message handlers

void CPropSheet2::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CPropertySheet::OnClose();
}

BOOL CPropSheet2::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	CButton *tempButton1,*tempButton2,*tempButton3;
    tempButton1=(CButton *)GetDlgItem(IDCANCEL);
	tempButton1->SetWindowText("х╥хо");
	tempButton2=(CButton *)GetDlgItem(IDOK);
	tempButton2->ShowWindow(SW_HIDE);
	tempButton3=(CButton*)GetDlgItem(ID_APPLY_NOW);
	tempButton3->ShowWindow(SW_HIDE);
	return bResult;
}
