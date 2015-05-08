// Page4.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Page4.h"


// Page4 �Ի���

IMPLEMENT_DYNAMIC(Page4, CDialog)

Page4::Page4(CWnd* pParent /*=NULL*/)
	: CDialog(Page4::IDD, pParent)
{

}

Page4::~Page4()
{
}

void Page4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(Page4, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_REFRESH, &Page4::OnBnClickedRefresh)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Page4::OnLvnItemchangedList1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// Page4 ��Ϣ�������
BOOL Page4::OnInitDialog()
{
	CDialog::OnInitDialog();
	//����list_ctrl���
	DWORD dwStyle = ::GetWindowLong(m_list.m_hWnd, GWL_STYLE); 
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);
	//�����·��
	SetWindowLong(m_list.m_hWnd, GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);
	//������չ���
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES;
	ListView_SetExtendedListViewStyleEx(m_list.m_hWnd,styles,styles);
	ListView_SetBkColor(m_list, RGB(254,240,69));
	ListView_SetTextBkColor(m_list,RGB(254,240,69));

	LV_COLUMN lvcol;
	lvcol.mask = LVCF_TEXT; 

	lvcol.pszText = "ӳ������"; 
	ListView_InsertColumn(m_list, 0, &lvcol); 
	ListView_SetColumnWidth(m_list, 0, 100); 


	lvcol.mask =LVCF_TEXT; 
	lvcol.pszText = "PID"; 
	ListView_InsertColumn(m_list, 1, &lvcol); 
	ListView_SetColumnWidth(m_list, 1, 100); 
	ListView_SetExtendedListViewStyle(m_list,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//������ʾ��ʽ
	SetTimer(1,500,NULL);
	EnumProcessInfo();
	InitListCtrl();

	return true;
}
void Page4::OnBnClickedRefresh()
{
	UpdateData(TRUE);
	m_list.DeleteAllItems();
	InitListCtrl();
	UpdateData(FALSE);
}

void Page4::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void Page4::InitListCtrl(void)
{
	for( UINT i=0 ; i < ProcList.size() ; i++ )
	{
		m_list.InsertItem( i, ProcList[i].szExeFile );
		CString string;
		string.Format("%d", ProcList[i].th32ProcessID);
		m_list.SetItemText( i, 1,  string);

	}
}

BOOL Page4::EnumProcessInfo(void)
{
	//����ϵͳ��ǰ���̿���
	HANDLE hProcessShot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS , 0 ); 
	if( INVALID_HANDLE_VALUE == hProcessShot )
		return FALSE ;
	// ���������Ϣ�ṹ
	PROCESSENTRY32 pe32 = {sizeof(pe32)} ;

	// ѭ��ö�ٽ�����Ϣ
	if (Process32First( hProcessShot, &pe32 ) )
	{
		do {
			if( 0 == pe32.th32ProcessID )
				strcpy(pe32.szExeFile, "System Idle Process") ;
			else if( 4 == pe32.th32ProcessID )
				strcpy(pe32.szExeFile, "System") ;

			// ��ӵ�vector��
			ProcList.push_back(pe32);
		}while (Process32Next(hProcessShot,&pe32));
	}
	CloseHandle (hProcessShot) ;
	
	return TRUE ;
}

void Page4::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if( nIDEvent == 1 )
	{
		EnumProcessInfo();
	}
	CDialog::OnTimer(nIDEvent);
}

void Page4::OnDestroy()
{
	CDialog::OnDestroy();
	KillTimer(1);
	// TODO: �ڴ˴������Ϣ����������
}
