#pragma once
#include "afxwin.h"
#include <tlhelp32.h>
#include <psapi.h>  
#pragma comment(lib,"psapi.lib")
#include <vector>
using namespace std;
// Page4 对话框

class Page4 : public CDialog
{
	DECLARE_DYNAMIC(Page4)

public:
	vector<PROCESSENTRY32> ProcList;
	Page4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Page4();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_TAB_DLG4 };
	void InitListCtrl(void);
	BOOL EnumProcessInfo(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
public:
	afx_msg void OnBnClickedRefresh();
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnDestroy();
};
