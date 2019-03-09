
#if !defined(AFX_PAGE6_H__38E9E2E7_A409_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE6_H__38E9E2E7_A409_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page6.h : header file
#include"Struct.h"
//
#define INITIAL_END_SEGMENT 0x40	/* Initial file blank line as end of segment */
#define REQUIRELOG 0x1	
#define ISLOGIP 0x2
#define IPOPTIONS 0x3
#define ISLOGPROTOCAL 0x4	
#define PROOPTIONS 0x8	
#define JL  0x13
#define COMMENT 0x8000	/* Comment line */
#define COMMAND_UNKNOWN 0x0	/* Unknown command */

/////////////////////////////////////////////////////////////////////////////
// CPage6 dialog


/////////////////////////////////////////////////////////////////////////////
// CPage6 dialog

class CPage6 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage6)

// Construction
public:
	void GetData();
	void ShowInitWindow();
	DWORD GetCommand(char *pbuf);
	void ReadCommand(FILE *pf, char *pbuf);
	DWORD GetDataFromFile();
	CPage6();
	~CPage6();

// Dialog Data
	//{{AFX_DATA(CPage6)
	enum { IDD = IDD_DIALOG7 };
	BOOL	m_check_incommingdos;
	BOOL	m_check_incommingport;
	BOOL	m_check_incommingip;
	BOOL	m_check_incommingtype;
	BOOL	m_check_islog;
	BOOL	m_check_logip;
	BOOL	m_check_logipport;
	BOOL	m_check_logkind;
	BOOL	m_check_logtime;
	BOOL	m_check_logurl;
	BOOL	m_check_outgoingdos;
	BOOL	m_check_outgoingip;
	BOOL	m_check_outgoingport;
	BOOL	m_check_outgoingtype;
	BOOL	m_check_prolog;
	BOOL	m_check_inbroad;
	BOOL	m_check_inlength;
	BOOL	m_check_inmore;
	BOOL	m_check_inoffset;
	BOOL	m_check_outbroad;
	BOOL	m_check_outlength;
	BOOL	m_check_outmore;
	BOOL	m_check_outoffset;
	BOOL	m_check_route;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage6)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage6)
	afx_msg void OnButtonOk();
	afx_msg void OnCHECKincommingDOS();
	afx_msg void OnCHECKincommingIP();
	afx_msg void OnCHECKincommingPort();
	afx_msg void OnCHECKincommingType();
	afx_msg void OnCheckIslog();
	afx_msg void OnCheckLogip();
	afx_msg void OnCheckLogipport();
	afx_msg void OnCheckLogkind();
	afx_msg void OnCheckLogtime();
	afx_msg void OnCheckLogurl();
	afx_msg void OnCHECKoutgoingDOS();
	afx_msg void OnCHECKoutgoingIP();
	afx_msg void OnCHECKoutgoingPort();
	afx_msg void OnCHECKoutgoingType();
	afx_msg void OnCheckProlog();
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCHECKInBroad();
	afx_msg void OnCHECKOutBroad();
	afx_msg void OnCHECKInLength();
	afx_msg void OnCHECKOutLength();
	afx_msg void OnCHECKInOffset();
	afx_msg void OnCHECKOutOffset();
	afx_msg void OnCHECKInMore();
	afx_msg void OnCHECKOutMore();
	afx_msg void OnCheckSorceRoute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE6_H__38E9E2E7_A409_11D4_8595_00D00912B25C__INCLUDED_)
