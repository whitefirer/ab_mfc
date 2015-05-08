// Dlg3.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Dlg3.h"

BOOL bend = FALSE;
CWinThread *pThread;
CImage m_imPicture, m_imNewPicture;
int nType = 0;
float m_zoom = 1.0;
// CDlg3 对话框

IMPLEMENT_DYNAMIC(CDlg3, CDialog)

CDlg3::CDlg3(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg3::IDD, pParent)
{
	m_imPicture.Destroy();
	m_imNewPicture.Destroy();
}

CDlg3::~CDlg3()
{
	m_imPicture.Destroy();
}

void CDlg3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg3, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg3::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON3, &CDlg3::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlg3::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlg3::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlg3::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlg3::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlg3::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlg3::OnBnClickedButton8)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

class Wave
{
public:


};
// CDlg3 消息处理程序

BOOL CDlg3::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CDlg3::OnPaint()
{		
	CPaintDC dc(this);

	if (!m_imNewPicture.IsNull()) 
	{
		m_imNewPicture.Draw(dc,0,0);
	}
	else
	{
		
		CBitmap bmpDraw;
		CDC memDC;
		bmpDraw.LoadBitmap(IDB_SPLASH);			// 装入位图资源
		BITMAP bm;
		bmpDraw.GetBitmap(&bm);					// 获取位图的尺寸

		memDC.CreateCompatibleDC(&dc);			//创建兼容DC

		CBitmap *pbmpOld = memDC.SelectObject(&bmpDraw);

		dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCAND);
		memDC.SelectObject(pbmpOld);
	}
}

int CDlg3::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	pThread = NULL;
	bend = FALSE;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetClientRect(&m_rect);
	return 0;
}


void CDlg3::Adjust(CWnd *pWnd, int cx, int cy)
{
	if(pWnd)												//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;											//获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);								//将控件大小转换为在对话框中的区域坐标
		//　cx/m_rect.Width()为对话框在横向的变化比例
		if(0 == m_rect.Width() || 0 == m_rect.Height())
			return;
		rect.left = rect.left * cx / m_rect.Width();				//调整控件大小
		rect.right = rect.right * cx / m_rect.Width();
		rect.top = rect.top * cy / m_rect.Height();
		rect.bottom = rect.bottom * cy / m_rect.Height();
		pWnd->MoveWindow(rect);								//设置控件大小
	}
}

void CDlg3::OnSize(UINT nType, int cx, int cy)
{

	CDialog::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	if(0 == rect.Width() || 0 == rect.Height())
		return;

	Adjust(GetDlgItem(IDC_BUTTON1), cx, cy);
	Adjust(GetDlgItem(IDC_BUTTON2), cx, cy);
	Adjust(GetDlgItem(IDC_BUTTON3), cx, cy);
	Adjust(GetDlgItem(IDC_BUTTON4), cx, cy);
	Adjust(GetDlgItem(IDC_BUTTON5), cx, cy);
	Adjust(GetDlgItem(IDC_BUTTON6), cx, cy);
	Adjust(GetDlgItem(IDC_BUTTON7), cx, cy);
	Adjust(GetDlgItem(IDC_BUTTON8), cx, cy);

	GetClientRect(&m_rect);
}


UINT DrawIt(LPVOID pParam)
{
	CClientDC dc((CWnd*)pParam);
	CDC memDC;

	HBITMAP hBitmap;
	CBitmap cBitmap;
	BITMAP bm;

	HINSTANCE hApp = ::GetModuleHandle(NULL);				// 得到应用程序的模块句柄 
	hBitmap = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH)); 
	if(hBitmap == NULL) 
	{
		((CWnd*)pParam)->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		return 1;
	}
	cBitmap.Attach(hBitmap);
	cBitmap.GetBitmap(&bm);

	memDC.CreateCompatibleDC(((CWnd*)pParam)->GetDC());
	memDC.SelectObject(&cBitmap);

	for (int i = 0; (i <= bm.bmHeight) && (!bend); i++ )   
	{
		for (int j = 0; (j <= bm.bmHeight - i) && (!bend); j++ )  
			dc.StretchBlt(0, j, bm.bmWidth, 1, &memDC, 0, bm.bmHeight - i, bm.bmWidth, 1, SRCCOPY);  

		Sleep(20); 
	}

	::DeleteObject(hBitmap);
	::DeleteDC(memDC);

	if(!bend)
		((CWnd*)pParam)->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);

	return 0;
}

