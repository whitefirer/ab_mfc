// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "ab_mfc.h"

#include "MainFrm.h"
#include "Dlg3.h"
#include "Dlg_Conect.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_32781, &CMainFrame::On32781)
	ON_COMMAND(ID_32782, &CMainFrame::On32782)
	ON_COMMAND(ID_32783, &CMainFrame::On32783)
	ON_COMMAND(ID_ICON_32777, &CMainFrame::OnIcon32777)
	ON_COMMAND(ID_ICON_32776, &CMainFrame::OnIcon32776)
	ON_COMMAND(ID_ICON_32775, &CMainFrame::OnIcon32775)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	flag = true;
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CRect rect1;
	GetClientRect(rect1);
	rect1.top += 25;
	rect1.bottom -= 20;
	//ScreenToClient(rect1);
    //m_oscopeCtrl1.Create(WS_VISIBLE | WS_CHILD, rect1, this) ; 
	//m_oscopeCtrl1.SetRange(0.0, 3200.0, 1) ;
	//m_oscopeCtrl1.SetYUnits("InputRev") ;
	//m_oscopeCtrl1.SetXUnits("Samples (Windows Timer: 500 ms)") ;
	//m_oscopeCtrl1.SetBackgroundColor(RGB(0, 0, 64)) ;
	//m_oscopeCtrl1.SetGridColor(RGB(0, 255, 0)) ;
	//m_oscopeCtrl1.SetPlotColor(RGB(255, 100, 0)) ;

	//托盘图标
	m_tnid.cbSize = sizeof(NOTIFYICONDATA); 
	m_tnid.hWnd = this->m_hWnd; 
	m_tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	m_tnid.uCallbackMessage = MYWM_NOTIFYICON;

	//用户定义的回调消息 
	CString szToolTip; 
	szToolTip = _T("superkiki~"); 
	_tcscpy(m_tnid.szTip, szToolTip); 
	m_tnid.uID = IDR_MAINFRAME;

	HICON hIcon; 
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_UBUNTU); 
	m_tnid.hIcon = hIcon; 
	::Shell_NotifyIcon(NIM_ADD, &m_tnid); 
	if(hIcon)
		::DestroyIcon(hIcon);

	m_tnid.uFlags = NIF_INFO;						// 这个标志使得可以显示气球提示
	lstrcpy(m_tnid.szInfo, TEXT("Hello Kiki!"));
	m_tnid.dwInfoFlags = NIIF_INFO;
	lstrcpy(m_tnid.szInfoTitle, TEXT("ab_mfc"));

	Shell_NotifyIcon(NIM_MODIFY, &m_tnid);			// 显示气球提示

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	 
	switch(message)
	{ 
		case MYWM_NOTIFYICON: 
		//如果是用户定义的消息 
		if(lParam==WM_LBUTTONDBLCLK)
		{ 
		//鼠标双击时主窗口出现 
			if (flag == true)
			{
				flag = !flag;
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW); 
			}
			else
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
				flag = !flag;
			}
		} 
		else if(lParam==WM_RBUTTONDOWN)
		{ //鼠标右键单击弹出选单 
			CMenu Menu;
			if (Menu.LoadMenu(IDR_MENU_ICON))        
			{
				CMenu* pMenu = Menu.GetSubMenu(0);
				ASSERT(pMenu != NULL);
				CPoint Point;
				GetCursorPos( &Point );
				SetForegroundWindow();		// 想下激活下有什么好处？
				pMenu->TrackPopupMenu(
					TPM_LEFTALIGN | TPM_RIGHTBUTTON,
					Point.x, Point.y, this); 
			}
		} 
		break; 
		case WM_SYSCOMMAND: 
		//如果是系统消息 
		if(wParam == SC_MINIMIZE)
		{ 
			// 接收到最小化消息时主窗口隐藏 
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			// $$$$$: kiki童鞋在这里return 0;了
		} 
		break;	
	} 
	return CFrameWnd::WindowProc(message, wParam, lParam); 
} 

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
	::Shell_NotifyIcon(NIM_DELETE,&m_tnid);//关闭窗口时删除图标
}

void CMainFrame::On32781()
{
	// TODO: 在此添加命令处理程序代码
	CDlg2 dlg;
	dlg.DoModal();         
}

void CMainFrame::On32782()
{
	// TODO: 在此添加命令处理程序代码
	CDlg3 dlg;
	dlg.DoModal(); 
}

void CMainFrame::On32783()
{
	CDlg_Conect dlg;
	dlg.DoModal(); 
	// TODO: 在此添加命令处理程序代码
}

void CMainFrame::OnIcon32777()
{
	//CMainFrame::OnClose();
	::Shell_NotifyIcon(NIM_DELETE,&m_tnid);//关闭窗口时删除图标
	exit(0);
	// TODO: 在此添加命令处理程序代码
}

void CMainFrame::OnIcon32776()
{
	ShowWindow(SW_HIDE);
	// TODO: 在此添加命令处理程序代码
}

void CMainFrame::OnIcon32775()
{
	ShowWindow(SW_SHOW);
	// TODO: 在此添加命令处理程序代码
}
