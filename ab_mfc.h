// ab_mfc.h : ab_mfc Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Cab_mfcApp:
// �йش����ʵ�֣������ ab_mfc.cpp
//

class Cab_mfcApp : public CWinApp
{
public:
	Cab_mfcApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cab_mfcApp theApp;