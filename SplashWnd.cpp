// SplashWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "SplashWnd.h"
#include "Speci_Window.h"

// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{
	LPCTSTR lpszWndClass = AfxRegisterWndClass(NULL);  // 注册窗口类 
	CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT, lpszWndClass, NULL, 
		WS_POPUP,  
		CW_USEDEFAULT,  CW_USEDEFAULT,  SPLASH_WIDTH,  SPLASH_HEIGHT,  NULL, 
		NULL, NULL);  // 创建窗口 
	SetLayeredWindowAttributes(0, 0, 3);
}

CSplashWnd::~CSplashWnd()
{
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // 得到应用程序的模块句柄 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // 加载程序中的位图资源 
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HRGN hrgn = ::BitmapToRegion(this->GetSafeHwnd(), hbmp, -2, -1, -1);
	if (hrgn)
		SetWindowRgn(hrgn, TRUE);
	// TODO:  在此添加您专用的创建代码
	CenterWindow();  // 启动画面窗口居中
	ShowWindow(SW_SHOW);
	UpdateWindow();
	SetTimer(1, 3000, NULL);  // 启动画面显示时间为 3秒 
	AnimateWindow(1200,AW_BLEND);  // 窗口渐显，实现淡入效果，时间 1.2 秒 
	return 0;
}

void CSplashWnd::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	AnimateWindow(800,AW_BLEND|AW_HIDE);  // 窗口渐隐，实现淡出效果，时间 0.8 秒 
	CWnd::OnClose();
}

void CSplashWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);  // 销毁定时器 
	PostMessage(WM_CLOSE);  // 关闭启动画面 
	CWnd::OnTimer(nIDEvent);
}

BOOL CSplashWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // 得到应用程序的模块句柄 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // 加载程序中的位图资源 
	HDC hmdc = ::CreateCompatibleDC(pDC->GetSafeHdc());  // 创建兼容 DC作为内存 DC 
	HBITMAP hbmpOld = (HBITMAP) ::SelectObject(hmdc, hbmp);  //将位图选入内存 DC 
	::BitBlt(pDC->GetSafeHdc(),  0,  0,  SPLASH_WIDTH,  SPLASH_HEIGHT,  hmdc, 0, 0, SRCCOPY);  // 将内存 DC 中的内容拷贝到设备 DC以显示 
	// 清理，防止内存泄漏
	::DeleteObject(hbmp); 
	::DeleteDC(hmdc); 
	   
	return CWnd::OnEraseBkgnd(pDC);
}