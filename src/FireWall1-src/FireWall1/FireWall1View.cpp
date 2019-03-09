
// FireWall1View.cpp : implementation of the CFireWall1View class
//

#include "stdafx.h"
#include "FireWall1.h"
#include "Struct.h"
#include "ip.h"
#include "global.h"
#include "config.h"
#include "outnat.h"
//#include "ipfilter.h"
#include "authentication.h"
#include "OptionDlg.h"
#include "LogDlg.h"
#include "Propsheet.h"
#include "FireWall1Doc.h"
#include "FireWall1View.h"
#include "page1.h"
#include "page3.h"
#include "page4.h"
#include "page5.h"
#include "page6.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 BOOL iiii=TRUE;
BOOL isFirst=TRUE;
HWND myhWnd1;
extern char tmpin[5];//用于存取网卡设备号
extern char tmpout[5];
extern char tmpdm[5];
extern BYTE INNO;
extern BYTE OUTNO;
extern BYTE DMNO;
CDataConvert gDataConvert;
extern  VOID SetFilterInitReq(struct FireWallConfig  mycfg);
extern  VOID SetFilterlogMask();
extern  int SetFilterRejTab(CPtrList &listRejIp);
extern  int SetFilterAcceptTab(CPtrList &listRejIp);
extern  VOID SetFilterAddrTree(struct IpaddrRange iprange,USHORT group);
extern  VOID SetFilterPortList(struct 	HostList* phostlist,UCHAR type,unsigned short group);
extern  VOID SetFilterCfg(struct FireWallConfig  mycfg,struct GlobalFlags myflags);
extern  VOID StartFilter();
extern  USHORT treeLookup (addrTreeNode *tbl, ULONG addr, unsigned short max_group);
extern  VOID mysearch();
extern  VOID clearipfilter();
extern  int SetDmIpTab(CPtrList &listRejIp,UCHAR type);
extern  VOID GetStatisticsInfo(Statistics* StatisticInfo);
extern  VOID ClearStatisticsInfo();

extern void ClearNatUseResoure();
extern void NatCfg();

extern LogManage MyLogManage;
LogManage TheLogManage;
extern UserVerifyData MyUserVerifyData;
UserVerifyData TheUserVerifyData;
extern DefaultSetData TheDefaultSetData;
extern ServiceManageData MyServiceManageData;
ServiceManageData TheServiceManageData;
BOOL CloseAll;//用于表示程序要关闭
DWORD dwThreadId_in;
DWORD dwThreadId_out;
DWORD dwThreadId_write;
HANDLE global_handle,global_handle1;
BOOL Read_Open;
HANDLE mythread_in;
HANDLE mythread_out;
HANDLE mythread_write;
extern NATTABLE_ITEM*     nattable;
extern NetCardProp myNetCardProp[8]; 
struct  NAT_Cfg pnatcfg;
in_addr_t OutGlobalAddr;
BOOL m_binit=FALSE;
BOOL bAppClose=FALSE;
BOOL bPropsheetOpen=FALSE;
//extern FILE *hlogFile;
USHORT logType=0;
USHORT StoplogType=0;
/////////////////////////////////////////////////////////////////////////////
// CFireWall1View
extern BYTE INNO;
extern BYTE OUTNO;
extern BYTE DMNO;
BOOL flag = FALSE;
int number=-1;
#define MAXITEMNUMBER 20
LV_ITEM lvItem;
LV_ITEM lvItem1;
//   CListCtrl m_listView;
 //   CListCtrl m_listView1;
FILE * pfoutDeviceInit;
FILE * pfoutServiceInit;
FILE * pfoutUserverInit;
FILE * pfoutLogsetInit;
FILE * pfoutDefaultInit;
IMPLEMENT_DYNCREATE(CFireWall1View, CScrollView)
char NETMASK[20];//内网子网掩码
BEGIN_MESSAGE_MAP(CFireWall1View, CScrollView)
	//{{AFX_MSG_MAP(CFireWall1View)
	ON_COMMAND(ID_OPTION, OnOption)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_STARTFILTER, OnStartfilter)
	ON_COMMAND(ID_SEARCH, OnSearch)
	ON_COMMAND(ID_CLOSEFILTER, OnClosefilter)
	ON_COMMAND(ID_SETDATA, OnSetdata)
	ON_COMMAND(ID_STARTFIREWALL, OnStartfirewall)
	ON_UPDATE_COMMAND_UI(ID_STARTFIREWALL, OnUpdateStartfirewall)
	ON_UPDATE_COMMAND_UI(ID_STARTFILTER, OnUpdateStartfilter)
	ON_COMMAND(ID_SHUTDOWNFIREWALL, OnShutdownfirewall)
	ON_UPDATE_COMMAND_UI(ID_SHUTDOWNFIREWALL, OnUpdateShutdownfirewall)
	ON_COMMAND(ID_TOOL_LOG, OnToolLog)
	ON_UPDATE_COMMAND_UI(ID_TOOL_LOG, OnUpdateToolLog)
	ON_COMMAND(ID_PROTLOG, OnProtlog)
	ON_UPDATE_COMMAND_UI(ID_PROTLOG, OnUpdateProtlog)
	ON_UPDATE_COMMAND_UI(ID_OPTION, OnUpdateOption)
	ON_COMMAND(ID_FILTERLOG, OnFilterlog)
	ON_UPDATE_COMMAND_UI(ID_FILTERLOG, OnUpdateFilterlog)
	ON_UPDATE_COMMAND_UI(ID_EXIT, OnUpdateExit)
	ON_UPDATE_COMMAND_UI(ID_CLOSEFILTER, OnUpdateClosefilter)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateAppExit)
	ON_COMMAND(ID_STOP_LOG, OnStopLog)
	//}}AFX_MSG_MAP
	ON_MESSAGE(M_LOGVIEW,OnLogView)
