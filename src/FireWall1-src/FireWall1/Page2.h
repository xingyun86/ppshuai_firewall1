
#if !defined(AFX_PAGE2_H__38E9E2E3_A409_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE2_H__38E9E2E3_A409_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page2.h : header file
//
#include "winsock2.h"
#include "Struct.h"
/////////////////////////////////////////////////////////////////////////////
// CPage2 dialog

class CPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2)

// Construction
public:
	CPage2();
	~CPage2();

// Dialog Data
	//{{AFX_DATA(CPage2)
	enum { IDD = IDD_DIALOG3 };
	CEdit	m_EditAllowIpTab;
	CEdit	m_RejIpTab;
	BOOL	m_bCheckIn;
	BOOL	m_bCheckOut;
	BOOL	m_bCheckDm;
	BOOL	m_bDiscardMultcast;
	BOOL	m_bLoseOtherIp;
	BOOL	m_bLoseNonIp;
	BOOL	m_bLoseSuspectOffset;
	BOOL	m_bLoseFragementIcmp;
	BOOL	m_bLoseAttackIcmp;
	BOOL	m_bforbidIcmp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2)
	afx_msg void OnOk();
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton3();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE2_H__38E9E2E3_A409_11D4_8595_00D00912B25C__INCLUDED_)
