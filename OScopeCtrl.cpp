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
	// 初始位
	m_dPreviousPosition =   0.0 ;

	// y轴上数字
	m_nYDecimals = 4 ;
	m_perx = 6;
	m_pery = 6;
	m_movex = 0;
	m_movey = 0;

	// 图表的边界限制
	m_dLowerLimit = -10.0 ;
	m_dUpperLimit =  10.0 ;
	m_dRange      =  m_dUpperLimit - m_dLowerLimit ;   

	m_nShiftPixels     = 4 ;
	m_nHalfShiftPixels = m_nShiftPixels / 2 ;                     
	m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels ;  

	m_crBackColor  = RGB(  0,   0,   0) ;  // 由SetBackgroundColor设置的背景颜色，构造时为黑色
	m_crGridColor  = RGB(  0, 255, 255) ;  // 由SetGridColor设置的表格颜色，构造时为绿色
	m_crPlotColor  = RGB(255, 255, 255) ;  // 由SetPlotColor设置的曲线颜色，构造时为白色

	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor) ;
	m_brushBack.CreateSolidBrush(m_crBackColor) ;

	m_strXUnitsString.Format("Time") ;		// x轴上的文字
	m_strYUnitsString.Format("Y units") ;		// y轴上的文字

	// 刚开始时给bitmap的内存地址取NULL~
	m_pbitmapOldGrid = NULL ;
	m_pbitmapOldPlot = NULL ;

}  // COScopeCtrl

/////////////////////////////////////////////////////////////////////////////
COScopeCtrl::~COScopeCtrl()
{
	// 写控件最重要的就是要注意析构时释放内存啦~
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
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;// 注册类

	  result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 
							className, NULL, dwStyle, 
							rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
							pParentWnd->GetSafeHwnd(), (HMENU)nID) ;		// 生成
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
  
 // 重绘控件
  InvalidateCtrl() ;

}  // SetRange


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetXUnits(CString string)
{
  m_strXUnitsString = string ;

  // 重绘控件
  InvalidateCtrl() ;

}  // SetXUnits

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetYUnits(CString string)
{
	m_strYUnitsString = string ;

	// 重绘控件
	InvalidateCtrl() ;

}  // SetYUnits

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetGridColor(COLORREF color)
{
	m_crGridColor = color ;

	// 重绘控件
	InvalidateCtrl() ;

}  // SetGridColor


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetPlotColor(COLORREF color)
{
	m_crPlotColor = color ;

	m_penPlot.DeleteObject() ;
	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor) ;

	// 重绘控件
	InvalidateCtrl() ;

}  // SetPlotColor


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetBackgroundColor(COLORREF color)
{
  m_crBackColor = color ;

  m_brushBack.DeleteObject() ;
  m_brushBack.CreateSolidBrush(m_crBackColor) ;

   // 重绘控件
  InvalidateCtrl() ;

}  // SetBackgroundColor

void COScopeCtrl::SetPerx(UINT x)
{
	m_perx = x ;

	// 重绘控件
	InvalidateCtrl() ;

}  // SetPerx

void COScopeCtrl::SetPery(UINT y)
{
	m_pery = y ;

	// 重绘控件
	InvalidateCtrl() ;

}  // SetPery

void COScopeCtrl::SetMovex(UINT x)
{
	m_movey = x ;

	// 重绘控件
	InvalidateCtrl() ;

}  // SetPery