END_MESSAGE_MAP()
	
/////////////////////////////////////////////////////////////////////////////
// CFireWall1View construction/destruction

CFireWall1View::CFireWall1View()
{
	// TODO: add construction code here
	memset(NETMASK,0,20);
	if(!m_binit)
	{
	Read_Open=FALSE;
    memset(&adapter,0,sizeof(CONTROL_BLOCK));
//	pnatcfg=new struct NAT_Cfg;
	bCfgOpen=FALSE;
	}
	CloseAll=FALSE;
	memset(tmpin,0,5);
	memset(tmpout,0,5);
	memset(tmpdm,0,5);
}

CFireWall1View::~CFireWall1View()
{
	if(!bAppClose)
	{
	if(m_bStartFirewall)
	{
      if((DWORD)pnatcfg.StaticTable.item!=NULL)
      delete []pnatcfg.StaticTable.item;
	  ClearNatUseResoure();
	  bAppClose=TRUE;
	}
	}
}

BOOL CFireWall1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFireWall1View drawing

void CFireWall1View::OnDraw(CDC* pDC)
{
	CFireWall1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CFireWall1View diagnostics

#ifdef _DEBUG
void CFireWall1View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFireWall1View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFireWall1Doc* CFireWall1View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFireWall1Doc)));
	return (CFireWall1Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFireWall1View message handlers

void CFireWall1View::OnOption() 
{ 
	
    bCfgOpen=TRUE;
	CPropSheet propSheet("防火墙设置", this, 0);
    int result = propSheet.DoModal();
    if (result == IDOK)
    {
//		memcpy((void *)&TheLogManage,(void *)&MyLogManage,sizeof(LogManage));
      OnCfgnat();
	  UserToKernel();
	  clearipfilter();
      m_bStartFilter=FALSE;
      Startfilter();
      m_bStartFilter=TRUE;
    }

	bCfgOpen=FALSE;
}

int CFireWall1View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetInitDataFromFile();
	if(!m_binit)
	{
    m_bStartFirewall=FALSE;
    m_bStartFilter=FALSE;
	//从文件中读取配置
	gDataConvert.ReadFromFile();
	//转换到主机结构中
    gDataConvert.InitHostList();
	//转换到firewall与全局结构中
    gDataConvert.InitFireWall();
	OutGlobalAddr.ss_addr=gDataConvert.m_firewallconfig.nOutsideIp;
	CEdit noused;
    gDataConvert.AddIpTable(INITALLOWIPTAB,noused);
	gDataConvert.AddIpTable(INITREJIPTAB,noused);
	gDataConvert.AddIpTable(INITOUTDMTAB,noused);
	gDataConvert.AddIpTable(INITINDMTAB,noused);

    bCfgOpen=TRUE;
	CPropSheet propSheet("防火墙设置", this, 0);
    int result = propSheet.DoModal();
//    myhWnd1=m_hWnd;
//    if (result == IDOK)
//    {
	  UserToKernel();
		
      OnOpensys();
	  SetNetData();//以后写在配置命令发生器
//      gDataConvert.SetNatCfg(&pnatcfg);
      OnCfgnat();
	  NatCfg();
	  ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TimeBeginThread,NULL,0,NULL);
	  OnReadsys();
      Startfilter();
      m_bStartFirewall=TRUE;
      m_bStartFilter=TRUE; 
//	}
	bCfgOpen=FALSE; 
	m_binit=TRUE;
	myhWnd1=m_hWnd;
	}
	return 0;
}



void CFireWall1View::OnDestroy() 
{
  CScrollView::OnDestroy();
  if(!bAppClose)
  {
   gDataConvert.WriteToFile();
   clearipfilter();
  }
}





void CFireWall1View::OnStartfilter() 
{
   Startfilter();
   m_bStartFilter=TRUE;
}

void CFireWall1View::OnSearch() 
{
//  mysearch();
//	USHORT RET=ntohs(0xFF1F);
				
}

void CFireWall1View::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 100;
	sizeTotal.cy = 300;
	SetScrollSizes(MM_TEXT, sizeTotal);
	CreateListView();

}

