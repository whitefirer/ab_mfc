// ab_mfcView.h : Cab_mfcView ��Ľӿ�
//


#pragma once


class Cab_mfcView : public CView
{
protected: // �������л�����
	Cab_mfcView();
	DECLARE_DYNCREATE(Cab_mfcView)
//OpenGL����
	GLdouble frenqu;
	GLdouble m_zTra;
	GLdouble m_yTra;
	GLdouble m_xTra;
	GLdouble m_zRo;
	GLdouble m_xRo;
	GLdouble m_yRo;
//	CDC* m_pDC;//�豸������
	void JIQIREN();
	void RenderScene();
	BOOL SetWindowPixelFormat(HDC m_hDC);
	HGLRC m_hRC;//����������
	HDC m_hDC;//�豸������
	int m_GLPixelIndex;
	float m_size;

// ����
public:
	Cab_mfcDoc* GetDocument() const;
	bool isConsole;
// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Cab_mfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // ab_mfcView.cpp �еĵ��԰汾
inline Cab_mfcDoc* Cab_mfcView::GetDocument() const
   { return reinterpret_cast<Cab_mfcDoc*>(m_pDocument); }
#endif

