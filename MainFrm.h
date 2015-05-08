// MainFrm.h : CMainFrame 类的接口
//


#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:
	NOTIFYICONDATA m_tnid;//托盘图标
	bool flag;
// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam); 
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	COScopeCtrl	m_oscopeCtrl1;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
public:
	afx_msg void On32781();
public:
	afx_msg void On32782();
public:
	afx_msg void On32783();
public:
	afx_msg void OnIcon32777();
public:
	afx_msg void OnIcon32776();
public:
	afx_msg void OnIcon32775();
};


