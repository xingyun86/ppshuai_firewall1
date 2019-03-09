
#if !defined(AFX_PAGE11_H__85922A64_A4E2_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE11_H__85922A64_A4E2_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page11.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage11 dialog

class CPage11 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage11)

// Construction
public:
	CPage11();
	~CPage11();

// Dialog Data
	//{{AFX_DATA(CPage11)
	enum { IDD = IDD_DIALOG11 };
	CEdit	m_UdpDstOut;
	CEdit	m_UdpSrcOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage11)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage11)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE11_H__85922A64_A4E2_11D4_8595_00D00912B25C__INCLUDED_)
