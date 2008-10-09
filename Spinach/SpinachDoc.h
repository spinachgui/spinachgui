// SpinachDoc.h : interface of the CSpinachDoc class
//


#pragma once


class CSpinachDoc : public CDocument
{
protected: // create from serialization only
	CSpinachDoc();
	DECLARE_DYNCREATE(CSpinachDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSpinachDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


