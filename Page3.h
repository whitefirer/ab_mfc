#pragma once


// Page3 对话框

class Page3 : public CDialog
{
	DECLARE_DYNAMIC(Page3)

public:
	Page3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Page3();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_TAB_DLG3 };
	COScopeCtrl	m_oscopeCtrl1;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
afx_msg void OnTimer(UINT_PTR nIDEvent);
};
