#if !defined(AFX_DIALOGSTRVARPROPERTIES_H__3E0F3F5A_D061_4ACC_ABE5_3C6C3D035BD9__INCLUDED_)
#define AFX_DIALOGSTRVARPROPERTIES_H__3E0F3F5A_D061_4ACC_ABE5_3C6C3D035BD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogStrVarProperties.h : header file
//

#include <afxtempl.h>
#include "CStringVar.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogStrVarProperties dialog

class CDialogStrVarProperties : public CDialog
{
protected:
	BOOL RefreshVarsList();	
	VOID RemoveAbsentVarsAndFillListBoxParameters();
	VOID RemoveAbsentVarsAndFillListBoxDependencies();

public:
	CList<CString, CString&> m_lstStrVarParameters;
	CList<CString, CString&> m_lstStrVarDependencies;

// Construction
public:
	CDialogStrVarProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogStrVarProperties)
	enum { IDD = IDD_DIALOG_STRVAR_PROPERTIES };
	CListBox	m_lstbxStrVarDependencies;
	CButton	m_chkSaveInTheBookmarksFile;
	CListBox	m_lstbxStrVarParams;
	CButton	m_btnMoveUp;
	CButton	m_btnMoveDown;
	CEdit	m_edtStrVarName;
	CString	m_strStrVarBody;
	CString	m_strStrVarName;
	BOOL	m_fSaveInTheBookmarksFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogStrVarProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CString m_strInitialVarName;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogStrVarProperties)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRefreshList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSTRVARPROPERTIES_H__3E0F3F5A_D061_4ACC_ABE5_3C6C3D035BD9__INCLUDED_)
