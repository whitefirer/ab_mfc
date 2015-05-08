// SplashWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "SplashWnd.h"
#include "Speci_Window.h"

// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{
	LPCTSTR lpszWndClass = AfxRegisterWndClass(NULL);  // ע�ᴰ���� 
	CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT, lpszWndClass, NULL, 
		WS_POPUP,  
		CW_USEDEFAULT,  CW_USEDEFAULT,  SPLASH_WIDTH,  SPLASH_HEIGHT,  NULL, 
		NULL, NULL);  // �������� 
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
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // �õ�Ӧ�ó����ģ���� 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // ���س����е�λͼ��Դ 
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HRGN hrgn = ::BitmapToRegion(this->GetSafeHwnd(), hbmp, -2, -1, -1);
	if (hrgn)
		SetWindowRgn(hrgn, TRUE);
	// TODO:  �ڴ������ר�õĴ�������
	CenterWindow();  // �������洰�ھ���
	ShowWindow(SW_SHOW);
	UpdateWindow();
	SetTimer(1, 3000, NULL);  // ����������ʾʱ��Ϊ 3�� 
	AnimateWindow(1200,AW_BLEND);  // ���ڽ��ԣ�ʵ�ֵ���Ч����ʱ�� 1.2 �� 
	return 0;
}

void CSplashWnd::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	AnimateWindow(800,AW_BLEND|AW_HIDE);  // ���ڽ�����ʵ�ֵ���Ч����ʱ�� 0.8 �� 
	CWnd::OnClose();
}

void CSplashWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);  // ���ٶ�ʱ�� 
	PostMessage(WM_CLOSE);  // �ر��������� 
	CWnd::OnTimer(nIDEvent);
}

BOOL CSplashWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HINSTANCE hApp = ::GetModuleHandle(NULL);  // �õ�Ӧ�ó����ģ���� 
	HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // ���س����е�λͼ��Դ 
	HDC hmdc = ::CreateCompatibleDC(pDC->GetSafeHdc());  // �������� DC��Ϊ�ڴ� DC 
	HBITMAP hbmpOld = (HBITMAP) ::SelectObject(hmdc, hbmp);  //��λͼѡ���ڴ� DC 
	::BitBlt(pDC->GetSafeHdc(),  0,  0,  SPLASH_WIDTH,  SPLASH_HEIGHT,  hmdc, 0, 0, SRCCOPY);  // ���ڴ� DC �е����ݿ������豸 DC����ʾ 
	// ������ֹ�ڴ�й©
	::DeleteObject(hbmp); 
	::DeleteDC(hmdc); 
	   
	return CWnd::OnEraseBkgnd(pDC);
}