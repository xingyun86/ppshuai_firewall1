
// PropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "FireWall1.h"
#include "PropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern int global_set;
FILE *pwritebigset=NULL;
extern BOOL bPropsheetOpen;

/////////////////////////////////////////////////////////////////////////////
// CPropSheet

IMPLEMENT_DYNAMIC(CPropSheet, CPropertySheet)

CPropSheet::CPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
    AddPage(&m_page2);
	AddPage(&m_dmpage);
    AddPage(&m_page4);
	AddPage(&m_page5);
    AddPage(&m_page6);
	bPropsheetOpen=TRUE;
}

CPropSheet::CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
    AddPage(&m_page2);
	AddPage(&m_dmpage);
    AddPage(&m_page4);
	AddPage(&m_page5);
    AddPage(&m_page6);
	bPropsheetOpen=TRUE;
}

CPropSheet::~CPropSheet()
{
	char str[15];
	memset(str,0,15);
	itoa(global_set,str,10);
	pwritebigset=fopen("bigset.txt","w");
	if(pwritebigset)
	{
		fprintf(pwritebigset,str);
		fprintf(pwritebigset,"\n");
		fclose(pwritebigset);
	}
		bPropsheetOpen=FALSE;
}
	

BEGIN_MESSAGE_MAP(CPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPropSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSheet message handlers



BOOL CPropSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	CButton* tempButton3;
	tempButton3=(CButton*)GetDlgItem(ID_APPLY_NOW);
	tempButton3->ShowWindow(SW_HIDE);	
	return bResult;
}