void COScopeCtrl::SetMovey(UINT y)
{
	m_movey = y ;

	// 重绘控件
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

	// 如果为空，则为它重新申请
	if (m_dcGrid.GetSafeHdc() == NULL)
	{
		m_dcGrid.CreateCompatibleDC(&dc) ;
		m_bitmapGrid.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
		m_pbitmapOldGrid = m_dcGrid.SelectObject(&m_bitmapGrid) ;
	}
  
	m_dcGrid.SetBkColor (m_crBackColor) ;

	// 填充表格区域
	m_dcGrid.FillRect(m_rectClient, &m_brushBack) ;

	nCharacters = abs((int)log10(fabs(m_dUpperLimit))) ;
	nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit)))) ;

	nCharacters = nCharacters + 4 + m_nYDecimals ;  

	m_rectPlot.left = m_rectClient.left + 6*(nCharacters) ;
	m_nPlotWidth    = m_rectPlot.Width() ;

	// 画矩形
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
	// 产生横坚两种字体
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
  
	// 运用字体
	oldFont = m_dcGrid.SelectObject(&axisFont) ;
  
	// 显示y的最大值
	m_dcGrid.SetTextColor (m_crGridColor) ;
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
	strTemp.Format ("%.*lf", m_nYDecimals, m_dUpperLimit) ;
	m_dcGrid.TextOut (m_rectPlot.left-4, m_rectPlot.top, strTemp) ;

	// 显示y的最小值
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_BASELINE) ;
	strTemp.Format ("%.*lf", m_nYDecimals, m_dLowerLimit) ;
	m_dcGrid.TextOut (m_rectPlot.left-4, m_rectPlot.bottom, strTemp) ;

	// 显示x的最小值
	m_dcGrid.SetTextAlign (TA_LEFT|TA_TOP) ;
	m_dcGrid.TextOut (m_rectPlot.left, m_rectPlot.bottom+4, "0") ;

	// x的最大值
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
	strTemp.Format ("%d", m_nPlotWidth/m_nShiftPixels) ; 
	m_dcGrid.TextOut (m_rectPlot.right, m_rectPlot.bottom+4, strTemp) ;

	// x轴文字
	m_dcGrid.SetTextAlign (TA_CENTER|TA_TOP) ;
	m_dcGrid.TextOut ((m_rectPlot.left+m_rectPlot.right)/2, 
						m_rectPlot.bottom+4, m_strXUnitsString) ;

	// 恢复字体
	m_dcGrid.SelectObject(oldFont) ;

	// y轴文字
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

	// 填充区域
	m_dcPlot.SetBkColor (m_crBackColor) ;
	m_dcPlot.FillRect(m_rectClient, &m_brushBack) ;

	// 重绘
	InvalidateRect(m_rectClient) ;

} // InvalidateCtrl


/////////////////////////////////////////////////////////////////////////////
double COScopeCtrl::AppendPoint(double dNewPoint)
{
	// 增加点并返回

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
	CPaintDC dc(this) ;  // 真正的绘图设备环境
	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // 源
	// 得到一个设备环境
	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

	if (memDC.GetSafeHdc() != NULL)
	{// 双缓冲
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
                 &m_dcGrid, 0, 0, SRCCOPY) ;
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
					&m_dcPlot, 0, 0, SRCPAINT) ;  //SRCPAINT
		// 最终显示出来
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

		// 清除范围
		rectCleanUp = m_rectPlot ;
		rectCleanUp.left  = rectCleanUp.right - m_nShiftPixels ;

		// 用背景色填充该区域
		m_dcPlot.FillRect(rectCleanUp, &m_brushBack) ;
		oldPen = m_dcPlot.SelectObject(&m_penPlot) ;

		// 移到指定位置
		prevX = m_rectPlot.right-m_nPlotShiftPixels ;
		prevY = m_rectPlot.bottom - 
				(long)((m_dPreviousPosition - m_dLowerLimit) * m_dVerticalFactor) ;
		m_dcPlot.MoveTo (prevX, prevY) ;

		// 画当前点
		currX = m_rectPlot.right - m_nHalfShiftPixels ;
		currY = m_rectPlot.bottom -
				(long)((m_dCurrentPosition - m_dLowerLimit) * m_dVerticalFactor) ;
		m_dcPlot.LineTo (currX, currY) ;

		// 将画笔保存 
		m_dcPlot.SelectObject(oldPen) ;

		// 边界判断
		if ((prevY <= m_rectPlot.top) || (currY <= m_rectPlot.top))
			m_dcPlot.FillRect(CRect(prevX, m_rectClient.top, currX + 1, m_rectPlot.top + 1), &m_brushBack) ;
		if ((prevY >= m_rectPlot.bottom) || (currY >= m_rectPlot.bottom))
			m_dcPlot.FillRect(CRect(prevX, m_rectPlot.bottom+1, currX+1, m_rectClient.bottom + 1), &m_brushBack) ;

		// 保存当前点以与下一点连接
		m_dPreviousPosition = m_dCurrentPosition ;

	}

} // end DrawPoint

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy) ;

	// 父窗口OnSize时调用
  
	GetClientRect(m_rectClient) ;

	// 保存客户区长宽
	m_nClientHeight = m_rectClient.Height() ;
	m_nClientWidth  = m_rectClient.Width() ;

	// 改变区域
	m_rectPlot.left   = 20 ;  
	m_rectPlot.top    = 10 ;
	m_rectPlot.right  = m_rectClient.right - 10 ;
	m_rectPlot.bottom = m_rectClient.bottom - 25 ;

	// 保存长宽
	m_nPlotHeight = m_rectPlot.Height() ;
	m_nPlotWidth  = m_rectPlot.Width() ;

	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 

} // OnSize


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::Reset()
{
	// 重绘控件
	InvalidateCtrl() ;
}
