
#if !defined(AFX_PAGE1_H__38E9E2E2_A409_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE1_H__38E9E2E2_A409_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page1.h : header file
//
#define INITIAL_END_SEGMENT 0x40	/* Initial file blank line as end of segment */
#define INNET 0x1	
#define OUTNET 0x2
#define DM 0x3
#define INNETMASK 0x4	
#define JL  0x13
#define COMMENT 0x8000	/* Comment line */
#define COMMAND_UNKNOWN 0x0	/* Unknown command */
/////////////////////////////////////////////////////////////////////////////
// CPage1 dialog

class CPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1)

// Construction
public:
	int GetBestData();
	WORD GetCommand(char *pbuf);
	void ReadCommand(FILE *pf, char *pbuf);
	WORD GetDataFromFile();

	void GetNetCardIP(LPBYTE pStr);
	void GetNetCardName();
	CPage1();
	~CPage1();

// Dialog Data
	//{{AFX_DATA(CPage1)
	enum { IDD = IDD_DIALOG2 };
	CEdit	m_StaticNatMap_tcp;
	CEdit	m_StaticNatMap_udp;
	CComboBox	m_sel_protocal_ct;
	CComboBox	m_SelAdaptDm;
	CComboBox	m_SelAdaptInNet;
	CComboBox	m_SelAdaptOutNet;
	CString	m_OutNetIp;
	CString	m_OutNetMask;
	CString	m_InNetIp;
	CString	m_InNetMask;
	CString	m_DmIp;
	CString	m_DmMask;
	UINT	m_UdpTimeOut;
	UINT	m_PortEnd;
	UINT	m_MaxPortNum;
	UINT	m_PortBegin;
	UINT	m_TcpTimeOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnSelchangeCombo4();
	afx_msg void OnSelchangeCombo3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE1_H__38E9E2E2_A409_11D4_8595_00D00912B25C__INCLUDED_)
