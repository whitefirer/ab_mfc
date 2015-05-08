// Page3.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Page3.h"


// Page3 对话框

IMPLEMENT_DYNAMIC(Page3, CDialog)

Page3::Page3(CWnd* pParent /*=NULL*/)
	: CDialog(Page3::IDD, pParent)
{

}

Page3::~Page3()
{
}

void Page3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Page3, CDialog)
		ON_WM_TIMER()
		ON_WM_DESTROY()
END_MESSAGE_MAP()


// Page3 消息处理程序
BOOL Page3::OnInitDialog()
{
	CDialog::OnInitDialog();
	IniteCPUInformationGet();
	SetTimer(1, 500, NULL);
	CRect rect1;
	GetClientRect(rect1);
	//ScreenToClient(rect1);
	rect1.left +=10;
	rect1.right -= 20;
	rect1.bottom -=10;
	rect1.bottom /= 2;
	rect1.top += 10;
    m_oscopeCtrl1.Create(WS_VISIBLE | WS_CHILD, rect1, this) ; 
	m_oscopeCtrl1.SetRange(0.0, 100.0, 1) ;
	
	m_oscopeCtrl1.SetPerx(20);
	m_oscopeCtrl1.SetPery(10);
	//m_oscopeCtrl1.SetMovex(1);
	//m_oscopeCtrl1.SetMovey(10);

	m_oscopeCtrl1.SetYUnits("CPU") ;
	m_oscopeCtrl1.SetXUnits("Time (Windows Timer: 500 ms)") ;
	m_oscopeCtrl1.SetBackgroundColor(RGB(0, 0, 0)) ;
	m_oscopeCtrl1.SetGridColor(RGB(0, 200, 100)) ;
	m_oscopeCtrl1.SetPlotColor(RGB(255, 100, 0)) ;

return true;
}

void Page3::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int CPUtime = GetCPUtime();

	m_oscopeCtrl1.AppendPoint(CPUtime);

	CDialog::OnTimer(nIDEvent);
}
void Page3::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(1);
}
