#if !defined(AFX_DIALOGPROBABILITY_H__3574377B_58DA_476E_BD3A_F00DBF1825C4__INCLUDED_)
#define AFX_DIALOGPROBABILITY_H__3574377B_58DA_476E_BD3A_F00DBF1825C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogProbability.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogProbability dialog

class CDialogProbability : public CDialog
{
// Construction
public:
	CDialogProbability(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogProbability)
	enum { IDD = IDD_DIALOG_PROBABILITY };
	CButton	m_btnEditProbVar;
	CButton	m_btnDeleteProbVar;
	CButton	m_btnAddProbVar;
	CListCtrl	m_lstProbVars;
	//}}AFX_DATA

// Поля
protected:
	CImageList m_ImListProbVars;
	CImageList* m_pImListProbVarsOld;
	int m_iIconPos;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogProbability)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogProbability)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonAddProbvar();
	afx_msg void OnButtonEditProbvar();
	afx_msg void OnButtonDeleteProbvar();
	afx_msg void OnItemchangedListProbabilityVars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListProbabilityVars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListProbabilityVars(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPROBABILITY_H__3574377B_58DA_476E_BD3A_F00DBF1825C4__INCLUDED_)
