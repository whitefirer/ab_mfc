// ab_mfcDoc.h : Cab_mfcDoc ��Ľӿ�
//


#pragma once


class Cab_mfcDoc : public CDocument
{
protected: // �������л�����
	Cab_mfcDoc();
	DECLARE_DYNCREATE(Cab_mfcDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~Cab_mfcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


