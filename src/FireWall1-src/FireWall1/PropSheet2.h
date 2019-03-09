
#if !defined(AFX_PROPSHEET2_H__85922A67_A4E2_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PROPSHEET2_H__85922A67_A4E2_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSheet2.h : header file
//
#include "resource.h"       // main symbols
//#include "page8.h"
//#include "page9.h"
#include "page10.h"
#include "page11.h"
#include "page12.h"
#include "page13.h"
/////////////////////////////////////////////////////////////////////////////
// CPropSheet2

class CPropSheet2 : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheet2)

// Construction
public:
	CPropSheet2(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheet2(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
 
   CPage10 m_page10;
   CPage12 m_page12;
   CPage11 m_page11;
   CPage13 m_page13;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSheet2)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropSheet2();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropSheet2)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSHEET2_H__85922A67_A4E2_11D4_8595_00D00912B25C__INCLUDED_)
