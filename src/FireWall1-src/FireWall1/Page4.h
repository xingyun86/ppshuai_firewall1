
#if !defined(AFX_PAGE4_H__38E9E2E5_A409_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_PAGE4_H__38E9E2E5_A409_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page4.h : header file
//
#define INITIAL_END_SEGMENT 0x40	/* Initial file blank line as end of segment */
#define NOFILTERURL 0x1	
#define FILTERANDBLOCK 0x2
#define TRANPROTOCAL 0x3
#define PORT 0x4	
#define FILTERKEYWORDS 0x8	
#define TCPFILTERBIT 0x10
#define BLOCKURL 0x12
#define JL  0x13
#define COMMENT 0x8000	/* Comment line */
#define COMMAND_UNKNOWN 0x0	/* Unknown command */
#define IDD_DIALOG5                     136
struct  ServiceManageData{
	DWORD    FilterAndBlock; //bit 0,isProComFilter;1,进行
							//bit 1, isURLBlock,1，进行，0，不进行；
	DWORD	NetProtocal;		//IF  1，IP；ELSE OTHER；
	DWORD 	TranProtocal;		//bit 0,TCP;bit 1,UDP;
	DWORD  Port;//bit 0,80;bit 1,ftp;bit 2,telnet;bit 3,smtp;
	char FilterKeyWord[200];//用以代替下面这个结构
	/*
	struct   FilterKeyWord{
						char * KeyWord;
						int	  Length;
				}UserFilterKeyWord[32];  //头三个是JAVA、SCR、ACT
				*/
	int  TCPFilterBit;		//bit 0,JAVA,=1,PERMIT;0,Filter;
							//bit 1,SCRIPT,=1,PERMIT;
							//bit 2,ACTIVEX,=1,PERMIT;
							//BIT 3,OTHER,	=1,PERMIT;
				
	int  TCPCommandBit;	//bit 0,GET
											//bit 1,PUT
											//bit 2,POST
											//bit 3,HEAD
											//bit 4,--31,保留
	int UDPFilterBit;//保留
	char BlockURLList[2048];
	char NoFilterURLList[2048];
	//用于替代下面结构
	/*
	struct  BlockURLList{		//填写要阻断URL地址；
					BlockURLList  *pNextList;
					char	*BlockURLName;
					int 		BlockURLNameLen;
				};
	struct  NoFilterURLList{	//填写不进行协议内容过滤的                     //URL地址
					NoFilterURLList  *pNextList;
					char   *NoFilterName;
					int 		 NoFilterNameLen;
	};
	*/
};
/////////////////////////////////////////////////////////////////////////////
// CPage4 dialog

class CPage4 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage4)

// Construction
public:
	void GetData();
	int GetBestData();
	void GetNameFromFile();

	void ShowInitWindow();
	WORD GetCommand(char *pbuf);
	void ReadCommand(FILE *pf,char  *pbuf);
	WORD GetDataFromFile();
	CPage4();
	~CPage4();

// Dialog Data
	//{{AFX_DATA(CPage4)
	enum { IDD = IDD_DIALOG5 };
	CEdit	m_edit_urlct;
	CEdit	m_edit_otherkeyct;
	CEdit	m_edit_nofilterurlct;
	BOOL	m_check_activex;
	BOOL	m_check_ftp;
	BOOL	m_check_http;
	BOOL	m_check_ispro;
	BOOL	m_check_isurl;
	BOOL	m_check_java;
	BOOL	m_check_other;
	BOOL	m_check_script;
	BOOL	m_check_smtp;
	BOOL	m_check_tcp;
	BOOL	m_check_telnet;
	BOOL	m_check_udp;
	CString	m_edit_nofilterurl;
	CString	m_edit_otherkey;
	CString	m_edit_url;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage4)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage4)
	afx_msg void OnButtonOk();
	afx_msg void OnCheckActivex();
	afx_msg void OnCheckHttp();
	afx_msg void OnCheckIspro();
	afx_msg void OnCheckIsurl();
	afx_msg void OnCheckJava();
	afx_msg void OnCheckOther();
	afx_msg void OnCheckScript();
	afx_msg void OnCheckTcp();
	afx_msg void OnCheckSmtp();
	afx_msg void OnCheckTelnet();
	afx_msg void OnCheckUdp();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE4_H__38E9E2E5_A409_11D4_8595_00D00912B25C__INCLUDED_)
