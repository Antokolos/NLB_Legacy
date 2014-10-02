#if !defined(AFX_DIALOGBOOKVARS_H__0BFA1130_7A48_496E_80EA_9328412BC414__INCLUDED_)
#define AFX_DIALOGBOOKVARS_H__0BFA1130_7A48_496E_80EA_9328412BC414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogBookVars.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogBookVars dialog

class CDialogBookVars : public CDialog
{
// Construction
public:
	CDialogBookVars(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogBookVars)
	enum { IDD = IDD_DIALOG_BOOKVARS };
	CButton	m_btnEditBookVar;
	CButton	m_btnDeleteBookVar;
	CListCtrl	m_lstBookVars;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBookVars)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Поля
protected:
	CImageList m_ImListBookVars;
	CImageList* m_pImListBookVarsOld;
	int m_iIconPos;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogBookVars)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonAddBookvar();
	afx_msg void OnButtonEditBookvar();
	afx_msg void OnButtonDeleteBookvar();
	afx_msg void OnItemchangedListBookvars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListBookvars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListBookvars(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBOOKVARS_H__0BFA1130_7A48_496E_80EA_9328412BC414__INCLUDED_)
