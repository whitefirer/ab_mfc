#pragma once
#include "afxcmn.h"


// CPage2 对话框

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage2();
//目录树
	void BrowseFile(int CallNum,CString strFile);
	CString GetFileName();
	HICON m_hIcon;
	char FileBuffer[MAX_PATH];
	CImageList m_iImageList;//图标链
	HTREEITEM strRoot;
	HTREEITEM strHTFir;
	HTREEITEM strHTSec;
	HTREEITEM strHtThi;
	HTREEITEM strHtFor;
	HTREEITEM strHtFif;
	HTREEITEM strHtEnd;
	OPENFILENAME OpenFileName;
// 对话框数据
	enum { IDD = IDD_TAB_DLG2 };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_FileTree;
	afx_msg void OnBnClickedButtonFilesearch();
	afx_msg void OnPaint();
	afx_msg void OnOK();
	afx_msg void OnDropFiles(HDROP hDropInfo);
public:
	afx_msg void OnBnClickedButton1();
};
