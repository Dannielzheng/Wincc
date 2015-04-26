// WinccView.cpp : implementation of the CWinccView class
//

#include "stdafx.h"
#include "Wincc.h"

#include "WinccDoc.h"
#include "WinccView.h"
#include "QueryDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinccView

IMPLEMENT_DYNCREATE(CWinccView, CView)

BEGIN_MESSAGE_MAP(CWinccView, CView)
	//{{AFX_MSG_MAP(CWinccView)
	ON_COMMAND(IDM_QUERY, OnQuery)
	ON_COMMAND(IDM_SET, OnSet)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(IDM_COM1, OnCom1)
	ON_COMMAND(IDM_COM2, OnCom2)
	ON_COMMAND(IDM_COM3, OnCom3)
	ON_COMMAND(IDM_COM4, OnCom4)
	ON_COMMAND(IDM_COM5, OnCom5)
	ON_COMMAND(IDM_COM6, OnCom6)
	ON_COMMAND(IDM_COM7, OnCom7)
	ON_COMMAND(IDM_COM8, OnCom8)
	ON_COMMAND(IDM_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinccView construction/destruction

CWinccView::CWinccView()
{
	// TODO: add construction code here
	flag=0;
	handler=NULL;
	memset(buffer,0,100);
}

CWinccView::~CWinccView()
{
}

BOOL CWinccView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWinccView drawing

void CWinccView::OnDraw(CDC* pDC)
{
	CWinccDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWinccView diagnostics

#ifdef _DEBUG
void CWinccView::AssertValid() const
{
	CView::AssertValid();
}

void CWinccView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinccDoc* CWinccView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinccDoc)));
	return (CWinccDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinccView message handlers

void CWinccView::OnQuery() 
{
	// TODO: Add your command handler code here
	if(!flag)
	{
		query.Create(IDD_QUERY,this);
		set.DestroyWindow();
		query.ShowWindow(SW_SHOW);
		flag=1;
		SetTimer(1,1000,NULL);
	}
}

void CWinccView::OnSet() 
{
	// TODO: Add your command handler code here
	if(flag)
	{
		KillTimer(1);
		set.Create(IDD_SET,this);	
		query.DestroyWindow();
		set.ShowWindow(SW_SHOW);
		flag=0;
	}
}

int CWinccView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	query.c=0;
 	set.s=0;
	OnQuery();
	return 0;
}

void CWinccView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(handler!=NULL)
	{	
		memset(buffer,0,100);
		ReadData(handler,buffer);
		if(strlen(buffer)!=0)
			query.Show(buffer);
		memset(buffer,0,100);
		query.UpdateData();
	 	buffer[0]='*';
 		buffer[1]=query.c+1;//���ڵ�ѡĬ�ϴ�0��ʼ��ţ�0�Ļ��ᱻ��������λ����+1����
		WriteData(handler, buffer);

	}
	CView::OnTimer(nIDEvent);
}

void CWinccView::OnSend() 
{
	// TODO: Add your command handler code here
	memset(buffer,0,100);
	set.Get(buffer);
	WriteData(handler, buffer);
}




void CWinccView::OnCom1() 
{
	// TODO: Add your command handler code here
	handler=InitCom("COM1");
}

void CWinccView::OnCom2() 
{
	// TODO: Add your command handler code here
	handler=InitCom("COM2");
}

void CWinccView::OnCom3() 
{
	// TODO: Add your command handler code here
	handler=InitCom("COM3");
}

void CWinccView::OnCom4() 
{
	// TODO: Add your command handler code here
	handler=InitCom("COM4");
}

void CWinccView::OnCom5() 
{
	// TODO: Add your command handler code here
	handler=InitCom("COM5");
}

void CWinccView::OnCom6() 
{
	// TODO: Add your command handler code here
	handler=InitCom("COM6");
}

void CWinccView::OnCom7() 
{
	// TODO: Add your command handler code here
	handler=InitCom("COM7");
}

void CWinccView::OnCom8() 
{
	// TODO: Add your command handler code here
	handler=InitCom("COM8");
}
HANDLE CWinccView::InitCom(char* comName)  
{  
    HANDLE hCom;  
    hCom = CreateFile(comName,//COM7��  
        GENERIC_READ|GENERIC_WRITE, //�������д  
        0, //��ռ��ʽ  
        NULL,  
        OPEN_EXISTING, //�򿪶����Ǵ���  
        0, //ͬ����ʽ  
        NULL);  
    if(hCom == (HANDLE)-1)  
    {  
        return NULL;  
    }  
    SetupComm(hCom,100,100); //���뻺����������������Ĵ�С����100  
    COMMTIMEOUTS TimeOuts;  
    //�趨����ʱ  
    TimeOuts.ReadIntervalTimeout=MAXDWORD;  
    TimeOuts.ReadTotalTimeoutMultiplier=0;  
    TimeOuts.ReadTotalTimeoutConstant=0;  
    //�ڶ�һ�����뻺���������ݺ���������������أ�  
    //�������Ƿ������Ҫ����ַ���  
    //�趨д��ʱ  
    TimeOuts.WriteTotalTimeoutMultiplier=100;  
    TimeOuts.WriteTotalTimeoutConstant=500;  
    SetCommTimeouts(hCom,&TimeOuts); //���ó�ʱ  
  
    DCB dcb;  
    GetCommState(hCom, &dcb);  
    dcb.BaudRate=115200; //������Ϊ9600  
    dcb.ByteSize=8; //ÿ���ֽ���8λ  
    dcb.Parity=NOPARITY; //����żУ��λ  
    dcb.StopBits=1; //����ֹͣλ  
    SetCommState(hCom, &dcb);  
    PurgeComm(hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);  
    return hCom;  
} 


int CWinccView::ReadData(HANDLE handler, char* buffer)  
{  
    char readBuffer[100];  
    memset(readBuffer, 0, 100);  
    DWORD wCount= 100;//��ȡ���ֽ���  
    BOOL bReadStat;  
  
    bReadStat = ReadFile(handler, readBuffer, wCount, &wCount, NULL);  
    if(!bReadStat)  
    {  
        AfxMessageBox("������ʧ��!");  
        return -1;  
    }  
    strcpy(buffer, readBuffer);  
    PurgeComm(handler, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);    
    return 0;  
}


int CWinccView::WriteData(HANDLE handler, char* buffer)  
{  
    unsigned long dwBytesWrite;  
    COMSTAT ComStat;  
    DWORD dwErrorFlags;  
    BOOL bWriteStat;  
    ClearCommError(handler, &dwErrorFlags, &ComStat); 
	dwBytesWrite = strlen(buffer);  
    bWriteStat=WriteFile(handler, buffer, dwBytesWrite, &dwBytesWrite, NULL);  
    if(!bWriteStat)  
    {  
		AfxMessageBox("д����ʧ��!");  
		return -1;  
    }  
    return 0;  
}  


	


