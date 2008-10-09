// SpinachDoc.cpp : implementation of the CSpinachDoc class
//

#include "stdafx.h"
#include "Spinach.h"

#include "SpinachDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpinachDoc

IMPLEMENT_DYNCREATE(CSpinachDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpinachDoc, CDocument)
END_MESSAGE_MAP()


// CSpinachDoc construction/destruction

CSpinachDoc::CSpinachDoc()
{
	// TODO: add one-time construction code here

}

CSpinachDoc::~CSpinachDoc()
{
}

BOOL CSpinachDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSpinachDoc serialization

void CSpinachDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSpinachDoc diagnostics

#ifdef _DEBUG
void CSpinachDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpinachDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSpinachDoc commands
