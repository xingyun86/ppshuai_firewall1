
// FireWall1Doc.cpp : implementation of the CFireWall1Doc class
//

#include "stdafx.h"
#include "FireWall1.h"

#include "FireWall1Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFireWall1Doc

IMPLEMENT_DYNCREATE(CFireWall1Doc, CDocument)

BEGIN_MESSAGE_MAP(CFireWall1Doc, CDocument)
	//{{AFX_MSG_MAP(CFireWall1Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFireWall1Doc construction/destruction

CFireWall1Doc::CFireWall1Doc()
{
	// TODO: add one-time construction code here

}

CFireWall1Doc::~CFireWall1Doc()
{
}

BOOL CFireWall1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFireWall1Doc serialization

void CFireWall1Doc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CFireWall1Doc diagnostics

#ifdef _DEBUG
void CFireWall1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFireWall1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFireWall1Doc commands
