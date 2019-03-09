
#if !defined(AFX_LOGDLG_H__A9A96064_C69A_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_LOGDLG_H__A9A96064_C69A_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog

class CLogDlg : public CDialog
{
// Construction
public:
	CLogDlg(CWnd* pParent = NULL);   // standard constructor
	// Dialog Data
	//{{AFX_DATA(CLogDlg)
	enum { IDD = IDD_DIALOG19 };
	BOOL	m_bFilterLog;
	BOOL	m_bProtLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGDLG_H__A9A96064_C69A_11D4_8595_00D00912B25C__INCLUDED_)
