#pragma once


// CDlg3 对话框

class CDlg3 : public CDialog
{
	DECLARE_DYNAMIC(CDlg3)
	friend UINT DrawIt(LPVOID pParam);
	friend UINT FilterImage(LPVOID pParam);
public:
	CDlg3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg3();
	void Bilinear(float);
	void Adjust(CWnd *pWnd, int cx, int cy);
	
void  UpdateLayered();
// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HBITMAP m_hmemBmp;
	BYTE* m_pBits;
	DWORD m_dwSize;
	DWORD m_dwEffWidth;
	BOOL  m_bEscape;
	CRect m_rect;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnPaint();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedButton5();
public:
	afx_msg void OnBnClickedButton6();
public:
	afx_msg void OnBnClickedButton7();
public:
	afx_msg void OnBnClickedButton8();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