void CFireWall1View::CreateListView()
{
	m_smallImageList.Create(16,16,FALSE,1,0);
	m_largeImageList.Create(32,32,FALSE,1,0);
	HICON hIcon = ::LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_ICON1));
	m_smallImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_ICON2));
	m_largeImageList.Add(hIcon);
	RECT rect;
	GetClientRect(&rect);
	rect.bottom = 768;
	rect.right = 1024;
	int row = 0,col = 0;
	lvItem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	lvItem.iImage = 0;
	lvItem.iItem = -1;
	lvItem.iSubItem = 0;

	row = 0,col=0;
	m_listView1.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT,
		rect,this/*m_wndSplitter.GetActivePane(&row,&col)*/,IDC_LISTVIEW);
	m_listView1.SetImageList(&m_smallImageList,LVSIL_SMALL);
	m_listView1.SetImageList(&m_largeImageList,LVSIL_NORMAL);
	//Create the column
	LV_COLUMN lvColumn1;
	lvColumn1.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn1.fmt = LVCFMT_CENTER;
	lvColumn1.cx = 135;
	lvColumn1.pszText = "时间";
	m_listView1.InsertColumn(0,&lvColumn1);
	lvColumn1.pszText = "协议";
	m_listView1.InsertColumn(1,&lvColumn1);
	lvColumn1.pszText = "源地址(:端口)";
	m_listView1.InsertColumn(2,&lvColumn1);
	lvColumn1.pszText = "目的地址(:端口)";
	m_listView1.InsertColumn(3,&lvColumn1);
	lvColumn1.pszText = "说明";
	m_listView1.InsertColumn(4,&lvColumn1);
	lvColumn1.pszText = "ICMP类型";	
	m_listView1.InsertColumn(5,&lvColumn1);
	
	lvItem1.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;	  
}
void CFireWall1View::OnOpensys() 
{

	// TODO: Add your command handler code here
	GetAdpaterName(adapter.AdapterName);
	//上一句调用可以省略，应为他获得适配器的符号连接
	adapter.BufferSize = MAX_PACKET_SIZE;
	AollocateReadBuffer();
    adapter.hFile = OpenAdapter(adapter.AdapterName);

}

void CFireWall1View::GetAdpaterName(LPTSTR pStr)
{
    HKEY            SystemKey;
    HKEY            ControlSetKey;
    HKEY            ServicesKey;
    HKEY            NdisPerfKey;
    HKEY            LinkageKey;
    LONG            Status;

    DWORD           RegType;
    ULONG BufferSize;
	BufferSize=256;//128;

    Status = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            TEXT("SYSTEM"),
            0,
            KEY_READ,
            &SystemKey
            );

    if (Status == ERROR_SUCCESS)
    {

        Status = RegOpenKeyEx(
                SystemKey,
                TEXT("CurrentControlSet"),
                0,
                KEY_READ,
                &ControlSetKey
                );

        if (Status == ERROR_SUCCESS)
        {

            Status = RegOpenKeyEx(
                    ControlSetKey,
                    TEXT("Services"),
                    0,
                    KEY_READ,
                    &ServicesKey
                    );

            if (Status == ERROR_SUCCESS)
            {

                Status = RegOpenKeyEx(
                        ServicesKey,
                        TEXT("ImSamp"),
                        0,
                        KEY_READ,
                        &NdisPerfKey
                        );

                if (Status == ERROR_SUCCESS)
                {

                    Status = RegOpenKeyEx(
                            NdisPerfKey,
                            TEXT("Linkage"),
                            0,
                            KEY_READ,
                            &LinkageKey
                            );


                    if (Status == ERROR_SUCCESS)
                    {

                        Status = RegQueryValueEx(
                                LinkageKey,
                                TEXT("Export"),
                                NULL,
                                &RegType,
                                (LPBYTE) pStr,
                                &BufferSize
                                );


                        RegCloseKey(LinkageKey);
                    }

                    RegCloseKey(NdisPerfKey);
                }

                RegCloseKey(ServicesKey);
            }

            RegCloseKey(ControlSetKey);
        }

        RegCloseKey(SystemKey);
    }


    return ;
}

HANDLE CFireWall1View::OpenAdapter(LPTSTR name)
{
           LPADAPTER       lpAdapter;
    ULONG           Error;

    TRACE(_T("OpenAdapter\n"));

    HGLOBAL hMem = GlobalAlloc(
            GMEM_MOVEABLE | GMEM_ZEROINIT,
            sizeof(ADAPTER));
    lpAdapter=(LPADAPTER) GlobalLock(hMem);

    if (lpAdapter == NULL)
    {

        AfxMessageBox(_T("OpenAdapter GlobalAlloc Failed\n"));

        return NULL;

    }

	memset(name,0,128);
//	wcscpy(name,_T("\\Device\\ImSamp"));
    memcpy(name,_T("\\Device\\ImSamp"),40);
	//因为在SYS中符号连接指明为“\\Device\\ImSamp”
    wsprintf(
            lpAdapter->SymbolicLink,
            TEXT("\\\\.\\%s"),
            &name[8]);
    
    lpAdapter->hFile = CreateFile(lpAdapter->SymbolicLink,
            GENERIC_WRITE | GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,
            0);
	global_handle1=lpAdapter->hFile;
	BOOL DUP=DuplicateHandle(GetCurrentProcess(),
		lpAdapter->hFile,
		GetCurrentProcess(),
		&global_handle,
		NULL,
		FALSE,
		DUPLICATE_SAME_ACCESS);
	DWORD aa=GetLastError();
    if (lpAdapter->hFile != INVALID_HANDLE_VALUE)
	{

	    return lpAdapter;
	}

    Error = GetLastError();
	
    AfxMessageBox(_T("OpenAdapter Could not open adapter\n"));
    
	GlobalUnlock(lpAdapter);
    GlobalFree(lpAdapter);

    return NULL;
}


