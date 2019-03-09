
#include "stdafx.h"
#include "FireWall1.h"
#include "outnat.h"
#include "authentication.h"
/////////////////////////////////////
/*
本程序从网卡1读入栈（TCP/IP栈）数据，
放入tempdata[480][1600]。
并把tempdata COPY到公共缓冲区global[68]的Buffer。
global_var global[68]见"testddk.h"
用到两种类型变量控制县城同步，
其中global_mutex是一个mutex，
用于控制global[]的下标global_cur。
global[].hMutex是一组Event，
用于控制缓冲区。
*/
////////////////////////////////////////
extern BOOL CloseAll;
global_var global[80];//全局管理数据
int global_cur;
extern int Resno;//恢复县城
HANDLE global_mutex;//控制global_cur
extern HANDLE mythread_write;
BOOL OPER;
BOOL write;   
BYTE tempdata[480][1600];//网卡1入栈数据包
//extern BYTE  SpecialBuffer[1500];
BOOL  ACK = 0;
static OVERLAPPED   OverLappedread_in,*lpOverLappedread;
static int defer;
extern HANDLE global_handle,global_handle1;
OVERLAPPED myoverlap;
static int off;
static int count40;
static int count = 0;

static int mydata;
extern HANDLE mythread_in;
static int aaa=0;
static int bbb=0;
VOID WINAPI ReadCompletionRoutine(DWORD dwErrorCode, 
   DWORD dwNumberOfBytesTransferred, 
   LPOVERLAPPED pOverlapped);
void initdata_in();
void ReadInBuffer();
static int inittime_in=0;
VOID ReadInThread(VOID)
{
	if(inittime_in++==0)	initdata_in();
	while(!CloseAll)//洗垢函数通知
	{
		if((off>=235)&&(mydata<190))
		{
	//		continue;
		}
		if(count40==60)//45*1518>65535,这样才能大数据包有效
		{
 			aaa++;
			SleepEx(1,TRUE);
			continue;
		}
		else count40++;
		if(off>79)off=0;
		ReadInBuffer();
	}

	//   TRACE(_T("INTHREAD FINISH\n"));

}


VOID WINAPI ReadCompletionRoutine(DWORD dwErrorCode, 
   DWORD dwNumberOfBytesTransferred, 
   LPOVERLAPPED pOverlapped) {
	if(OPER)
	{
		if(mydata>79)
		{
			bbb++;
			mydata=0;
		}
		
		DWORD dw=WaitForSingleObject(global_mutex,INFINITE);
		if(dw==WAIT_OBJECT_0)
		{
			
			BOOL fdone=FALSE;

			{
				{
				
					memcpy(global[global_cur].Buffer,
						tempdata[mydata],
						1518);
				
					global[global_cur].overlap=*pOverlapped;
					SetEvent(global[global_cur].hMutex);
			
					global_cur++;
					if(global_cur>79)global_cur=0;

					ReleaseMutex(global_mutex);
					fdone=TRUE;
					mydata++;
					count40--;
				//	count40=0;
				}
				
			}
		
			if(dwErrorCode!= 0)		
			{
				//   TRACE(_T("I/O ERR IN READCOMP\n"));
				return;
			}
		}
	}

		
}


void ReadInBuffer()
{
	
	BOOL Result;
	memset(&OverLappedread_in,0,sizeof(OVERLAPPED));
	lpOverLappedread=&OverLappedread_in;
	Result=ReadFileEx(
		global_handle,
		tempdata[off],
		1520,
		lpOverLappedread,
		ReadCompletionRoutine);
	if (Result==0)
	{
	
		int ii=GetLastError();
		OPER=FALSE;
	//	if((ii==1450)||(ii==998))
			TRACE(_T("不通\n"));

		
	}
	else
	{
		
		off++;
		OPER=TRUE;
	}
}
void initdata_in()
{

	mydata=0;
	defer=0;
	count40=0;
	off=0;         
	global_cur=0;
	global_mutex=CreateMutex(NULL,0,NULL);
	for(int aa=0;aa<480;aa++)
	{   
		memset(tempdata[aa],0,1600);
	}
	for(int i=0;i<80;i++)
	{
		memset(&global[i],0,sizeof(global_var));
		global[i].hMutex =CreateEvent(0,0,0,0);
	}
	
	memset(&myoverlap,0,sizeof(myoverlap));
	OPER=FALSE;
}