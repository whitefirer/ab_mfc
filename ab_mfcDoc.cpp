// ab_mfcDoc.cpp : Cab_mfcDoc ���ʵ��
//

#include "stdafx.h"
#include "ab_mfc.h"

#include "ab_mfcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cab_mfcDoc

IMPLEMENT_DYNCREATE(Cab_mfcDoc, CDocument)

BEGIN_MESSAGE_MAP(Cab_mfcDoc, CDocument)
END_MESSAGE_MAP()


// Cab_mfcDoc ����/����

Cab_mfcDoc::Cab_mfcDoc()
{
	// TODO: �ڴ����һ���Թ������

}

Cab_mfcDoc::~Cab_mfcDoc()
{
}

BOOL Cab_mfcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// Cab_mfcDoc ���л�

void Cab_mfcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// Cab_mfcDoc ���

#ifdef _DEBUG
void Cab_mfcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cab_mfcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cab_mfcDoc ����
