// SpinachView.cpp : implementation of the CSpinachView class
//

#include "stdafx.h"
#include "Spinach.h"

#include "SpinachDoc.h"
#include "SpinachView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpinachView

IMPLEMENT_DYNCREATE(CSpinachView, CView)

BEGIN_MESSAGE_MAP(CSpinachView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSpinachView construction/destruction

CSpinachView::CSpinachView()
{
	// TODO: add construction code here

}

CSpinachView::~CSpinachView()
{
}

BOOL CSpinachView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSpinachView drawing

void CSpinachView::OnDraw(CDC* /*pDC*/)
{
	CSpinachDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSpinachView printing

BOOL CSpinachView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSpinachView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSpinachView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CSpinachView diagnostics

#ifdef _DEBUG
void CSpinachView::AssertValid() const
{
	CView::AssertValid();
}

void CSpinachView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSpinachDoc* CSpinachView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpinachDoc)));
	return (CSpinachDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpinachView message handlers
