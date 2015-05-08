// ab_mfcDoc.cpp : Cab_mfcDoc 类的实现
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


// Cab_mfcDoc 构造/析构

Cab_mfcDoc::Cab_mfcDoc()
{
	// TODO: 在此添加一次性构造代码

}

Cab_mfcDoc::~Cab_mfcDoc()
{
}

BOOL Cab_mfcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Cab_mfcDoc 序列化

void Cab_mfcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// Cab_mfcDoc 诊断

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


// Cab_mfcDoc 命令
