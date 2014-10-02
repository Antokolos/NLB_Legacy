#if !defined(AFX_DIALOGMODIFICATION_H__DA186323_C357_41D7_8E6A_417F083161BA__INCLUDED_)
#define AFX_DIALOGMODIFICATION_H__DA186323_C357_41D7_8E6A_417F083161BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogModification.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogModification dialog

#include "types.h"

class CDialogModification : public CDialog
{
public:
	S_BookVarModification m_bvm;

// Construction
public:
	CDialogModification(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogModification)
	enum { IDD = IDD_DIALOG_MODIFICATION };
	CComboBox	m_cbbVarName;
	CComboBox	m_cbbModification;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogModification)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogModification)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGMODIFICATION_H__DA186323_C357_41D7_8E6A_417F083161BA__INCLUDED_)
