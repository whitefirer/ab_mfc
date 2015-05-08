// ab_mfc.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// Cab_mfcApp 构造

Cab_mfcApp::Cab_mfcApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Cab_mfcApp 对象

Cab_mfcApp theApp;


// Cab_mfcApp 初始化

BOOL Cab_mfcApp::InitInstance()
{
	CSplashWnd* pSplash = new CSplashWnd;  // 创建启动画面窗口 
	pSplash->ShowWindow(m_nCmdShow);  // 显示启动画面窗口 
	Sleep(2500);  // 主窗口延时 2.5 秒 
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cab_mfcDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(Cab_mfcView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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
	DDX_Control(pDX, IDC_TAB1, m_tab);//用于设置此变量与IDC_TAB1控件进行交互
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CAboutDlg::OnTcnSelchangeTab1)
	//ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_COMMAND(ID_ICON_32777, &CAboutDlg::OnIcon32777)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void Cab_mfcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Cab_mfcApp 消息处理程序

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_tab.InsertItem(0, _T("tab1")); 
	m_tab.InsertItem(1, _T("tab2")); 
	m_page1.Create(IDD_TAB_DLG1, &m_tab); 
	m_page2.Create(IDD_TAB_DLG2, &m_tab); 
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
void CAboutDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//把当前的页面隐藏起来 
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE); 
	//得到新的页面索引 
	m_CurSelTab = m_tab.GetCurSel(); 
	//把新的页面显示出来 
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW); 
	*pResult = 0;
}

//void CAboutDlg::OnBnClickedOk()
//{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加命令处理程序代码
}
