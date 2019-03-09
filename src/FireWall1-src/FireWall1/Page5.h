
#if !defined(AFX_PAGE5_H__38E9E2E6_A409_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE5_H__38E9E2E6_A409_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page5.h : header file
//
#define INITIAL_END_SEGMENT 0x40	/* Initial file blank line as end of segment */
#define ISUSERVERIFY 0x1	
#define TIME 0x2
#define USERNAMEANDPASSWD 0x3
#define JL  0x13
#define COMMENT 0x8000	/* Comment line */
#define COMMAND_UNKNOWN 0x0	/* Unknown command */
#define IDD_DIALOG6                     137

/////////////////////////////////////////////////////////////////////////////
// CPage5 dialog
struct UserVerifyData{
	BOOL IsVerify;		//1,需要进行用户验证管理；0，不进行；
	int  timeRefesh;  //刷新时间；
	char UserList[9182];
	/*
	struct UserList{
		char *UserName;
		int  UserNameLen;
		char *UserPass;
		int  UserPassLen;
		UserList *pnext;
};*/
};		//用户验证列表
/////////////////////////////////////////////////////////////////////////////
// CPage5 dialog

class CPage5 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage5)

// Construction
public:
	void GetData();
	void GetNameFromFile();
		WORD GetCommand(char *pbuf);
	void ReadCommand(FILE *pf,char *pbuf);
	DWORD GetDataFromFile();
	CPage5();
	~CPage5();

// Dialog Data
	//{{AFX_DATA(CPage5)
	enum { IDD = IDD_DIALOG6 };
	BOOL	m_check_isuserver;
	CString	m_edit_passwd;
	int		m_edit_time;
	CString	m_edit_user;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage5)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage5)
	afx_msg void OnButtonAdduser();
	afx_msg void OnButtonDeluser();
	afx_msg void OnButtonOk();
	afx_msg void OnCheckIsuserver();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE5_H__38E9E2E6_A409_11D4_8595_00D00912B25C__INCLUDED_)
