// Dlg_Conect.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Dlg_Conect.h"

// CDlg_Conect 对话框

IMPLEMENT_DYNAMIC(CDlg_Conect, CDialog)

CDlg_Conect::CDlg_Conect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Conect::IDD, pParent)
{

}

CDlg_Conect::~CDlg_Conect()
{
}

void CDlg_Conect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Conect, CDialog)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

UINT ServerThreadProc(LPVOID pParam)
{
	char buf[800];

	memset(buf,0,800);
	//
	((CDlg_Conect*)pParam)->udp_server.Recvfrom(buf, 800);
	//((CDlg_Conect*)pParam)->udp_server.Sendto("发送成功", strlen("发送成功") + 1);
	//cout << buf;
	if(strlen(buf))
	{
		printf("%s\n", buf);
		//AfxMessageBox(buf);
		((CDlg_Conect*)pParam)->m_oscopeCtrl1.AppendPoint((int)buf[0]);	
	}

	if(((CDlg_Conect*)pParam)->threadflag)
	{
		return 1;
	}
	AfxBeginThread(ServerThreadProc, pParam, THREAD_PRIORITY_NORMAL);
	

return 0;
}

// CDlg_Conect 消息处理程序

int CDlg_Conect::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	threadflag = 0;
	CRect rect1;

	GetClientRect(rect1);

	rect1.top = 10;
	rect1.right /= 2;
	rect1.bottom /= 2;
	rect1.left = 20;

    m_oscopeCtrl1.Create(WS_VISIBLE | WS_CHILD, rect1, this) ; 
	m_oscopeCtrl1.SetRange(0.0, 100.0, 1) ;
	m_oscopeCtrl1.SetYUnits("InputRev") ;
	m_oscopeCtrl1.SetXUnits("Samples (Windows Timer: 500 ms)") ;
//	m_oscopeCtrl1.SetBackgroundColor(RGB(0, 0, 64)) ;
	m_oscopeCtrl1.SetGridColor(RGB(0, 255, 0)) ;
	m_oscopeCtrl1.SetPlotColor(RGB(255, 100, 0)) ;

	unsigned short in_port = 1989;
	udp_server.Socket();
	udp_server.SetPort(in_port);
	if(udp_server.Bind() == 0)
	{
		printf("bind error!\n");
			//exit(1);
	}

	// TODO:  在此添加您专用的创建代码
	AfxBeginThread(ServerThreadProc, this, THREAD_PRIORITY_NORMAL);

	return 0;
}

void CDlg_Conect::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	threadflag = 1;

	CDialog::OnClose();
}
