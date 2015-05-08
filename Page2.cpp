// Page2.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Page2.h"


// CPage2 对话框

IMPLEMENT_DYNAMIC(CPage2, CDialog)

CPage2::CPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CPage2::IDD, pParent)
{
	strcpy(FileBuffer, "成绩表");
}

CPage2::~CPage2()
{
	
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_FileTree);
}


BEGIN_MESSAGE_MAP(CPage2, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILESEARCH, &CPage2::OnBnClickedButtonFilesearch)
	ON_WM_PAINT()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPage2::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPage1 消息处理程序

void CPage2::OnBnClickedButtonFilesearch()
{
	CString m_FileDir;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.ulFlags   = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	BOOL bRet = FALSE;
	TCHAR szFolder[MAX_PATH * 2];
	strcpy(szFolder, "成绩表");


	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))  
			bRet = TRUE;
		IMalloc *pMalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) &&pMalloc)
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		}
	}

	m_FileDir = szFolder;
	BrowseFile(0, m_FileDir);//遍历"成绩表"文件夹内的所有目录
}

BOOL CPage2::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_iImageList.Create(24, 24, TRUE, 1, 0);
	m_FileTree.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT |  TVS_HASLINES | TVS_INFOTIP);  // 带加号、虚线

	HICON hIcon = NULL;
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
	MAKEINTRESOURCE(IDI_ICON_UBUNTU), IMAGE_ICON, 24, 24, 0);
	m_iImageList.Add(hIcon);
	m_FileTree.SetImageList ( &m_iImageList,TVSIL_NORMAL );

	//HINSTANCE hApp = ::GetModuleHandle(NULL);  // 得到应用程序的模块句柄 
	//HBITMAP hbmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));  // 加载程序中的位图资源 

	//HRGN hrgn = ::BitmapToRegion(GetDlgItem(IDC_PIC)->m_hWnd, hbmp, -2, -1, -1);
	//if (hrgn)
		//SetWindowRgn(hrgn, TRUE);

	return TRUE;
}


CString CPage2::GetFileName()
{		
	OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	OpenFileName.lStructSize = sizeof(OpenFileName);
	OpenFileName.hwndOwner = this->GetSafeHwnd();
	OpenFileName.hInstance = ::GetModuleHandle(NULL);
	OpenFileName.lpstrFilter = "所有文件\0*.*";//扩展名
	OpenFileName.lpstrCustomFilter = NULL;//扩展名
	OpenFileName.nMaxFile = MAX_PATH;//文件名缓冲长度
	OpenFileName.lpstrInitialDir = FileBuffer;
	OpenFileName.lpstrFile = FileBuffer;
	OpenFileName.lpstrFileTitle = NULL;//扩展名
	OpenFileName.lpTemplateName = "NULL";//扩展名
	OpenFileName.lpstrTitle = "Open files...";
	OpenFileName.lpstrDefExt="*";

	GetOpenFileName(&OpenFileName);

return OpenFileName.lpstrFile;	
}


void CPage2::BrowseFile(int CallNum, CString strFile)
{
	CallNum++;
	CFileFind ff;
	CString szDir = strFile; 

	if(szDir.Right(1) != "\\")
		szDir += "\\";

	szDir += "*.*";

	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())
		{	 
			//如果是一个子目录，用递归继续往深一层找
			CString strPath = ff.GetFilePath();
			CString strTitle = ff.GetFileTitle();
			int i =0;
			switch(CallNum)
			{
			case 1:	  
				strHTFir = m_FileTree.InsertItem(strTitle,0,0,NULL);						
				break;
			case 2:
				strHTSec = m_FileTree.InsertItem(strTitle,0,0,strHTFir);											
				break;
			case 3:
				strHtThi = m_FileTree.InsertItem(strTitle,0,0,strHTSec);					
				break;
			case 4:
				strHtFor = m_FileTree.InsertItem(strTitle,0,0,strHtThi);					
				break;
			default:
				strHtFif = m_FileTree.InsertItem(strTitle,0,0,strHtFor);
				break;					
			}
			BrowseFile(CallNum,strPath);
		}
		else if(!ff.IsDirectory() && !ff.IsDots())
		{
			//显示当前访问的文件
			CString strPath;
			CString strTitle;
			strPath = ff.GetFilePath();
			strTitle = ff.GetFileTitle();
			switch(CallNum)
			{
			case 1:
				strRoot = m_FileTree.InsertItem(strTitle,0,0,NULL);
				break;
			case 2:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHTFir);
				break;
			case 3:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHTSec);				
				break;
			case 4:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHtThi);
				break;
			case 5:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHtFor);
				break;
			default:
				strHtEnd = m_FileTree.InsertItem(strTitle,0,0,strHtFif);
				break;
			}
		}
	}
	ff.Close();//关闭
}
void CPage2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


void CPage2::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	TCHAR strFile[MAX_PATH] ;
	UINT cFiles = ::DragQueryFile(hDropInfo, 0xffffffff, 0, 0);     // 取得文件数

	for (UINT count = 0; count < cFiles; count++)
	{
		if (cFiles > 0)
        {		
            ::DragQueryFile(hDropInfo, count, strFile, MAX_PATH);	// 得到拖放的文件名
			AfxMessageBox(strFile);									// 提示拖放的文件名
			BrowseFile(0, strFile);									// 浏览文件目录
        }
	}
	
	CDialog::OnDropFiles(hDropInfo);
}

void CPage2::OnOK()
{
	return;
}

void CPage2::OnBnClickedButton1()
{
	CString strpath, strfilename;

	// TODO: 在此添加控件通知处理程序代码
	strpath = "路径:";
	strfilename = "文件名";
	//strpath += GetFileName();			// 另外一种直接打开找文件窗口的方式
	CFileDialog dlg(TRUE, NULL, NULL, NULL, "全部文件|*.*|jpg|*.jpg|bmp|*.bmp", NULL);
	{
		if(dlg.DoModal() == IDOK)
		{  
			strpath += dlg.GetPathName();
			strfilename += dlg.GetFileName();
		}
	}
	strpath += "\r\n";					// 注意这里用到的换行符
	strpath += strfilename;
	SetDlgItemText(IDC_PATH1, strpath);
}
