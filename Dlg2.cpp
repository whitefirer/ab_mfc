// Dlg2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Dlg2.h"
#include "Page3.h"
#include "Page4.h"

// CDlg2 �Ի���

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


// CDlg2 ��Ϣ�������
BOOL CDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_tab.InsertItem(0, _T("����")); 
	m_tab.InsertItem(1, _T("����")); 
	m_page1.Create(IDD_TAB_DLG3, &m_tab); 
	m_page2.Create(IDD_TAB_DLG4, &m_tab); 
	//���û��洰�ڴ�С�ڿؼ���
	CRect rc; m_tab.GetClientRect(rc); 
	rc.top += 20; 
	rc.bottom -= 0; 
	rc.left += 0; 
	rc.right -= 0; 
	m_page1.MoveWindow(&rc); 
	m_page2.MoveWindow(&rc); 

	pDialog[0] = &m_page1; 
	pDialog[1] = &m_page2; 
	//��ʾ��ʼ����
	pDialog[0]->ShowWindow(SW_SHOW); 
	pDialog[1]->ShowWindow(SW_HIDE);

	//���浱ǰ�� ѡ��
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

	// TODO: �ڴ˴������Ϣ����������
	//KillTimer(1);
}

void CDlg2::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//�ѵ�ǰ��ҳ���������� 
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE); 
	//�õ��µ�ҳ������ 
	m_CurSelTab = m_tab.GetCurSel(); 
	//���µ�ҳ����ʾ���� 
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW); 
	*pResult = 0;
}
