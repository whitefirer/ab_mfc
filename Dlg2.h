#pragma once
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "afxcmn.h"
#include "Page3.h"
#include "Page4.h"

// CDlg2 对话框

class CDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CDlg2)

public:
	CDlg2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg2();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnDestroy();
	CTabCtrl m_tab;
	int m_CurSelTab; 
	Page3 m_page1; 
	Page4 m_page2; 
	CDialog* pDialog[2];
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
