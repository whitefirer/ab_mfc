// Dlg2.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Dlg2.h"
#include "Page3.h"
#include "Page4.h"

// CDlg2 对话框

IMPLEMENT_DYNAMIC(CDlg2, CDialog)

CDlg2::CDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg2::IDD, pParent)
{

}

CDlg2::~CDlg2()
{
}

void CDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(CDlg2, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlg2::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CDlg2 消息处理程序
BOOL CDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_tab.InsertItem(0, _T("性能")); 
	m_tab.InsertItem(1, _T("进程")); 
	m_page1.Create(IDD_TAB_DLG3, &m_tab); 
	m_page2.Create(IDD_TAB_DLG4, &m_tab); 
	//设置画面窗口大小在控件内
	CRect rc; m_tab.GetClientRect(rc); 
	rc.top += 20; 
	rc.bottom -= 0; 
	rc.left += 0; 
	rc.right -= 0; 
	m_page1.MoveWindow(&rc); 
	m_page2.MoveWindow(&rc); 

	pDialog[0] = &m_page1; 
	pDialog[1] = &m_page2; 
	//显示初始画面
	pDialog[0]->ShowWindow(SW_SHOW); 
	pDialog[1]->ShowWindow(SW_HIDE);

	//保存当前点 选项
	m_CurSelTab = 0; 

return true;
}
int CDlg2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CDlg2::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//KillTimer(1);
}

void CDlg2::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//把当前的页面隐藏起来 
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE); 
	//得到新的页面索引 
	m_CurSelTab = m_tab.GetCurSel(); 
	//把新的页面显示出来 
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW); 
	*pResult = 0;
}
