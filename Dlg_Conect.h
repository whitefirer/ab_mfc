#pragma once


// CDlg_Conect �Ի���

class CDlg_Conect : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Conect)
	friend UINT ServerThreadProc(LPVOID pParam);
public:
	CDlg_Conect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Conect();

// �Ի�������
	enum { IDD = IDD_CONECT_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