void CDlg3::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码 
	bend = TRUE;
	if(pThread)												// 改变变量，线程结束
		WaitForSingleObject(pThread->m_hThread, INFINITE);	// 等待线程结束
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);			// 使按钮恢复
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);			// 使按钮恢复
	bend = FALSE;
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);			// 使按钮灰化
	pThread = AfxBeginThread(DrawIt, this);					// 使用多线程以防止绘图时窗口不动
	pThread->m_bAutoDelete = FALSE;							// 线程为手动删除 
}

void CDlg3::OnDestroy()
{
	bend = TRUE;	
	if(pThread)												// 改变变量，线程结束
	{
		WaitForSingleObject(pThread->m_hThread, INFINITE);	// 等待线程结束
		delete pThread;										// 删除线程 
	}

	CDialog::OnDestroy();									// 为什么放在
	// TODO: 在此处添加消息处理程序代码
}

void CDlg3::Bilinear(float k)
{  
    int nBpp = m_imPicture.GetBPP ();   
    int widthNew, heightNew;//新图像的宽度和高度
    float widthScale = (float)(1.0 / k), heightScale = (float)(1.0 / k);
    float xx, yy;
    int a, b;
    int rr, gg, bb;//保存R、G、B分量

    //得到新图像的宽度和高度

    widthNew =(int)(m_imPicture.GetWidth() * k);
    heightNew =(int)(m_imPicture.GetHeight() * k);

    //利用新图像的宽度和高度来创建新图像

    m_imNewPicture.Destroy();
    m_imNewPicture.Create(widthNew, heightNew, nBpp);
	if(!m_imNewPicture) 
	{
		m_zoom = m_zoom + 0.1;
		Bilinear(m_zoom);
	}

    //得到新、老图像的每行的字节数

    int nPitch = m_imPicture.GetPitch();
    int nPitchNew = m_imNewPicture.GetPitch();

    //得到新、老图像的数据指针

    LPBYTE pBitsNew = (LPBYTE)m_imNewPicture.GetBits();
    LPBYTE pBits = (LPBYTE)m_imPicture.GetBits();  

    if(m_imPicture.GetBPP() != 24)
	{
        MessageBox("必须是24位图像或8位图像");
        m_imNewPicture.Destroy();
        Invalidate();
        return ;
    }  

    for(int x = (int)k; x < widthNew - k; x++)
	{
        for(int y = (int)k; y < heightNew - k; y++)
		{
            xx = x * widthScale ;
            yy = y * heightScale ;

            if(xx <= 1e-8)
			{
                xx = 0;
            }

            if(xx > m_imPicture.GetWidth() - 2)
                xx = (float)(m_imPicture.GetWidth() - 2);

            if(yy <= 1e-8)
                yy = 0;

            if(yy > m_imPicture.GetHeight() - 2)
                yy = (float)(m_imPicture.GetHeight() - 2);

			a = (int)xx;
			b = (int)yy;         

            //分别得到对应像素的R、G、B值并用双线性插值得到新像素的R、G、B值

            int r11,r12,r21,r22;

            r11 = *(pBits + b * nPitch + 3 * a + 2);
            r12 = *(pBits + b * nPitch + 3 * (a + 1) + 2);
            r21 = *(pBits + (b + 1) * nPitch + 3 * a + 2);
            r22 = *(pBits + (b + 1) * nPitch + 3 * (a + 1) + 2);
            rr = (int)(r11 * (a + 1 - xx) * (b + 1 - yy) + r12 * (a+1 - xx) * (yy - b)
                + r21 * (xx - a) * (b + 1 - yy) + r22 * (xx - a) * (yy - b));


            int g11, g12, g21, g22;

            g11 = *(pBits + b * nPitch + 3 * a + 1);
            g12 = *(pBits + b * nPitch + 3 * (a + 1) + 1);
            g21 = *(pBits + (b + 1) * nPitch + 3 * a + 1);
            g22 = *(pBits + (b + 1) * nPitch + 3 * (a + 1) + 1);
            gg = (int)(g11 * (a + 1 - xx) * (b + 1 - yy) + g12 * (a + 1 - xx) * (yy- b)
				+ g21 * (xx - a) * (b + 1 - yy) + g22 * (xx - a) * (yy - b));
 

            int b11, b12, b21, b22;

            b11 = *(pBits + b * nPitch + 3 * a);
            b12 = *(pBits + b * nPitch + 3 * (a + 1));
            b21 = *(pBits + (b + 1) * nPitch + 3 * a);
            b22 = *(pBits + (b + 1) * nPitch + 3 * (a+1));
            bb = (int)(b11 * (a + 1 - xx) * (b + 1 - yy) + b12 * (a + 1 - xx) * (yy - b)
                + b21 * (xx - a) * (b + 1 - yy) + b22 * (xx - a) * (yy - b));

            //将得到的新R、G、B值写到新图像中          

            *(pBitsNew + y * nPitchNew + x * 3) = min(255, bb);
            *(pBitsNew + y * nPitchNew + x * 3 + 1) = min(255, gg);
            *(pBitsNew + y * nPitchNew + x * 3 + 2) = min(255, rr);
        }      
    }      
    

    Invalidate();
}

