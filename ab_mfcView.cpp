// ab_mfcView.cpp : Cab_mfcView 类的实现
//

#include "stdafx.h"
#include "ab_mfc.h"

#include "ab_mfcDoc.h"
#include "ab_mfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cab_mfcView

IMPLEMENT_DYNCREATE(Cab_mfcView, CView)

BEGIN_MESSAGE_MAP(Cab_mfcView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_CONSOLE, &Cab_mfcView::OnConsole)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// Cab_mfcView 构造/析构

Cab_mfcView::Cab_mfcView()
{
	m_size =195;
	m_xRo = 0;
	m_yRo = 0;
	m_zRo = 0;
	m_xTra=10;
	m_yTra=5;
	m_zTra=-30;
	frenqu=5;  //胳膊转速
	// TODO: 在此处添加构造代码
	isConsole = FALSE;
}

Cab_mfcView::~Cab_mfcView()
{
}

BOOL Cab_mfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cab_mfcView 绘制

void Cab_mfcView::OnDraw(CDC* /*pDC*/)
{
	Cab_mfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	RenderScene();  //渲染场景
	::SwapBuffers(m_hDC);//交互缓冲区
}


// Cab_mfcView 打印

BOOL Cab_mfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cab_mfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Cab_mfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// Cab_mfcView 诊断

#ifdef _DEBUG
void Cab_mfcView::AssertValid() const
{
	CView::AssertValid();
}

void Cab_mfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cab_mfcDoc* Cab_mfcView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cab_mfcDoc)));
	return (Cab_mfcDoc*)m_pDocument;
}
#endif //_DEBUG


#include <io.h>
#include <cstdio>
#include <FCNTL.H>

void Cab_mfcView::OnConsole()
{
	if(!isConsole)
	{
		AllocConsole();
		freopen( "CONOUT$", "w+t", stdout );// 申请写
		freopen( "CONIN$", "r+t", stdin );// 申请读
	}
	else
	{
		FreeConsole();
	}
	isConsole = !isConsole;
}
int Cab_mfcView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//初始化
	HWND hWnd = GetSafeHwnd();
	m_hDC = ::GetDC(hWnd);
	//建立窗口格式
	if(SetWindowPixelFormat(m_hDC)==FALSE)
		return 0;
	//建立RC
	m_hRC = wglCreateContext(m_hDC);
	if(m_hRC==NULL)
		return 0;
	if(wglMakeCurrent(m_hDC,m_hRC)==FALSE)
		return 0;
	//初始化OpenGL的各个参数	
	glClearColor(0,0,0,1.0);

	//设置光照
	GLfloat light_ambient[]={0.3f,0.2f,0.5f};
	GLfloat light_diffuse[]={1.0f,1.0f,1.0f};
	GLfloat light_position[] = { 2.0f, 2.0f, 2.0f, 1.0f };


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	JIQIREN();
	SetTimer(0,10,NULL);
	//////////////////////////////////////////////////
	return 0;
}

void Cab_mfcView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GLsizei width,height;
	GLdouble aspect;
	width = cx;
	height = cy;
	if(cy==0)
		aspect = (GLdouble)width;
	else
		aspect = (GLdouble)width/(GLdouble)height;
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,aspect,1,80);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

BOOL Cab_mfcView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
//	return CView::OnEraseBkgnd(pDC);
}

void Cab_mfcView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(wglGetCurrentContext()!=NULL)
	{
		wglMakeCurrent(NULL,NULL);
	}
	if(m_hRC!=NULL)
	{
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}
}

////////////////// 定义像素格式///////////////////////////
BOOL Cab_mfcView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW |//支持在窗口中绘图
		PFD_SUPPORT_OPENGL | //支持OpenGL  
		PFD_DOUBLEBUFFER |   //双缓存模式
		PFD_STEREO_DONTCARE;  

	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(m_GLPixelIndex == 0) // Choose default
	{
		m_GLPixelIndex = 1;
		if(DescribePixelFormat(hDC,m_GLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
			return FALSE;
	}

	if(!SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc))
		return FALSE;

	return TRUE;
}

