#pragma once
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "afxcmn.h"
#include "Page3.h"
#include "Page4.h"

// CDlg2 �Ի���

class CDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CDlg2)

public:
	CDlg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg2();
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
