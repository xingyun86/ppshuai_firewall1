
#if !defined(AFX_PAGE10_H__85922A63_A4E2_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE10_H__85922A63_A4E2_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page10.h : header file
//
#include "winsock2.h"
#include "Struct.h"
/////////////////////////////////////////////////////////////////////////////
// CPage10 dialog

class CPage10 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage10)

// Construction
public:
	CPage10();
	~CPage10();

// Dialog Data
	//{{AFX_DATA(CPage10)
	enum { IDD = IDD_DIALOG10 };
	CEdit	m_TcpDstOut;
	CEdit	m_TcpSrcOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage10)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage10)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE10_H__85922A63_A4E2_11D4_8595_00D00912B25C__INCLUDED_)
