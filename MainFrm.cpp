// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	flag = true;
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��������ͣ������ɾ��������
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

	//����ͼ��
	m_tnid.cbSize = sizeof(NOTIFYICONDATA); 
	m_tnid.hWnd = this->m_hWnd; 
	m_tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	m_tnid.uCallbackMessage = MYWM_NOTIFYICON;

	//�û�����Ļص���Ϣ 
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

	m_tnid.uFlags = NIF_INFO;						// �����־ʹ�ÿ�����ʾ������ʾ
	lstrcpy(m_tnid.szInfo, TEXT("Hello Kiki!"));
	m_tnid.dwInfoFlags = NIIF_INFO;
	lstrcpy(m_tnid.szInfoTitle, TEXT("ab_mfc"));

	Shell_NotifyIcon(NIM_MODIFY, &m_tnid);			// ��ʾ������ʾ

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

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


// CMainFrame ��Ϣ�������


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	 
	switch(message)
	{ 
		case MYWM_NOTIFYICON: 
		//������û��������Ϣ 
		if(lParam==WM_LBUTTONDBLCLK)
		{ 
		//���˫��ʱ�����ڳ��� 
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
		{ //����Ҽ���������ѡ�� 
			CMenu Menu;
			if (Menu.LoadMenu(IDR_MENU_ICON))        
			{
				CMenu* pMenu = Menu.GetSubMenu(0);
				ASSERT(pMenu != NULL);
				CPoint Point;
				GetCursorPos( &Point );
				SetForegroundWindow();		// ���¼�������ʲô�ô���
				pMenu->TrackPopupMenu(
					TPM_LEFTALIGN | TPM_RIGHTBUTTON,
					Point.x, Point.y, this); 
			}
		} 
		break; 
		case WM_SYSCOMMAND: 
		//�����ϵͳ��Ϣ 
		if(wParam == SC_MINIMIZE)
		{ 
			// ���յ���С����Ϣʱ���������� 
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			// $$$$$: kikiͯЬ������return 0;��
		} 
		break;	
	} 
	return CFrameWnd::WindowProc(message, wParam, lParam); 
} 

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
	::Shell_NotifyIcon(NIM_DELETE,&m_tnid);//�رմ���ʱɾ��ͼ��
}

void CMainFrame::On32781()
{
	// TODO: �ڴ���������������
	CDlg2 dlg;
	dlg.DoModal();         
}

void CMainFrame::On32782()
{
	// TODO: �ڴ���������������
	CDlg3 dlg;
	dlg.DoModal(); 
}

void CMainFrame::On32783()
{
	CDlg_Conect dlg;
	dlg.DoModal(); 
	// TODO: �ڴ���������������
}

void CMainFrame::OnIcon32777()
{
	//CMainFrame::OnClose();
	::Shell_NotifyIcon(NIM_DELETE,&m_tnid);//�رմ���ʱɾ��ͼ��
	exit(0);
	// TODO: �ڴ���������������
}

void CMainFrame::OnIcon32776()
{
	ShowWindow(SW_HIDE);
	// TODO: �ڴ���������������
}

void CMainFrame::OnIcon32775()
{
	ShowWindow(SW_SHOW);
	// TODO: �ڴ���������������
}
