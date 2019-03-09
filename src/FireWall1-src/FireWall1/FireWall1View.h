
// FireWall1View.h : interface of the CFireWall1View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIREWALL1VIEW_H__9D269EED_A406_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_FIREWALL1VIEW_H__9D269EED_A406_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"OptionDlg.h"
#define MAX_PACKET_SIZE  8192
#define        MAX_LINK_NAME_LENGTH   500




typedef struct _ADAPTER {

    HANDLE     hFile;

    TCHAR      SymbolicLink[MAX_LINK_NAME_LENGTH];
} ADAPTER, *LPADAPTER;

class CFireWall1View : public CScrollView
{
protected: // create from serialization only
	CFireWall1View();
	DECLARE_DYNCREATE(CFireWall1View)

// Attributes
public:
	CFireWall1Doc* GetDocument();
    COptionDlg m_OptionDlg;
	BOOL bCfgOpen;
// Operations
public:


	CImageList m_smallImageList;
	CImageList m_largeImageList;
	CListCtrl m_listView;
    CListCtrl m_listView1;
	void CreateListView();

    CONTROL_BLOCK adapter;
	HICON m_hIcon;
	void OnOpensys();
	void OnReadsys();
    void OnClosethread();
    void OnClosesys();
	BOOL PacketWriteBuffer(LPADAPTER AdapterObject, LPPACKET lpPacket, BOOLEAN Sync, PULONG BytesWrited);
	BOOL PacketReceiveBuffer(LPADAPTER AdapterObject, LPPACKET lpPacket, BOOLEAN Sync, PULONG BytesReceived);
	VOID FreeReadBuffer(PVOID buffer);
	LPPACKET AollocateReadBuffer();
	LPPACKET pReadBuffer;
	void CloseSys(LPADAPTER lpAdapter);
	HANDLE OpenAdapter(LPTSTR name);
	void GetAdpaterName(LPTSTR name);
	void OnCfgnat();
  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFireWall1View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	WORD GetDataFromPage3();
	WORD GetDataFromPage6();
	WORD GetDataFromPage5();
	WORD GetDataFromPage4();
	DWORD GetCommand(char *pbuf,int i);
	void ReadCommand(FILE *pf, char *pbuf);
	WORD GetDataFromPage1();
	WORD GetInitDataFromFile();
	void UserToKernel();
	void OptMyConfig();
	void CommandCon();
	void SetNetData();
	BOOL m_bStartFilter;
	BOOL m_bStartFirewall;
	void Startfilter();
	virtual ~CFireWall1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFireWall1View)
	afx_msg void OnOption();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnStartfilter();
	afx_msg void OnSearch();
	afx_msg void OnClosefilter();
	afx_msg void OnSetdata();
	afx_msg void OnStartfirewall();
	afx_msg void OnUpdateStartfirewall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartfilter(CCmdUI* pCmdUI);
	afx_msg void OnShutdownfirewall();
	afx_msg void OnUpdateShutdownfirewall(CCmdUI* pCmdUI);
	afx_msg void OnToolLog();
	afx_msg void OnUpdateToolLog(CCmdUI* pCmdUI);
	afx_msg void OnProtlog();
	afx_msg void OnUpdateProtlog(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOption(CCmdUI* pCmdUI);
	afx_msg void OnFilterlog();
	afx_msg void OnUpdateFilterlog(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClosefilter(CCmdUI* pCmdUI);
	afx_msg void OnMenuitem32816();
	afx_msg void OnUpdateAppExit(CCmdUI* pCmdUI);
	afx_msg void OnStopLog();
	//}}AFX_MSG
		afx_msg LRESULT OnLogView(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FireWall1View.cpp
inline CFireWall1Doc* CFireWall1View::GetDocument()
   { return (CFireWall1Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALL1VIEW_H__9D269EED_A406_11D4_8595_00D00912B25C__INCLUDED_)
