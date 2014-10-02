// NLB_BldrDoc.h : interface of the CNLB_BldrDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NLB_BLDRDOC_H__4FA97958_C036_4289_A298_4B07942C3BAE__INCLUDED_)
#define AFX_NLB_BLDRDOC_H__4FA97958_C036_4289_A298_4B07942C3BAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "CBookPage.h"
#include "CBookVar.h"
#include "CStringVar.h"
#include "CTriggeredString.h"
#include "CProbVar.h"
#include "CLink.h"
#include "CModificationsList.h"
#include "consts.h"

#define SZ_NLB_SIGNATURE "NLB_B66F485C_A43B_4e65_8D07_5CE706EB5EBC"
const int iSignatureLen = 41;	// Длина вышеуказанной строчки с учетом завершающего нуля

struct S_NLB_Hdr
{
	char* szSignature;
	DWORD dwNLB_Ver;	// Номер версии программы
	DWORD dwReserved1;
	DWORD dwReserved2;
	DWORD dwReserved3;
	DWORD dwReserved4;
	DWORD dwReserved5;
};

class CNLB_BldrDoc : public CDocument
{
protected: // create from serialization only
	CNLB_BldrDoc();
	DECLARE_DYNCREATE(CNLB_BldrDoc)

// Attributes
public:
	// CList is a template class that takes two template arguments.
	// The first argument is type stored internally by the list, the
	// second argument is the type used in the arguments for the
	// CList methods.
	CList<CBookPage*, CBookPage*&> m_lstPages;
	CBookPage* m_pStartPoint;	
	CList<CLink*, CLink*&> m_lstLnks;
	CList<CBookVar*, CBookVar*&> m_lstBookVars;	
	CList<CStringVar*, CStringVar*&> m_lstStrVars;	
	CList<CTriggeredString*, CTriggeredString*&> m_lstTriggeredStrings;	
	CList<CProbVar*, CProbVar*&> m_lstProbVars;	
	int m_iSelectedPagesCount;	// Число выделенных (заселекченных) страниц

// Attributes
public:
	// В помощь сериализации...
	CList<POSITION, POSITION> m_lstPosFromChanged;
	CList<POSITION, POSITION> m_lstPosToChanged;	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLB_BldrDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNLB_BldrDoc();
	VOID RelinkPointers(const CList<CLink*, CLink*&>& lstWhere, CBookPage* pp_old, CBookPage* pp_new);
	VOID RelinkModPointers(CModificationsList* plstWhere, 
									 CList<POSITION, POSITION>* plstModifiedBookVarChanged, 
									 CList<POSITION, POSITION>* plstModificationBodyChanged, 
									 CBookVar* pv_old, 
									 CBookVar* pv_new);
	VOID RelinkTSVarBVPointers(CTriggeredString* pTSVarCur,
									 CBookVar* pv_old, 
									 CBookVar* pv_new);
	VOID RelinkTSVarSVPointers(CTriggeredString* pTSVarCur,
									 CStringVar* pv_old, 
									 CStringVar* pv_new);
	int FindPageIndexByPointer(const CBookPage* pPage) const;	
	int FindPageIndexByVar(const CString& strPageVar) const;		
	POSITION FindPagePositionByName(const CString& strPageVar) const;		
	POSITION FindBookVarPositionByName(const CString& strBookVar) const;
	int FindBookVarIndexByPointer(const CBookVar* pBVar) const;	
	POSITION FindStrVarPositionByName(const CString& strStrVar) const;
	int FindStrVarIndexByPointer(const CStringVar* pSVar) const;
	POSITION FindTSVarPositionByName(const CString& strTSVar) const;
	POSITION FindProbVarPositionByName(const CString& strProbVar) const;
	BOOL IsExistLink(const CLink& lnk);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNLB_BldrDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLB_BLDRDOC_H__4FA97958_C036_4289_A298_4B07942C3BAE__INCLUDED_)