void CFireWall1View::OnReadsys() 
{       
	// TODO: Add your command handler code here
	CloseAll=0;
	pReadBuffer->Buffer=adapter.PacketBuffer;
    pReadBuffer->Length=adapter.BufferSize;
	mythread_in=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReadInThread,NULL,0,&dwThreadId_in);//(LPTHREAD_START_ROUTINE)
	mythread_out=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReadOutThread,NULL,0,&dwThreadId_out);//(LPTHREAD_START_ROUTINE)
	mythread_write=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)WriteThread,NULL,0,&dwThreadId_write);
	SetThreadPriority(mythread_write,THREAD_PRIORITY_ABOVE_NORMAL);
	if((mythread_in!=NULL)&&(mythread_out!=NULL))Read_Open=TRUE;
	TRACE(_T("BGING"));

}

void CFireWall1View::CloseSys(LPADAPTER lpAdapter)
{
	if(lpAdapter)
	{
      if(lpAdapter->hFile)
	  {  
		  BOOL ok=CloseHandle(lpAdapter->hFile);
        
	      GlobalUnlock(lpAdapter);
          GlobalFree(lpAdapter);
	  }
	}
}



LPPACKET CFireWall1View::AollocateReadBuffer()
{
    //LPPACKET buffer;     
    HGLOBAL hMem  = GlobalAlloc(
            GMEM_MOVEABLE | GMEM_ZEROINIT,
            sizeof(PACKET));
	int a=sizeof(PACKET);
    pReadBuffer=(LPPACKET)GlobalLock(hMem);

    if (pReadBuffer == NULL)
    {

        AfxMessageBox(_T("AllocatePacket: GlobalAlloc Failed\n"));

        return NULL;

    }

    pReadBuffer->OverLappedread.hEvent = CreateEvent(
            NULL,
            TRUE,
            FALSE,
            NULL);



    #if defined _DEBUG
    ULONG err=GetLastError();
    #endif

    if (pReadBuffer->OverLappedread.hEvent == NULL)
    {

        AfxMessageBox(_T("AllocatePacket: CreateEvent Failed\n"));
        
		GlobalUnlock(pReadBuffer);
        GlobalFree(pReadBuffer);

        return NULL;
    }

	 pReadBuffer->OverLappedwrite.hEvent = CreateEvent(
            NULL,
            TRUE,
            FALSE,
            NULL);



    #if defined _DEBUG
    err=GetLastError();
    #endif

    if (pReadBuffer->OverLappedwrite.hEvent == NULL)
    {

        AfxMessageBox(_T("AllocatePacket: CreateEvent Failed\n"));
        
		GlobalUnlock(pReadBuffer);
        GlobalFree(pReadBuffer);

        return NULL;
    }

     
    return pReadBuffer;
}

VOID CFireWall1View::FreeReadBuffer(PVOID buffer)
{
    CloseHandle(((LPPACKET)buffer)->OverLappedread.hEvent);
	CloseHandle(((LPPACKET)buffer)->OverLappedwrite.hEvent);

    GlobalFree(buffer);
}


BOOL CFireWall1View::PacketReceiveBuffer(LPADAPTER AdapterObject, LPPACKET lpPacket, BOOLEAN Sync, PULONG BytesReceived)
{
     
    lpPacket->OverLappedread.Offset = 0;
    lpPacket->OverLappedread.OffsetHigh = 0;
/*
    if (!ResetEvent(lpPacket->OverLappedread.hEvent))
    {
        #if defined _DEBUG
        ULONG err=GetLastError();
        #endif
        return FALSE;

    }
	*/  

	
/*
    Result = ReadFile(
            AdapterObject->hFile,
            lpPacket->Buffer,
            lpPacket->Length,
            BytesReceived,
            &lpPacket->OverLappedread);

    #if defined _DEBUG
    ULONG err=GetLastError();
    #endif 

    if (Sync)
    {

        Result = GetOverlappedResult(
                AdapterObject->hFile,
                &lpPacket->OverLappedread,
                BytesReceived,
                TRUE);
        
		#if defined _DEBUG
        ULONG err=GetLastError();
        #endif

        TRACE(_T("RECIEVE OK"));
    }
    else
    {

        Result = TRUE;

    }
*/
    return TRUE;
}

BOOL CFireWall1View::PacketWriteBuffer(LPADAPTER AdapterObject, LPPACKET lpPacket, BOOLEAN Sync, PULONG BytesWrited)
{
	BOOL            Result;

    lpPacket->OverLappedwrite.Offset = 0;
    lpPacket->OverLappedwrite.OffsetHigh = 0;

    if (!ResetEvent(lpPacket->OverLappedwrite.hEvent))
    {
        #if defined _DEBUG
        ULONG err=GetLastError();
        #endif
        return FALSE;

    }

    Result = WriteFile(
            AdapterObject->hFile,
            lpPacket->Buffer,
            lpPacket->Length,
            BytesWrited,
            &lpPacket->OverLappedwrite);

    #if defined _DEBUG
    ULONG err=GetLastError();
    #endif 

    if (Sync)
    {

        Result = GetOverlappedResult(
                AdapterObject->hFile,
                &lpPacket->OverLappedwrite,
                BytesWrited,
                TRUE);
        
		#if defined _DEBUG
        ULONG err=GetLastError();
        #endif

//        TRACE(_T("Write OK"));
    }
    else
    {

        Result = TRUE;

    }

    return Result;
   
}