void Cab_mfcView::RenderScene()//绘图工作主要在这里进行
{
// 	float b=0.0;
// 	float c=0.0;
// 	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除缓存  
// 	glBegin(GL_POINTS);
// 	for(int i=0;i<1024;i++)
// 	{
// 		b=b+1;
// 		for(int j=0;j<1024;j++)
// 		{ 
// 			glColor3f (1.0f, 0.0f, 0.0f);
// 			glVertex2f(c, b);
// 			c=c+1;
// 		}
// 	}
// 	glEnd();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    //深度缓存   
	glLoadIdentity(); 
	glTranslated(m_xTra,m_yTra,m_zTra);
	glRotated(m_xRo, 1.0, 0.0, 0.0);
	glRotated(m_yRo, 0.0, 1.0, 0.0);

	//躯干
	glCallList(1);

	//右胳膊
	glPushMatrix();
	glTranslated(3.0,2.5,0.0);
	glRotated(-m_zRo,0,0,1);
	glTranslated(3,0,0);
	glCallList(2);
	glPopMatrix();

	//左胳膊
	glPushMatrix();
	glTranslated(-3.0,2.5,0.0);
	glRotated(m_zRo,0,0,1);
	glTranslated(-3,0,0);
	glCallList(2);
	glPopMatrix();

	//头
	glPushMatrix();
	glTranslated(0,6.5,0);
	glCallList(3);
	glPopMatrix();

	//腿
	glPushMatrix();
	glTranslated(-2,-8,0);
	glCallList(4);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2,-14.5,0);
	glCallList(4);
	glPopMatrix();

	glPushMatrix();
	glTranslated(2,-8,0);
	glCallList(4);
	glPopMatrix();

	glPushMatrix();
	glTranslated(2,-14.5,0);
	glCallList(4);
	glPopMatrix();

	//眼
	glPushMatrix();
	glTranslated(-0.6,7,1.8);
	glCallList(5);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6,7,1.8);
	glCallList(5);
	glPopMatrix();

	//嘴
	glPushMatrix();
	glTranslated(0,5.8,2);
	glCallList(6);
	glPopMatrix();
}

//创建显示列表********************************************************
void Cab_mfcView::JIQIREN()
{	//躯干
	::glNewList(1,GL_COMPILE_AND_EXECUTE);
	glColor3f(.0,0.0,1.0);
	auxSolidBox(6,8.5,5);
	::glEndList();

	//胳膊
	::glNewList(2,GL_COMPILE_AND_EXECUTE);
	glColor3f(0.0,1.0,1.0);
	auxSolidBox(8,2,1);
	//auxSolidSphere(1.6);
	::glEndList();

	//头和脖子
	::glNewList(3,GL_COMPILE_AND_EXECUTE);
	glColor3f(1.0,1.0,0.0);
	auxSolidCube((GLdouble) 3);
	auxSolidBox((GLdouble) 1,(GLdouble) 0,(GLdouble) 1) ;//脖子
	::glEndList();

	//腿
	::glNewList(4,GL_COMPILE_AND_EXECUTE);
	glColor3f(0.0,1.0,.0);
	auxSolidBox((GLdouble) 2,(GLdouble) 6,(GLdouble) 3) ;
	::glEndList();

	//眼
	::glNewList(5,GL_COMPILE_AND_EXECUTE);
	glColor3f(1.0,.0,.0);
	auxSolidSphere(0.6);
	::glEndList();

	//嘴
	::glNewList(6,GL_COMPILE_AND_EXECUTE);
	glColor3f(1.0,0.0,1.0);
	auxSolidBox(1.5,0.6,0.2);
	::glEndList();

}
void Cab_mfcView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nChar)
	{
	case VK_LEFT:
		m_yRo -= 30;       break;
	case VK_RIGHT:
		m_yRo += 30;        break;
	case VK_UP:
		m_xRo += 30;        break;
	case VK_DOWN:
		m_xRo -= 30;        break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Cab_mfcView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_zRo -=frenqu;
	if(m_zRo<=-90||
		m_zRo>=90)   //胳膊转动范围
		frenqu=-frenqu;

	//InvalidateRect(NULL,FALSE);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}
