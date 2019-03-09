
#if !defined(AFX_PAGE3_H__38E9E2E4_A409_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE3_H__38E9E2E4_A409_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page3.h : header file
//
#include "Struct.h"
#define INITIAL_END_SEGMENT 0x40	/* Initial file blank line as end of segment */
#define PRIORITY 0x1	
#define CHECKINOROUTNETCARD 0x2
#define PRO 0x3
#define TCPPROTOCAL 0x4	
#define PORTOCALCOMMENTS 0x8	
#define UDPPROTOCAL 0x10
#define JL  0x13
#define COMMENT 0x8000	/* Comment line */
#define COMMAND_UNKNOWN 0x0	/* Unknown command */

/////////////////////////////////////////////////////////////////////////////
// CPage3 dialog

class CPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3)

// Construction
public:
	struct GlobalFlags m_globFlags;
		DWORD theDefaultPri;
	DWORD GetCommand(char *pbuf);
	void ReadCommand(FILE *pf, char *pbuf);
	DWORD GetDataFromFile();
	CPage3();
	~CPage3();

// Dialog Data
	//{{AFX_DATA(CPage3)
	enum { IDD = IDD_DIALOG4 };
	BOOL	m_check_activex;
	BOOL	m_check_comein;
	BOOL	m_check_dns;
	BOOL	m_check_finger;
	BOOL	m_check_goout;
	BOOL	m_check_intoout1024l;
	BOOL	m_check_intoout1024s;
	BOOL	m_check_intoout80;
	BOOL	m_check_intooutftp;
	BOOL	m_check_intoouticmp;
	BOOL	m_check_intooutigmp;
	BOOL	m_check_intooutnbt;
	BOOL	m_check_intooutsnmp;
	BOOL	m_check_intoouttelnet;
	BOOL	m_check_java;
	BOOL	m_check_outtoin1024l;
	BOOL	m_check_outtoin1024s;
	BOOL	m_check_outtoin80;
	BOOL	m_check_outtoinftp;
	BOOL	m_check_outtoinicmp;
	BOOL	m_check_outtoinigmp;
	BOOL	m_check_outtoinnbt;
	BOOL	m_check_outtoinsnmp;
	BOOL	m_check_outtointelnet;
	BOOL	m_check_script;
	int		m_radio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3)
	afx_msg void OnButtonOk();
	afx_msg void OnRadioHigh();
	afx_msg void OnRadioLow();
	afx_msg void OnRadioMiddle();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE3_H__38E9E2E4_A409_11D4_8595_00D00912B25C__INCLUDED_)
