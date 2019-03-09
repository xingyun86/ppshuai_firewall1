
// FireWall1Doc.h : interface of the CFireWall1Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIREWALL1DOC_H__9D269EEB_A406_11D4_8595_00D00912B25C__INCLUDED_)
#define AFX_FIREWALL1DOC_H__9D269EEB_A406_11D4_8595_00D00912B25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFireWall1Doc : public CDocument
{
protected: // create from serialization only
	CFireWall1Doc();
	DECLARE_DYNCREATE(CFireWall1Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFireWall1Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFireWall1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFireWall1Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALL1DOC_H__9D269EEB_A406_11D4_8595_00D00912B25C__INCLUDED_)