UINT FilterImage(LPVOID pParam)
{

	if (m_imNewPicture.IsNull())
		return 1;

	int smoothGauss[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1}; // 高斯模板
	int sharpLaplacian[9] = {-1, -1, -1, -1, 9, -1, -1, -1, -1}; // 拉普拉斯模板
	int opTemp[9];

	float aver; // 系数

	if( nType > 1)
		nType = 0;

	switch(nType)
	{
	case 0: // 高斯模板 平滑
		aver = (float)(1.0 / 16.0);
		memcpy(opTemp, smoothGauss, 9 * sizeof(int));
		break;

	case 1: // 拉普拉斯模板 锐化
		aver = 1.0;
		memcpy(opTemp, sharpLaplacian, 9 * sizeof(int));
		break;
	}

	int i,j;
	int nWidth = m_imNewPicture.GetWidth();
	int nHeight = m_imNewPicture.GetHeight();

	for (i = 1; i < nWidth - 1; i++)
	{
		for (j = 1; j < nHeight - 1; j++)
		{
			int rr = 0, gg = 0, bb = 0;
			int index = 0;
			for (int col = -1; col <= 1; col++)
			{
				for (int row = -1; row <= 1; row++)
				{
					if(bend) return 1;
					COLORREF clr = m_imNewPicture.GetPixel(i + row, j + col);
					rr += GetRValue(clr) * opTemp[index];
					gg += GetGValue(clr) * opTemp[index];
					bb += GetBValue(clr) * opTemp[index];
					index++;
				}
			}

			rr = (int)(rr * aver);
			gg = (int)(gg * aver);
			bb = (int)(bb * aver);

			// 处理溢出点

			if ( rr > 255 ) rr = 255;
			else if ( rr < 0 ) rr = -rr;

			if ( gg > 255 ) gg = 255;
			else if ( gg < 0 ) gg = -gg;

			if ( bb > 255 ) bb = 255;
			else if ( bb < 0 ) bb = -bb;

			// 错位重写以避免前一个像素被新的像素覆盖
			m_imNewPicture.SetPixel( i - 1, j - 1, RGB(rr, gg, bb));
		}
	}
	((CWnd*)pParam)->Invalidate(); // 强制调用OnDraw
	if(!bend)
	{
		((CWnd*)pParam)->GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		((CWnd*)pParam)->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	}
	return 0;
}

