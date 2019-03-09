//Download by http://www.NewXing.com
// testddk.h : main header file for the TESTDDK application
//

#if !defined(AFX_TESTDDK_H__CFFDA486_82CD_11D4_9E49_0050BAF35242__INCLUDED_)
#define AFX_TESTDDK_H__CFFDA486_82CD_11D4_9E49_0050BAF35242__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
//#include <winbase.h>
#include "resource.h"       // main symbols
VOID ReadOutThread(VOID);
VOID ReadInThread(VOID);
VOID WriteThread(VOID);
//UINT WriteThread(LPVOID lparam);
/*
HANDLE global_handle;
OVERLAPPED myoverlap;
*/
#define MAX_PACKET_SIZE  8192
#define        MAX_LINK_NAME_LENGTH   500
typedef struct _PACKET {
    HANDLE       hEvent;
    OVERLAPPED   OverLappedread;
	OVERLAPPED   OverLappedwrite;
    PVOID        Buffer;
    UINT         Length;
} PACKET, *LPPACKET;
typedef struct _CONTROL_BLOCK
{
    PVOID           hFile;
    HANDLE          hEvent;
    TCHAR           AdapterName[128];
    ULONG           PacketLength;
    ULONG           LastReadSize;
    UINT            BufferSize;
    BYTE            PacketBuffer[MAX_PACKET_SIZE];
} CONTROL_BLOCK, *PCONTROL_BLOCK;
typedef struct _global_var{
	BYTE	Buffer[1600];
	HANDLE  hMutex;
	OVERLAPPED  overlap;
	int Flag;
}global_var,*pglobal_var;


/////////////////////////////////////////////////////////////////////////////
// CTestddkApp:
// See testddk.cpp for the implementation of this class
//
class CTestddkApp : public CWinApp
{
public:
	CTestddkApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestddkApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestddkApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDDK_H__CFFDA486_82CD_11D4_9E49_0050BAF35242__INCLUDED_)
