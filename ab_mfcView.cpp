// ab_mfcView.cpp : Cab_mfcView ���ʵ��
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
	// ��׼��ӡ����
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

// Cab_mfcView ����/����

Cab_mfcView::Cab_mfcView()
{
	m_size =195;
	m_xRo = 0;
	m_yRo = 0;
	m_zRo = 0;
	m_xTra=10;
	m_yTra=5;
	m_zTra=-30;
	frenqu=5;  //�첲ת��
	// TODO: �ڴ˴���ӹ������
	isConsole = FALSE;
}

Cab_mfcView::~Cab_mfcView()
{
}

BOOL Cab_mfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Cab_mfcView ����

void Cab_mfcView::OnDraw(CDC* /*pDC*/)
{
	Cab_mfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	RenderScene();  //��Ⱦ����
	::SwapBuffers(m_hDC);//����������
}


// Cab_mfcView ��ӡ

BOOL Cab_mfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Cab_mfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Cab_mfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// Cab_mfcView ���

#ifdef _DEBUG
void Cab_mfcView::AssertValid() const
{
	CView::AssertValid();
}

void Cab_mfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cab_mfcDoc* Cab_mfcView::GetDocument() const // �ǵ��԰汾��������
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
		freopen( "CONOUT$", "w+t", stdout );// ����д
		freopen( "CONIN$", "r+t", stdin );// �����
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

	// TODO:  �ڴ������ר�õĴ�������
	//��ʼ��
	HWND hWnd = GetSafeHwnd();
	m_hDC = ::GetDC(hWnd);
	//�������ڸ�ʽ
	if(SetWindowPixelFormat(m_hDC)==FALSE)
		return 0;
	//����RC
	m_hRC = wglCreateContext(m_hDC);
	if(m_hRC==NULL)
		return 0;
	if(wglMakeCurrent(m_hDC,m_hRC)==FALSE)
		return 0;
	//��ʼ��OpenGL�ĸ�������	
	glClearColor(0,0,0,1.0);

	//���ù���
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

	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
//	return CView::OnEraseBkgnd(pDC);
}

void Cab_mfcView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
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

////////////////// �������ظ�ʽ///////////////////////////
BOOL Cab_mfcView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW |//֧���ڴ����л�ͼ
		PFD_SUPPORT_OPENGL | //֧��OpenGL  
		PFD_DOUBLEBUFFER |   //˫����ģʽ
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

void Cab_mfcView::RenderScene()//��ͼ������Ҫ���������
{
// 	float b=0.0;
// 	float c=0.0;
// 	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�������  
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    //��Ȼ���   
	glLoadIdentity(); 
	glTranslated(m_xTra,m_yTra,m_zTra);
	glRotated(m_xRo, 1.0, 0.0, 0.0);
	glRotated(m_yRo, 0.0, 1.0, 0.0);

	//����
	glCallList(1);

	//�Ҹ첲
	glPushMatrix();
	glTranslated(3.0,2.5,0.0);
	glRotated(-m_zRo,0,0,1);
	glTranslated(3,0,0);
	glCallList(2);
	glPopMatrix();

	//��첲
	glPushMatrix();
	glTranslated(-3.0,2.5,0.0);
	glRotated(m_zRo,0,0,1);
	glTranslated(-3,0,0);
	glCallList(2);
	glPopMatrix();

	//ͷ
	glPushMatrix();
	glTranslated(0,6.5,0);
	glCallList(3);
	glPopMatrix();

	//��
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

	//��
	glPushMatrix();
	glTranslated(-0.6,7,1.8);
	glCallList(5);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6,7,1.8);
	glCallList(5);
	glPopMatrix();

	//��
	glPushMatrix();
	glTranslated(0,5.8,2);
	glCallList(6);
	glPopMatrix();
}

//������ʾ�б�********************************************************
void Cab_mfcView::JIQIREN()
{	//����
	::glNewList(1,GL_COMPILE_AND_EXECUTE);
	glColor3f(.0,0.0,1.0);
	auxSolidBox(6,8.5,5);
	::glEndList();

	//�첲
	::glNewList(2,GL_COMPILE_AND_EXECUTE);
	glColor3f(0.0,1.0,1.0);
	auxSolidBox(8,2,1);
	//auxSolidSphere(1.6);
	::glEndList();

	//ͷ�Ͳ���
	::glNewList(3,GL_COMPILE_AND_EXECUTE);
	glColor3f(1.0,1.0,0.0);
	auxSolidCube((GLdouble) 3);
	auxSolidBox((GLdouble) 1,(GLdouble) 0,(GLdouble) 1) ;//����
	::glEndList();

	//��
	::glNewList(4,GL_COMPILE_AND_EXECUTE);
	glColor3f(0.0,1.0,.0);
	auxSolidBox((GLdouble) 2,(GLdouble) 6,(GLdouble) 3) ;
	::glEndList();

	//��
	::glNewList(5,GL_COMPILE_AND_EXECUTE);
	glColor3f(1.0,.0,.0);
	auxSolidSphere(0.6);
	::glEndList();

	//��
	::glNewList(6,GL_COMPILE_AND_EXECUTE);
	glColor3f(1.0,0.0,1.0);
	auxSolidBox(1.5,0.6,0.2);
	::glEndList();

}
void Cab_mfcView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_zRo -=frenqu;
	if(m_zRo<=-90||
		m_zRo>=90)   //�첲ת����Χ
		frenqu=-frenqu;

	//InvalidateRect(NULL,FALSE);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}