void CDlg3::OnBnClickedButton2()
{
	HRESULT hResult;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, 
		"全部文件|*.*|jpg|*.jpg|bmp|*.bmp|png|*.png|gif|*.gif");

	if(IDOK != dlg.DoModal())
		return;

	m_imPicture.Destroy();

	// 将外部图像文件装载到CImage对象中
	hResult = m_imPicture.Load(dlg.GetFileName());
	if (FAILED(hResult)) 
	{
		MessageBox(_T("调用图像文件失败！"));
		return;
	}

	// 设置主窗口标题栏内容

	CString str;
	str.LoadString(AFX_IDS_APP_TITLE);
	AfxGetMainWnd()->SetWindowText(str + _T(" - ") +dlg.GetFileName());
	bend = TRUE;
	if(pThread)												// 改变变量，线程结束
		WaitForSingleObject(pThread->m_hThread, INFINITE);	// 等待线程结束
	bend = FALSE;

	Bilinear(m_zoom);
}

void CDlg3::OnBnClickedButton3()
{
	if (m_imNewPicture.IsNull()) 
	{
		MessageBox("未载入外部图片！");
		return ;
	}
	bend = TRUE;
	if(pThread)												// 改变变量，线程结束
		WaitForSingleObject(pThread->m_hThread, INFINITE);	// 等待线程结束
	CPaintDC dc(this);

	if (!m_imNewPicture.IsNull()) 
	{
		m_imNewPicture.Draw(dc,0,0);
	}

	bend = FALSE;
	nType = 0;
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);			// 使按钮恢复
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);			// 使按钮灰化
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);			// 使按钮灰化
	pThread = AfxBeginThread(FilterImage, this);			// 使用多线程以防止绘图时窗口不动
	pThread->m_bAutoDelete = FALSE;							// 线程为手动删除 
	
}

void CDlg3::OnBnClickedButton4()
{
	if (m_imNewPicture.IsNull()) 
	{
		MessageBox("未载入外部图片！");
		return ;
	}
	bend = TRUE;
	if(pThread)												// 改变变量，线程结束
		WaitForSingleObject(pThread->m_hThread, INFINITE);	// 等待线程结束
	CPaintDC dc(this);

	if (!m_imNewPicture.IsNull()) 
	{
		m_imNewPicture.Draw(dc,0,0);
	}

	bend = FALSE;
	nType = 1;
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);			// 使按钮恢复
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);			// 使按钮灰化
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);			// 使按钮灰化
	pThread = AfxBeginThread(FilterImage, this);			// 使用多线程以防止绘图时窗口不动
	pThread->m_bAutoDelete = FALSE;							// 线程为手动删除 
}

void CDlg3::OnBnClickedButton5()
{
	CString strFilter;
	strFilter = "bmp位图文件|*.bmp|JPEG 图像文件|*.jpg| \
			GIF 图像文件|*.gif|PNG 图像文件|*.png||";
	CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	if ( IDOK != dlg.DoModal())
		return;

	// 如果用户没有指定文件扩展名，则为其添加一个
	CString strFileName;
	CString strExtension;

	strFileName = dlg.m_ofn.lpstrFile;
	if (dlg.m_ofn.nFileExtension == 0)
	{
		switch (dlg.m_ofn.nFilterIndex)
		{
			case 1:
				strExtension = "bmp"; break;
			case 2:
				strExtension = "jpg"; break;
			case 3:
				strExtension = "gif"; break;
			case 4:
				strExtension = "png"; break;
			default:
				break;
		}
		strFileName = strFileName + '.' + strExtension;
	}

	// 图像保存
	HRESULT hResult = m_imNewPicture.Save(strFileName);
	if (FAILED(hResult))
		MessageBox("保存图像文件失败！");
}

