#pragma once
#include "afxcmn.h"


// CPage2 �Ի���

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage2();
//Ŀ¼��
	void BrowseFile(int CallNum,CString strFile);
	CString GetFileName();
	HICON m_hIcon;
	char FileBuffer[MAX_PATH];
	CImageList m_iImageList;//ͼ����
	HTREEITEM strRoot;
	HTREEITEM strHTFir;
	HTREEITEM strHTSec;
	HTREEITEM strHtThi;
	HTREEITEM strHtFor;
	HTREEITEM strHtFif;
	HTREEITEM strHtEnd;
	OPENFILENAME OpenFileName;
// �Ի�������
	enum { IDD = IDD_TAB_DLG2 };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
