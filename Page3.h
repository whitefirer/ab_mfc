#pragma once


// Page3 �Ի���

class Page3 : public CDialog
{
	DECLARE_DYNAMIC(Page3)

public:
	Page3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Page3();
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_TAB_DLG3 };
	COScopeCtrl	m_oscopeCtrl1;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
afx_msg void OnTimer(UINT_PTR nIDEvent);
};
