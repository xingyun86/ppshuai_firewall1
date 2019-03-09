
#if !defined(AFX_HTTPDLG_H__870742A2_B553_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_HTTPDLG_H__870742A2_B553_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// httpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDmDlg dialog

class CDmDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CDmDlg)

// Construction
public:
	CDmDlg();
	~CDmDlg();

// Dialog Data
	//{{AFX_DATA(CDmDlg)
	enum { IDD = IDD_DIALOG16 };
	CEdit	m_AllowIp;
	CEdit	m_RejIp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDmDlg)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPDLG_H__870742A2_B553_11D4_8595_00D00912B25C__INCLUDED_)