void CFireWall1View::OnClosethread() 
{
	// TODO: Add your command handler code here
	CloseAll=TRUE;   
	//CloseSys((ADAPTER* )adapter.hFile);
}

void CFireWall1View::OnClosesys() 
{
	// TODO: Add your command handler code here
	BOOL ok;
	CloseSys((ADAPTER* )adapter.hFile);
	ok=CloseHandle(global_handle);
}

void CFireWall1View::OnClosefilter() 
{
	clearipfilter();
    m_bStartFilter=FALSE;
}

void CFireWall1View::OnSetdata() 
{
	// TODO: Add your command handler code here
	ULONG           BytesReturned;	
	BYTE  temp[40];
	memset(temp,0,40);
	int no;
	for(int i=0;i<8;i++)
	{
		no=myNetCardProp[i].DeviceNo;
		if(no)
		{
			if(myNetCardProp[i].Flag==0)
			{

				if(gDataConvert.globalFlags.bCheckInNetAttack)
					temp[no]=1;
			}
			if(myNetCardProp[i].Flag==1)
			{

				if(gDataConvert.globalFlags.bCheckOutNetAccack)
					temp[no]=3;
			}
			if(myNetCardProp[i].Flag==2)
			{

	//			if(gDataConvert.globalFlags.bCheckDmAccack)
	//				temp[no]=3;
			}
		}
	}
/*	temp[0] = 0;
	temp[1] = 0;
	temp[2] = 0;
	temp[3] = 3;
	temp[4] = 0;
	temp[5] = 3;
	temp[6] = 0;
	temp[7] = 0;
*/
	BOOL ok = DeviceIoControl(
            global_handle,
            0x8000,
            temp,
            8,
            NULL,
            0,
            &BytesReturned,
            NULL
            );
	if(ok)
		TRACE(_T("OKOKOKOKOKOKOKOKOK\n"));
}

void CFireWall1View::OnCfgnat() 
{
//	if(pnatcfg)
//	{
		if((DWORD)pnatcfg.StaticTable.item!=NULL)
		{
          delete []pnatcfg.StaticTable.item;
          pnatcfg.StaticTable.item=NULL;
		}
	    memset(&pnatcfg,0,sizeof(struct NAT_Cfg));
//	}

   	gDataConvert.SetNatCfg(&pnatcfg);
}

void CFireWall1View::OnStartfirewall() 
{
	// TODO: Add your command handler code here
/*   OnOpensys();
	gDataConvert.SetNatCfg(&pnatcfg);
	SetNetData();//以后写在配置命令发生器
	NatCfg();
	OnReadsys();
    Startfilter();//add
*/	
	SetNetData();//以后写在配置命令发生器
    m_bStartFirewall=TRUE;
}

void CFireWall1View::OnUpdateStartfirewall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(!m_bStartFirewall);
}

void CFireWall1View::OnUpdateStartfilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable((!m_bStartFilter)||(!m_bStartFirewall));	
}

void CFireWall1View::OnShutdownfirewall() 
{
	// TODO: Add your command handler code here
/*	clearipfilter();
    OnClosethread();
    OnClosesys();
//	ClearNatUseResoure();
//	if(pnatcfg)
//	{
      if((DWORD)pnatcfg.StaticTable.item!=NULL)
      delete []pnatcfg.StaticTable.item;
//      delete []pnatcfg;
//	}
    */
	m_bStartFirewall=FALSE;
	char temp[10];
	memset(temp,0,10);
	DWORD BytesReturned;
	BOOL ok = DeviceIoControl(
            global_handle,
            0x8000,
            temp,
            8,
            NULL,
            0,
            &BytesReturned,
            NULL
            );
	if(ok)
		TRACE(_T("OKOKOKOKOKOKOKOKOK\n"));

}

void CFireWall1View::OnUpdateShutdownfirewall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(m_bStartFirewall);
}

void CFireWall1View::OnToolLog() 
{
	logType=1;
}

void CFireWall1View::OnUpdateToolLog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CFireWall1View::OnProtlog() 
{
	// TODO: Add your command handler code here
	
}

void CFireWall1View::OnUpdateProtlog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CFireWall1View::OnUpdateOption(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bCfgOpen);
}

void CFireWall1View::OnFilterlog() 
{
	// TODO: Add your command handler code here

	
}

void CFireWall1View::OnUpdateFilterlog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}


void CFireWall1View::OnUpdateExit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CFireWall1View::OnUpdateClosefilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable((m_bStartFilter)&&(m_bStartFirewall));	
}

