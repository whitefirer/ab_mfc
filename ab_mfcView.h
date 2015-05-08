// ab_mfcView.h : Cab_mfcView 类的接口
//


#pragma once


class Cab_mfcView : public CView
{
protected: // 仅从序列化创建
	Cab_mfcView();
	DECLARE_DYNCREATE(Cab_mfcView)
//OpenGL定义
	GLdouble frenqu;
	GLdouble m_zTra;
	GLdouble m_yTra;
	GLdouble m_xTra;
	GLdouble m_zRo;
	GLdouble m_xRo;
	GLdouble m_yRo;
//	CDC* m_pDC;//设备描述表
	void JIQIREN();
	void RenderScene();
	BOOL SetWindowPixelFormat(HDC m_hDC);
	HGLRC m_hRC;//绘制描述表
	HDC m_hDC;//设备描述表
	int m_GLPixelIndex;
	float m_size;

// 属性
public:
	Cab_mfcDoc* GetDocument() const;
	bool isConsole;
// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Cab_mfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConsole();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // ab_mfcView.cpp 中的调试版本
inline Cab_mfcDoc* Cab_mfcView::GetDocument() const
   { return reinterpret_cast<Cab_mfcDoc*>(m_pDocument); }
#endif