void CDlg3::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_imPicture.IsNull() || m_imNewPicture.IsNull()) 
	{
		MessageBox("未载入外部图片！");
		return ;
	}

	bend = TRUE;
	if(pThread)												// 改变变量，线程结束
		WaitForSingleObject(pThread->m_hThread, INFINITE);	// 等待线程结束
	bend = FALSE;

	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);			// 使按钮恢复
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);			// 使按钮恢复
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);			// 使按钮恢复

	m_zoom = m_zoom - 0.1;
	Bilinear(m_zoom);
}

void CDlg3::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_imPicture.IsNull() || m_imNewPicture.IsNull()) 
	{
		MessageBox("未载入外部图片！");
		return ;
	}

	bend = TRUE;
	if(pThread)												// 改变变量，线程结束
		WaitForSingleObject(pThread->m_hThread, INFINITE);	// 等待线程结束
	bend = FALSE;

	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);			// 使按钮恢复
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);			// 使按钮恢复
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);			// 使按钮恢复

	m_zoom = m_zoom + 0.1;
	Bilinear(m_zoom);
}

void CDlg3::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_imPicture.IsNull() || m_imNewPicture.IsNull()) 
	{
		MessageBox("未载入外部图片！");
		return ;
	}

	bend = TRUE;
	if(pThread)												// 改变变量，线程结束
		WaitForSingleObject(pThread->m_hThread, INFINITE);	// 等待线程结束
	bend = FALSE;

	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);			// 使按钮恢复
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);			// 使按钮恢复
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);			// 使按钮恢复

	m_zoom = 1.0;
	Bilinear(m_zoom);
}

void CDlg3::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SCROLLINFO si;  
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_HORZ,&si);

	int nPrePosH = si.nPos;

	switch(nSBCode)
	{
	case SB_LEFT:
		si.nPos = si.nMin;
	break;
	case   SB_RIGHT:
		si.nPos = si.nMax;
	break;

	case SB_LINELEFT:
		si.nPos -= 1;
	break;
	case SB_LINERIGHT:
		si.nPos += 1;
	break;
	case SB_PAGELEFT:
		si.nPos -= 1;
		break;
	case SB_PAGERIGHT:
		si.nPos += 1;
	break;
	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos;
		break;
	default:
		break;  
	}

	si.fMask = SIF_POS;
	SetScrollInfo(SB_HORZ, &si, TRUE);
	GetScrollInfo(SB_HORZ, &si);
	CRect rc;
	GetClientRect(&rc);
                                     
	ScrollWindow(nPrePosH - si.nPos, 0, NULL, NULL);
	UpdateWindow();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlg3::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SCROLLINFO si;  
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT,&si);

	int nPrePosV = si.nPos;

	switch(nSBCode)
	{
	case SB_TOP:
		si.nPos = si.nMin;
	break;
	case SB_BOTTOM:
		si.nPos = si.nMax;
	break;

	case SB_LINEUP:
		si.nPos -= 1;
	break;
	case SB_LINEDOWN:
		si.nPos += 1;
	break;
	case SB_PAGEUP:
		si.nPos -= 1;
		break;
	case SB_PAGEDOWN:
		si.nPos += 1;
	break;
	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos;
		break;
	default:
		break;  
	}

	si.fMask = SIF_POS;
	SetScrollInfo(SB_VERT, &si, TRUE);
	GetScrollInfo(SB_VERT, &si);
	CRect rc;
	GetClientRect(&rc);
                                     
	ScrollWindow(0, nPrePosV - si.nPos, NULL, NULL);
	UpdateWindow();

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