void CFireWall1View::Startfilter()
{
    int group=1;
    SetFilterInitReq(gDataConvert.m_firewallconfig);
	SetFilterCfg(gDataConvert.m_firewallconfig,gDataConvert.globalFlags);
	POSITION pos;
	pos=gDataConvert.m_ptrlistHostPort.GetHeadPosition();
//	struct HostList *phostlist;
	while(pos!=NULL)
	{
		struct HostList *phostlist=(struct HostList *)gDataConvert.m_ptrlistHostPort.GetNext(pos);
        SetFilterAddrTree(phostlist->sIpRange,group);
		for(int i=0;i<10;i++)
		{
	     SetFilterPortList(phostlist,i,group);
		}
		group++;
	}
 //   SetFilterAddrTree();
	SetFilterAcceptTab(gDataConvert.m_ptrlistAllowIp);
    SetFilterRejTab(gDataConvert.m_ptrlistRejectIp);
	SetDmIpTab(gDataConvert.m_listInHttp,T_INDM);
	SetDmIpTab(gDataConvert.m_listOutHttp,T_OUTDM);
    StartFilter();
}


void CFireWall1View::OnMenuitem32816() 
{
  

}

void CFireWall1View::OnUpdateAppExit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bPropsheetOpen);
}

void CFireWall1View::SetNetData()
{
	ULONG           BytesReturned;	
	BYTE  temp[40];
	memset(temp,0,40);
	int no;
	for(int i=0;i<8;i++)
	{
		no=myNetCardProp[i].DeviceNo;
		if(no)
		{
			if(no==(INNO-0x30))temp[no]=1;
			else temp[no]=3;
/*			if(myNetCardProp[i].Flag==0)
			{

				if(gDataConvert.globalFlags.bCheckInNetAttack)
					temp[no]=1;
			}
			if(myNetCardProp[i].Flag==1)
			{

				if(gDataConvert.globalFlags.bCheckOutNetAccack)
					temp[no]=3;
			}
			if(myNetCardProp[i].Flag==2)
			{

	//			if(gDataConvert.globalFlags.bCheckDmAccack)
	//				temp[no]=3;
			}
			*/
		}
	}
/*
	temp[0] = 0;
	temp[1] = 0;
	temp[2] = 0;
	temp[3] = 0;
	temp[4] = 3;
	temp[5] = 0;
	temp[6] = 0;
	temp[7] = 0;
*/
	BOOL ok = DeviceIoControl(
            global_handle,
            0x8000,
            temp,
            8,
            NULL,
            0,
            &BytesReturned,
            NULL
            );
	if(ok)
		TRACE(_T("OKOKOKOKOKOKOKOKOK\n"));
	else
	{
		int iiii=GetLastError();
	}
}

void CFireWall1View::OnStopLog() 
{
     logType=0;
}

LRESULT CFireWall1View:: OnLogView(WPARAM wParam,LPARAM lParam)
{
            ++number;	
	        if(number>MAXITEMNUMBER)
			{
		      flag = TRUE;
			}
	       lvItem.iItem = number;
	       if(flag)
		   {
		     m_listView1.DeleteItem(0);
		     lvItem.iItem = MAXITEMNUMBER;
		   }

	       lvItem.iSubItem = 0;
		   if(lParam==111)
		   {
		     struct LogView *logview=(struct LogView*)wParam;
			 lvItem.pszText = logview->time;
	         m_listView1.InsertItem(&lvItem);
	         m_listView1.SetItemText(lvItem.iItem,1,logview->prot);
	         m_listView1.SetItemText(lvItem.iItem,2,logview->src);
		     m_listView1.SetItemText(lvItem.iItem,3,logview->dst);
		     m_listView1.SetItemText(lvItem.iItem,4,logview->demo);
			 m_listView1.Update(lvItem.iItem);
		   }
		   if(lParam==112)
		   {
		     struct LogViewIcmp *logviewIcmp=(struct LogViewIcmp*)wParam;
			 lvItem.pszText = logviewIcmp->time;
	         m_listView1.InsertItem(&lvItem);
	         m_listView1.SetItemText(lvItem.iItem,1,logviewIcmp->prot);
	         m_listView1.SetItemText(lvItem.iItem,2,logviewIcmp->src);
		     m_listView1.SetItemText(lvItem.iItem,3,logviewIcmp->dst);
		     m_listView1.SetItemText(lvItem.iItem,4,logviewIcmp->demo);
		     m_listView1.SetItemText(lvItem.iItem,5,logviewIcmp->type);
		   }

	return 1L;
}

void CFireWall1View::CommandCon()
{
	OptMyConfig();//优化数据
	UserToKernel();
}

void CFireWall1View::OptMyConfig()
{
	return;
}

void CFireWall1View::UserToKernel()
{
	memcpy((void *)&TheServiceManageData,(void *)&MyServiceManageData,sizeof(ServiceManageData));
	memcpy((void *)&TheUserVerifyData,(void *)&MyUserVerifyData,sizeof(UserVerifyData));
	memcpy((void *)&TheLogManage,(void *)&MyLogManage,sizeof(LogManage));
	char cc;
	sscanf(NETMASK,"%d%c%d%c%d%c%d",&(mask.AddrByte[0]),&cc,
		&mask.AddrByte[1],&cc,
		&mask.AddrByte[2],&cc,
		&mask.AddrByte[3]);
	ProtoManInit();
}





WORD CFireWall1View::GetInitDataFromFile()
{
	memset((void *)&MyServiceManageData,0,sizeof(ServiceManageData));
	memset((void *)&MyUserVerifyData,0,sizeof(UserVerifyData));
	memset((void *)&MyLogManage,0,sizeof(LogManage));
	GetDataFromPage1();
	GetDataFromPage3();
	GetDataFromPage4();
	GetDataFromPage5();
	GetDataFromPage6();
	
	return 0;
}


