
#if !defined(AFX_PROPSHEET_H__38E9E2E8_A409_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PROPSHEET_H__38E9E2E8_A409_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSheet.h : header file
//
#include "resource.h"       // main symbols
#include  "Struct.h"
#include "page1.h"
#include "page2.h"
#include "page3.h"
#include "page4.h"
#include "page5.h"
#include "page6.h"
#include "DmDlg.h";
/////////////////////////////////////////////////////////////////////////////
// CPropSheet

class CPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheet)

// Construction
public:
	CPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropSheet();
    CPage1 m_page1;
	CPage2 m_page2;
    CPage3 m_page3;
    CPage4 m_page4;
    CPage5 m_page5;
    CPage6 m_page6;
	CDmDlg m_dmpage;
	// Generated message map functions
protected:
	//{{AFX_MSG(CPropSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSHEET_H__38E9E2E8_A409_11D4_8595_00D00912B25C__INCLUDED_)
