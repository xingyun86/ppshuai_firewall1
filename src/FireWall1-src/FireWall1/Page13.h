
#if !defined(AFX_PAGE13_H__85922A66_A4E2_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE13_H__85922A66_A4E2_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page13.h : header file
//
#include"winsock.h"
#include"Struct.h"
/////////////////////////////////////////////////////////////////////////////
// CPage13 dialog

class CPage13 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage13)

// Construction
public:
	BOOL m_bSel;
	CPage13();
	~CPage13();

// Dialog Data
	//{{AFX_DATA(CPage13)
	enum { IDD = IDD_DIALOG13 };
	CListBox	m_HostList;
	CString	m_HostIp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage13)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage13)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE13_H__85922A66_A4E2_11D4_8595_00D00912B25C__INCLUDED_)