WORD CFireWall1View::GetDataFromPage1()
{
	char command[40];
	memset(command,0,40);
	char *pbuf;
	pfoutDeviceInit=fopen("device.txt","r");
	if(pfoutDeviceInit==NULL)return 0;
	while(feof(pfoutDeviceInit)==0)
	{
		ReadCommand(pfoutDeviceInit,command);
		switch(GetCommand(command,1))
		{
		case INNET:
			ReadCommand(pfoutDeviceInit,command);
			strcpy(tmpin,command);
			INNO=tmpin[0];
//		TRACE(tmpin);
		
		
			break;
		case OUTNET:
			ReadCommand(pfoutDeviceInit,command);
			strcpy(tmpout,command);
			OUTNO=tmpout[0];
			break;
		case DM:
			ReadCommand(pfoutDeviceInit,command);
			strcpy(tmpdm,command);
			DMNO=tmpdm[0];
			break;
		case INNETMASK:
			ReadCommand(pfoutDeviceInit,command);
			strcpy(NETMASK,command);
			break;
		}
	}
	if(pfoutDeviceInit)int errrr=fclose(pfoutDeviceInit);
	return 1;
}

void CFireWall1View::ReadCommand(FILE *pf, char *pbuf)
{
	while(feof(pf)==0) {
		*pbuf=fgetc(pf);
		if(*pbuf=='\n') break;
		pbuf++;
	}
	*pbuf--=0;
	if(*pbuf=='\r')*pbuf=0;
}

DWORD CFireWall1View::GetCommand(char *pbuf, int i)
{
	char buf[50];
	
	if(*pbuf==0) return INITIAL_END_SEGMENT;
	sscanf(pbuf,"%s",buf);
	if(i==1)
	{
		if(strstr(buf,"[InNet]")!=NULL) return INNET;
		else if(strstr(buf,"[OutNet]")!=NULL) return OUTNET;
		else if(strstr(buf,"[DM]")!=NULL)return DM;
		else if(buf[0]=='#') return COMMENT;
		else if(strstr(buf,"[jl]")!=NULL) return INITIAL_END_SEGMENT;
		else return COMMAND_UNKNOWN;
	}
	if(i==4)
	{
		if(strstr(buf,"[NoFilterURL]")!=NULL) return NOFILTERURL;
		else if(strstr(buf,"[FilterAndBlock]")!=NULL) return FILTERANDBLOCK;
		else if(strstr(buf,"[TranProtocal]")!=NULL)return TRANPROTOCAL;
		else if(strstr(buf,"[Port]")!=NULL) return PORT;
		else if(strstr(buf,"[FilterKeyWords]")!=NULL) return FILTERKEYWORDS;
		else if(strstr(buf,"[TCPFilterBit]")!=NULL) return TCPFILTERBIT;
		else if(strstr(buf,"[BlockUrl]")!=NULL)return BLOCKURL;
		else if(buf[0]=='#') return COMMENT;
		else if(strstr(buf,"[jl]")!=NULL) return INITIAL_END_SEGMENT;
		else return COMMAND_UNKNOWN;
	}
	if(i==5)
	{
		if(strstr(buf,"[isUserVerify]")!=NULL) return ISUSERVERIFY;
		else if(strstr(buf,"[TIME]")!=NULL) return TIME;
		else if(strstr(buf,"[UserNameandPasswd]")!=NULL)return USERNAMEANDPASSWD;
		else if(strstr(buf,"[jl]")!=NULL) return INITIAL_END_SEGMENT;
		else return COMMAND_UNKNOWN;
	}
	if(i==6)
	{
		if(strstr(buf,"[RequireLog]")!=NULL) return REQUIRELOG;
		else if(strstr(buf,"[isLogIp]")!=NULL) return ISLOGIP;
		else if(strstr(buf,"[ipOptions]")!=NULL)return IPOPTIONS;
		else if(strstr(buf,"[isLogProtocal]")!=NULL)return ISLOGPROTOCAL;
		else if(strstr(buf,"[proOptions]")!=NULL)return PROOPTIONS;
		else if(strstr(buf,"[jl]")!=NULL) return INITIAL_END_SEGMENT;
		else return COMMAND_UNKNOWN;
	}
}



