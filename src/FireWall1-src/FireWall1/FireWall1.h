
// FireWall1.h : main header file for the FIREWALL1 application
//

#if !defined(AFX_FIREWALL1_H__9D269EE5_A406_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_FIREWALL1_H__9D269EE5_A406_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

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
#define MYWN_NOTIFYICON WM_USER+111

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
typedef struct _NetCardProp{
	BYTE	name[20];//Íø¿¨Ãû
	int		DeviceNo;//Éè±¸ºÅ
	BYTE	IP[20];
	BYTE	IPMASK[20];
	int		Flag;//0,in,1,out,2,dm
}NetCardProp,*pNetCardProp;
class CFireWall1App : public CWinApp
{
public:
	void ReadCommand(FILE *pf, char *pbuf);
	CFireWall1App();
	BOOL MyTaskAddIcon(HWND hwnd,UINT uID,HICON hicon,LPSTR lpszTip);
	void* hMutexOneInstance;
	BOOL bFound;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFireWall1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFireWall1App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALL1_H__9D269EE5_A406_11D4_8595_00D00912B25C__INCLUDED_)
