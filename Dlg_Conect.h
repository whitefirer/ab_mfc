#pragma once


// CDlg_Conect 对话框

class CDlg_Conect : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Conect)
	friend UINT ServerThreadProc(LPVOID pParam);
public:
	CDlg_Conect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Conect();

// 对话框数据
	enum { IDD = IDD_CONECT_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	COScopeCtrl	m_oscopeCtrl1;
	UDPserver udp_server;
	CWinThread* thisthred;
	BOOL threadflag;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnClose();
};