WORD CFireWall1View::GetDataFromPage4()
{
	int keywordcopy=0;
	int nofilterurlcopy=0;
	int blockurlcopy=0;
	int totlen=0;
	int len;
	char command[40];
	char *pbuf;
	pfoutServiceInit=fopen("service.txt","r");
	if(pfoutServiceInit==NULL)return 0;
	while(feof(pfoutServiceInit)==0)
	{
		ReadCommand(pfoutServiceInit,command);
		switch(GetCommand(command,4))
		{
		case FILTERANDBLOCK:
			ReadCommand(pfoutServiceInit,command);
			sscanf(command,"%d",&MyServiceManageData.FilterAndBlock);
			break;
		case TRANPROTOCAL:
			ReadCommand(pfoutServiceInit,command);
			sscanf(command,"%d",&MyServiceManageData.TranProtocal);
			break;
		case PORT:
			ReadCommand(pfoutServiceInit,command);
			sscanf(command,"%d",&MyServiceManageData.Port);
			break;
		case FILTERKEYWORDS:
			ReadCommand(pfoutServiceInit,command);
			while(GetCommand(command,4)!=INITIAL_END_SEGMENT) {
				if(keywordcopy++==0)
					strcpy(MyServiceManageData.FilterKeyWord,command);
				else strcat(MyServiceManageData.FilterKeyWord,command);
				strcat(MyServiceManageData.FilterKeyWord,"\r\n");
					ReadCommand(pfoutServiceInit,command);
			}
			break;
		case TCPFILTERBIT:
			ReadCommand(pfoutServiceInit,command);
			sscanf(command,"%d",&MyServiceManageData.TCPFilterBit);
			break;
		case BLOCKURL:
			ReadCommand(pfoutServiceInit,command);
			while(GetCommand(command,4)!=INITIAL_END_SEGMENT) {
				if(blockurlcopy++==0)
					strcpy(MyServiceManageData.BlockURLList,command);
				else strcat(MyServiceManageData.BlockURLList,command);
				strcat(MyServiceManageData.BlockURLList,"\r\n");
				ReadCommand(pfoutServiceInit,command);
			}
			//AfxMessageBox(MyServiceManageData.BlockURLList);
			break;
		case NOFILTERURL:
			ReadCommand(pfoutServiceInit,command);
			while(GetCommand(command,4)!=INITIAL_END_SEGMENT) {
				if(nofilterurlcopy++==0)
					strcpy(MyServiceManageData.NoFilterURLList,command);
				else strcat(MyServiceManageData.NoFilterURLList,command);
				strcat(MyServiceManageData.NoFilterURLList,"\r\n");
				ReadCommand(pfoutServiceInit,command);
			}
			break;	
		}
	}
	if(pfoutServiceInit)fclose(pfoutServiceInit);
	return 1;
}

WORD CFireWall1View::GetDataFromPage5()
{
	int iiii=0;
	char command[40];
	char *pbuf;
	pfoutUserverInit=fopen("userverify.txt","r");
	if(pfoutUserverInit==NULL)return 0;
	while(feof(pfoutUserverInit)==0)
	{
		ReadCommand(pfoutUserverInit,command);
		switch(GetCommand(command,5))
		{
		case ISUSERVERIFY:
			ReadCommand(pfoutUserverInit,command);
			sscanf(command,"%d",&MyUserVerifyData.IsVerify);
			break;
		case TIME:
			ReadCommand(pfoutUserverInit,command);
			sscanf(command,"%d",&MyUserVerifyData.timeRefesh);
			break;
		case USERNAMEANDPASSWD:
			ReadCommand(pfoutUserverInit,command);
			
			while(GetCommand(command,5)!=INITIAL_END_SEGMENT) {
				if(iiii++==0)strcpy(MyUserVerifyData.UserList,command);
				else strcat(MyUserVerifyData.UserList,command);
				strcat(MyUserVerifyData.UserList,"\r\n");
				ReadCommand(pfoutUserverInit,command);
			}
			break;	
		}
	}
	if(pfoutUserverInit)fclose(pfoutUserverInit);
	return 1;
}

WORD CFireWall1View::GetDataFromPage6()
{
	char command[40];
	char *pbuf;
	pfoutLogsetInit=fopen("logset.txt","r");
	if(pfoutLogsetInit==NULL)return 0;
	while(feof(pfoutLogsetInit)==0)
	{
		ReadCommand(pfoutLogsetInit,command);
		switch(GetCommand(command,6))
		{
		case REQUIRELOG:
			ReadCommand(pfoutLogsetInit,command);
			sscanf(command,"%d",&MyLogManage.RequireLog);
			break;
		case ISLOGIP:
			ReadCommand(pfoutLogsetInit,command);
			sscanf(command,"%d",&MyLogManage.TheIpFilter.isLogIp);
			break;
		case IPOPTIONS:
			ReadCommand(pfoutLogsetInit,command);
			sscanf(command,"%d",&MyLogManage.TheIpFilter.Options);
			break;
		case ISLOGPROTOCAL:
			ReadCommand(pfoutLogsetInit,command);
			sscanf(command,"%d",&MyLogManage.TheProCommentsLog.isLogProtocal);
			break;
		case PROOPTIONS:
			ReadCommand(pfoutLogsetInit,command);
			sscanf(command,"%d",&MyLogManage.TheProCommentsLog.Options);
			break;	
		}
	}
	if(pfoutLogsetInit)fclose(pfoutLogsetInit);
	return 1;
}

WORD CFireWall1View::GetDataFromPage3()
{
	char command[40];
	char *pbuf;
	pfoutDefaultInit=fopen("default.txt","r");
	if(pfoutDefaultInit==NULL)return 0;
	while(feof(pfoutDefaultInit)==0)
	{
		ReadCommand(pfoutDefaultInit,command);
		switch(GetCommand(command,3))
		{
		case PRIORITY:
			ReadCommand(pfoutDefaultInit,command);
			sscanf(command,"%d",&TheDefaultSetData.Priority);
			break;
		case CHECKINOROUTNETCARD:
			break;
		case PRO:
			break;
		case TCPPROTOCAL:
			break;
		case PORTOCALCOMMENTS:
			break;
		case UDPPROTOCAL:
			break;
		}
	}
	return 1;
}
