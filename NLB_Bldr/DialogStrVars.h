#if !defined(AFX_DIALOGSTRVARS_H__23FF8221_3B2E_4791_BB59_4896AA6AC9B7__INCLUDED_)
#define AFX_DIALOGSTRVARS_H__23FF8221_3B2E_4791_BB59_4896AA6AC9B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogStrVars.h : header file
//

#include <afxtempl.h>
#include "CStringVar.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogStrVars dialog

class CDialogStrVars : public CDialog
{
// Construction
public:
	CDialogStrVars(CWnd* pParent = NULL);   // standard constructor	

// Dialog Data
	//{{AFX_DATA(CDialogStrVars)
	enum { IDD = IDD_DIALOG_STRVARS };
	CButton	m_btnDeleteStrVar;
	CButton	m_btnEditStrVar;
	CListCtrl	m_lstStrVars;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogStrVars)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Поля
protected:
	CImageList m_ImListStrVars;
	CImageList* m_pImListStrVarsOld;
	int m_iIconPos;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogStrVars)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonAddStrvar();
	afx_msg void OnButtonDeleteStrvar();
	afx_msg void OnButtonEditStrvar();
	afx_msg void OnItemchangedListStrvars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListStrvars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListStrvars(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSTRVARS_H__23FF8221_3B2E_4791_BB59_4896AA6AC9B7__INCLUDED_)
