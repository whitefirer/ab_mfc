// OScopeCtrl.cpp : implementation file//

#include "stdafx.h"
#include "math.h"

#include "OScopeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

/////////////////////////////////////////////////////////////////////////////
// COScopeCtrl
COScopeCtrl::COScopeCtrl()
{
	// ��ʼλ
	m_dPreviousPosition =   0.0 ;

	// y��������
	m_nYDecimals = 4 ;
	m_perx = 6;
	m_pery = 6;
	m_movex = 0;
	m_movey = 0;

	// ͼ��ı߽�����
	m_dLowerLimit = -10.0 ;
	m_dUpperLimit =  10.0 ;
	m_dRange      =  m_dUpperLimit - m_dLowerLimit ;   

	m_nShiftPixels     = 4 ;
	m_nHalfShiftPixels = m_nShiftPixels / 2 ;                     
	m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels ;  

	m_crBackColor  = RGB(  0,   0,   0) ;  // ��SetBackgroundColor���õı�����ɫ������ʱΪ��ɫ
	m_crGridColor  = RGB(  0, 255, 255) ;  // ��SetGridColor���õı����ɫ������ʱΪ��ɫ
	m_crPlotColor  = RGB(255, 255, 255) ;  // ��SetPlotColor���õ�������ɫ������ʱΪ��ɫ

	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor) ;
	m_brushBack.CreateSolidBrush(m_crBackColor) ;

	m_strXUnitsString.Format("Time") ;		// x���ϵ�����
	m_strYUnitsString.Format("Y units") ;		// y���ϵ�����

	// �տ�ʼʱ��bitmap���ڴ��ַȡNULL~
	m_pbitmapOldGrid = NULL ;
	m_pbitmapOldPlot = NULL ;

}  // COScopeCtrl

/////////////////////////////////////////////////////////////////////////////
COScopeCtrl::~COScopeCtrl()
{
	// д�ؼ�����Ҫ�ľ���Ҫע������ʱ�ͷ��ڴ���~
	if (m_pbitmapOldGrid != NULL)
		m_dcGrid.SelectObject(m_pbitmapOldGrid) ;  
	if (m_pbitmapOldPlot != NULL)
		m_dcPlot.SelectObject(m_pbitmapOldPlot) ;  

} // ~COScopeCtrl


BEGIN_MESSAGE_MAP(COScopeCtrl, CWnd)
  //{{AFX_MSG_MAP(COScopeCtrl)
  ON_WM_PAINT()
  ON_WM_SIZE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COScopeCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
BOOL COScopeCtrl::Create(DWORD dwStyle, const RECT& rect, 
                         CWnd* pParentWnd, UINT nID) 
{
	BOOL result ;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;// ע����

	  result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 
							className, NULL, dwStyle, 
							rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
							pParentWnd->GetSafeHwnd(), (HMENU)nID) ;		// ����
	if (result != 0)
		InvalidateCtrl() ;

	return result ;

} // Create

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetRange(double dLower, double dUpper, int nDecimalPlaces)
{
	ASSERT(dUpper > dLower) ;

	m_dLowerLimit     = dLower ;
	m_dUpperLimit     = dUpper ;
	m_nYDecimals      = nDecimalPlaces ;
	m_dRange          = m_dUpperLimit - m_dLowerLimit ;
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 
  
 // �ػ�ؼ�
  InvalidateCtrl() ;

}  // SetRange


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetXUnits(CString string)
{
  m_strXUnitsString = string ;

  // �ػ�ؼ�
  InvalidateCtrl() ;

}  // SetXUnits

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetYUnits(CString string)
{
	m_strYUnitsString = string ;

	// �ػ�ؼ�
	InvalidateCtrl() ;

}  // SetYUnits

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetGridColor(COLORREF color)
{
	m_crGridColor = color ;

	// �ػ�ؼ�
	InvalidateCtrl() ;

}  // SetGridColor


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetPlotColor(COLORREF color)
{
	m_crPlotColor = color ;

	m_penPlot.DeleteObject() ;
	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor) ;

	// �ػ�ؼ�
	InvalidateCtrl() ;

}  // SetPlotColor


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetBackgroundColor(COLORREF color)
{
  m_crBackColor = color ;

  m_brushBack.DeleteObject() ;
  m_brushBack.CreateSolidBrush(m_crBackColor) ;

   // �ػ�ؼ�
  InvalidateCtrl() ;

}  // SetBackgroundColor

void COScopeCtrl::SetPerx(UINT x)
{
	m_perx = x ;

	// �ػ�ؼ�
	InvalidateCtrl() ;

}  // SetPerx

void COScopeCtrl::SetPery(UINT y)
{
	m_pery = y ;

	// �ػ�ؼ�
	InvalidateCtrl() ;

}  // SetPery

void COScopeCtrl::SetMovex(UINT x)
{
	m_movey = x ;

	// �ػ�ؼ�
	InvalidateCtrl() ;

}  // SetPery

