// SpinachView.h : interface of the CSpinachView class
//


#pragma once


class CSpinachView : public CView
{
protected: // create from serialization only
	CSpinachView();
	DECLARE_DYNCREATE(CSpinachView)

// Attributes
public:
	CSpinachDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSpinachView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SpinachView.cpp
inline CSpinachDoc* CSpinachView::GetDocument() const
   { return reinterpret_cast<CSpinachDoc*>(m_pDocument); }
#endif

