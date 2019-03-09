//Download by http://www.NewXing.com
if !defined(AFX_PAGE99_H__BDAC6267_A8C2_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE99_H__BDAC6267_A8C2_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page99.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage99 dialog

class CPage99 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage99)

// Construction
public:
	CPage99();
	~CPage99();

// Dialog Data
	//{{AFX_DATA(CPage99)
	enum { IDD = IDD_DIALOG9 };
	CEdit	m_FtpDstOut;
	CEdit	m_FtpDstIn;
	CEdit	m_FtpSrcOut;
	CEdit	m_FtpSrcIn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage99)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage99)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE99_H__BDAC6267_A8C2_11D4_8595_00D00912B25C__INCLUDED_)