void COScopeCtrl::SetMovey(UINT y)
{
	m_movey = y ;

	// �ػ�ؼ�
	InvalidateCtrl() ;

}  // SetPery
/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::InvalidateCtrl()
{
	int i ;
	int nCharacters ;
	int nTopGridPix, nMidGridPix, nBottomGridPix ;
	int nTopGridPiy, nMidGridPiy, nBottomGridPiy ;

	CPen *oldPen ;
	CPen solidPen(PS_SOLID, 0, m_crGridColor) ;
	CFont axisFont, yUnitFont, *oldFont ;
	CString strTemp ;

	CClientDC dc(this) ;  

	// ���Ϊ�գ���Ϊ����������
	if (m_dcGrid.GetSafeHdc() == NULL)
	{
		m_dcGrid.CreateCompatibleDC(&dc) ;
		m_bitmapGrid.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
		m_pbitmapOldGrid = m_dcGrid.SelectObject(&m_bitmapGrid) ;
	}
  
	m_dcGrid.SetBkColor (m_crBackColor) ;

	// ���������
	m_dcGrid.FillRect(m_rectClient, &m_brushBack) ;

	nCharacters = abs((int)log10(fabs(m_dUpperLimit))) ;
	nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit)))) ;

	nCharacters = nCharacters + 4 + m_nYDecimals ;  

	m_rectPlot.left = m_rectClient.left + 6*(nCharacters) ;
	m_nPlotWidth    = m_rectPlot.Width() ;

	// ������
	oldPen = m_dcGrid.SelectObject (&solidPen) ; 
	m_dcGrid.MoveTo (m_rectPlot.left, m_rectPlot.top) ;
	m_dcGrid.LineTo (m_rectPlot.right + 1, m_rectPlot.top) ;
	m_dcGrid.LineTo (m_rectPlot.right + 1, m_rectPlot.bottom+1) ;
	m_dcGrid.LineTo (m_rectPlot.left, m_rectPlot.bottom+1) ;
	m_dcGrid.LineTo (m_rectPlot.left, m_rectPlot.top) ;
	m_dcGrid.SelectObject (oldPen) ; 

	nMidGridPix    = m_rectPlot.top;
	for(int j = 0; j < m_pery; j++)
	{
		nTopGridPix    = nMidGridPix - m_nPlotHeight / m_pery * j;
		nBottomGridPix = nMidGridPix + m_nPlotHeight / m_pery * j;

		for (i = m_rectPlot.left; i < m_rectPlot.right; i++)
		{
			m_dcGrid.SetPixel (i, nTopGridPix + m_movex,    m_crGridColor) ;
			m_dcGrid.SetPixel (i, nMidGridPix + m_movex,    m_crGridColor) ;
			m_dcGrid.SetPixel (i, nBottomGridPix, m_crGridColor) ;
		}
	}

	nMidGridPix    = m_rectPlot.left;
	for(int j = 0; j < m_perx + 1; j++)
	{
		nTopGridPix    = nMidGridPix - m_nPlotWidth / m_perx * j;
		nBottomGridPix = nMidGridPix + m_nPlotWidth / m_perx * j;
		for (i = m_rectPlot.top; i < m_rectPlot.bottom; i++)
		{
			m_dcGrid.SetPixel (nTopGridPix + m_movey, i,    m_crGridColor) ;
			m_dcGrid.SetPixel (nMidGridPix + m_movey, i,    m_crGridColor) ;
			m_dcGrid.SetPixel (nBottomGridPix, i, m_crGridColor) ;
		}
		}
	// ���������������
	axisFont.CreateFont (14, 0, 0, 0, 300,
						FALSE, FALSE, 0, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, 
						CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, 
						DEFAULT_PITCH|FF_SWISS, "Arial") ;
	yUnitFont.CreateFont (14, 0, 900, 0, 300,
						FALSE, FALSE, 0, ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS, 
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, 
                       DEFAULT_PITCH|FF_SWISS, "Arial") ;
  
	// ��������
	oldFont = m_dcGrid.SelectObject(&axisFont) ;
  
	// ��ʾy�����ֵ
	m_dcGrid.SetTextColor (m_crGridColor) ;
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
	strTemp.Format ("%.*lf", m_nYDecimals, m_dUpperLimit) ;
	m_dcGrid.TextOut (m_rectPlot.left-4, m_rectPlot.top, strTemp) ;

	// ��ʾy����Сֵ
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_BASELINE) ;
	strTemp.Format ("%.*lf", m_nYDecimals, m_dLowerLimit) ;
	m_dcGrid.TextOut (m_rectPlot.left-4, m_rectPlot.bottom, strTemp) ;

	// ��ʾx����Сֵ
	m_dcGrid.SetTextAlign (TA_LEFT|TA_TOP) ;
	m_dcGrid.TextOut (m_rectPlot.left, m_rectPlot.bottom+4, "0") ;

	// x�����ֵ
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
	strTemp.Format ("%d", m_nPlotWidth/m_nShiftPixels) ; 
	m_dcGrid.TextOut (m_rectPlot.right, m_rectPlot.bottom+4, strTemp) ;

	// x������
	m_dcGrid.SetTextAlign (TA_CENTER|TA_TOP) ;
	m_dcGrid.TextOut ((m_rectPlot.left+m_rectPlot.right)/2, 
						m_rectPlot.bottom+4, m_strXUnitsString) ;

	// �ָ�����
	m_dcGrid.SelectObject(oldFont) ;

	// y������
	oldFont = m_dcGrid.SelectObject(&yUnitFont) ;
	m_dcGrid.SetTextAlign (TA_CENTER|TA_BASELINE) ;
	m_dcGrid.TextOut ((m_rectClient.left+m_rectPlot.left)/2, 
						(m_rectPlot.bottom+m_rectPlot.top)/2, m_strYUnitsString) ;
	m_dcGrid.SelectObject(oldFont) ;

	if (m_dcPlot.GetSafeHdc() == NULL)
	{
		m_dcPlot.CreateCompatibleDC(&dc) ;
		m_bitmapPlot.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
		m_pbitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot) ;
	}

	// �������
	m_dcPlot.SetBkColor (m_crBackColor) ;
	m_dcPlot.FillRect(m_rectClient, &m_brushBack) ;

	// �ػ�
	InvalidateRect(m_rectClient) ;

} // InvalidateCtrl


