// OScopeCtrl.h : header file
//

#ifndef __OScopeCtrl_H__
#define __OScopeCtrl_H__

/////////////////////////////////////////////////////////////////////////////
// COScopeCtrl window

class COScopeCtrl : public CWnd
{
public:
	COScopeCtrl();

public:
	double AppendPoint(double dNewPoint);
	void SetRange(double dLower, double dUpper, int nDecimalPlaces=1);
	void SetXUnits(CString string);
	void SetYUnits(CString string);
	void SetGridColor(COLORREF color);
	void SetPlotColor(COLORREF color);
	void SetBackgroundColor(COLORREF color);
	void SetPerx(UINT x);
	void SetPery(UINT y);
	void SetMovex(UINT x);
	void SetMovey(UINT y);
	void InvalidateCtrl();
	void DrawPoint();
	void Reset();

public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COScopeCtrl)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL);
	//}}AFX_VIRTUAL

public:
	int m_nShiftPixels;          // ������
	int m_nYDecimals;

	CString m_strXUnitsString;
	CString m_strYUnitsString;

	COLORREF m_crBackColor;        // ����ɫ
	COLORREF m_crGridColor;        // ���ɫ
	COLORREF m_crPlotColor;        // ����ɫ  
  
	double m_dCurrentPosition;   // ��ǰλ��
	double m_dPreviousPosition;  // ��һλ��
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	virtual ~COScopeCtrl();

protected:
	//{{AFX_MSG(COScopeCtrl)
	afx_msg void OnPaint();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_nHalfShiftPixels;
	int m_nPlotShiftPixels;
	int m_nClientHeight;
	int m_nClientWidth;
	int m_nPlotHeight;
	int m_nPlotWidth;
	int m_movex;
	int m_movey;
	UINT m_perx;
	UINT m_pery;

	double m_dLowerLimit;       
	double m_dUpperLimit;        
	double m_dRange;
	double m_dVerticalFactor;
  
	CRect  m_rectClient;
	CRect  m_rectPlot;
	CPen   m_penPlot;
	CBrush m_brushBack;

	CDC     m_dcGrid;
	CDC     m_dcPlot;
	CBitmap *m_pbitmapOldGrid;
	CBitmap *m_pbitmapOldPlot;
	CBitmap m_bitmapGrid;
	CBitmap m_bitmapPlot;
};

/////////////////////////////////////////////////////////////////////////////
#endif
