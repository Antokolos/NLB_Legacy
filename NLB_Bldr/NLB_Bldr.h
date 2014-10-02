// NLB_Bldr.h : main header file for the NLB_BLDR application
//

#if !defined(AFX_NLB_BLDR_H__8DC3FA2A_7730_458F_97C9_500D68F47716__INCLUDED_)
#define AFX_NLB_BLDR_H__8DC3FA2A_7730_458F_97C9_500D68F47716__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrApp:
// See NLB_Bldr.cpp for the implementation of this class
//

class CNLB_BldrApp : public CWinApp
{
public:
	CNLB_BldrApp();

public:
	virtual void HandleError(int iErrCode);

public:
	UINT m_uiNLBClipFormat;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLB_BldrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNLB_BldrApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLB_BLDR_H__8DC3FA2A_7730_458F_97C9_500D68F47716__INCLUDED_)