/////////////////////////////////////////////////////////////////////////////
double COScopeCtrl::AppendPoint(double dNewPoint)
{
	// ���ӵ㲢����

	double dPrevious ;
  
	dPrevious = m_dCurrentPosition ;
	m_dCurrentPosition = dNewPoint ;
	DrawPoint() ;

	Invalidate() ;

	return dPrevious ;
} // AppendPoint
 
////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::OnPaint() 
{
	CPaintDC dc(this) ;  // �����Ļ�ͼ�豸����
	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // Դ
	// �õ�һ���豸����
	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

	if (memDC.GetSafeHdc() != NULL)
	{// ˫����
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
                 &m_dcGrid, 0, 0, SRCCOPY) ;
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
					&m_dcPlot, 0, 0, SRCPAINT) ;  //SRCPAINT
		// ������ʾ����
		dc.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
					&memDC, 0, 0, SRCCOPY) ;
  }

  memDC.SelectObject(oldBitmap) ;

} // OnPaint

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::DrawPoint()
{
	int currX, prevX, currY, prevY ;

	CPen *oldPen ;
	CRect rectCleanUp ;

	if (m_dcPlot.GetSafeHdc() != NULL)
	{
		m_dcPlot.BitBlt(m_rectPlot.left, m_rectPlot.top + 1, 
						m_nPlotWidth, m_nPlotHeight, &m_dcPlot, 
						m_rectPlot.left + m_nShiftPixels, m_rectPlot.top + 1, 
						SRCCOPY) ;

		// �����Χ
		rectCleanUp = m_rectPlot ;
		rectCleanUp.left  = rectCleanUp.right - m_nShiftPixels ;

		// �ñ���ɫ��������
		m_dcPlot.FillRect(rectCleanUp, &m_brushBack) ;
		oldPen = m_dcPlot.SelectObject(&m_penPlot) ;

		// �Ƶ�ָ��λ��
		prevX = m_rectPlot.right-m_nPlotShiftPixels ;
		prevY = m_rectPlot.bottom - 
				(long)((m_dPreviousPosition - m_dLowerLimit) * m_dVerticalFactor) ;
		m_dcPlot.MoveTo (prevX, prevY) ;

		// ����ǰ��
		currX = m_rectPlot.right - m_nHalfShiftPixels ;
		currY = m_rectPlot.bottom -
				(long)((m_dCurrentPosition - m_dLowerLimit) * m_dVerticalFactor) ;
		m_dcPlot.LineTo (currX, currY) ;

		// �����ʱ��� 
		m_dcPlot.SelectObject(oldPen) ;

		// �߽��ж�
		if ((prevY <= m_rectPlot.top) || (currY <= m_rectPlot.top))
			m_dcPlot.FillRect(CRect(prevX, m_rectClient.top, currX + 1, m_rectPlot.top + 1), &m_brushBack) ;
		if ((prevY >= m_rectPlot.bottom) || (currY >= m_rectPlot.bottom))
			m_dcPlot.FillRect(CRect(prevX, m_rectPlot.bottom+1, currX+1, m_rectClient.bottom + 1), &m_brushBack) ;

		// ���浱ǰ��������һ������
		m_dPreviousPosition = m_dCurrentPosition ;

	}

} // end DrawPoint

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy) ;

	// ������OnSizeʱ����
  
	GetClientRect(m_rectClient) ;

	// ����ͻ�������
	m_nClientHeight = m_rectClient.Height() ;
	m_nClientWidth  = m_rectClient.Width() ;

	// �ı�����
	m_rectPlot.left   = 20 ;  
	m_rectPlot.top    = 10 ;
	m_rectPlot.right  = m_rectClient.right - 10 ;
	m_rectPlot.bottom = m_rectClient.bottom - 25 ;

	// ���泤��
	m_nPlotHeight = m_rectPlot.Height() ;
	m_nPlotWidth  = m_rectPlot.Width() ;

	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 

} // OnSize


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::Reset()
{
	// �ػ�ؼ�
	InvalidateCtrl() ;
}
