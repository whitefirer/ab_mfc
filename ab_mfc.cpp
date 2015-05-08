// ab_mfc.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "MainFrm.h"

#include "ab_mfcDoc.h"
#include "ab_mfcView.h"
#include "afxcmn.h"

#include "Page1.h"
#include "Page2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cab_mfcApp

BEGIN_MESSAGE_MAP(Cab_mfcApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &Cab_mfcApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// Cab_mfcApp ����

Cab_mfcApp::Cab_mfcApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cab_mfcApp ����

Cab_mfcApp theApp;


// Cab_mfcApp ��ʼ��

BOOL Cab_mfcApp::InitInstance()
{
	CSplashWnd* pSplash = new CSplashWnd;  // �����������洰�� 
	pSplash->ShowWindow(m_nCmdShow);  // ��ʾ�������洰�� 
	Sleep(2500);  // ��������ʱ 2.5 �� 
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cab_mfcDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(Cab_mfcView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	int m_CurSelTab; 
	CPage1 m_page1; 
	CPage2 m_page2; 
	CDialog* pDialog[2];
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnBnClickedOk();
	virtual void OnOK();
public:
	afx_msg void OnIcon32777();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);//�������ô˱�����IDC_TAB1�ؼ����н���
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CAboutDlg::OnTcnSelchangeTab1)
	//ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_COMMAND(ID_ICON_32777, &CAboutDlg::OnIcon32777)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void Cab_mfcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Cab_mfcApp ��Ϣ�������

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_tab.InsertItem(0, _T("tab1")); 
	m_tab.InsertItem(1, _T("tab2")); 
	m_page1.Create(IDD_TAB_DLG1, &m_tab); 
	m_page2.Create(IDD_TAB_DLG2, &m_tab); 
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
void CAboutDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//�ѵ�ǰ��ҳ���������� 
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE); 
	//�õ��µ�ҳ������ 
	m_CurSelTab = m_tab.GetCurSel(); 
	//���µ�ҳ����ʾ���� 
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW); 
	*pResult = 0;
}

//void CAboutDlg::OnBnClickedOk()
//{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//return;
	//OnOK();
//}


void CAboutDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	//return ;
	CDialog::OnOK();
}

void CAboutDlg::OnIcon32777()
{
	CAboutDlg::OnClose();
	// TODO: �ڴ���������������
}
