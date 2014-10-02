#if !defined(AFX_DIALOGLEAFS_H__DD875DB2_E466_4542_8F29_BFC08BE3AE10__INCLUDED_)
#define AFX_DIALOGLEAFS_H__DD875DB2_E466_4542_8F29_BFC08BE3AE10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogLeafs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogLeafs dialog

class CDialogLeafs : public CDialog
{
// Construction
public:
	CDialogLeafs(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogLeafs)
	enum { IDD = IDD_DIALOG_LEAFS };
	CButton	m_btnGoTo;
	CListCtrl	m_lstLeafs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogLeafs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Поля
protected:
	CImageList m_ImListLeafs;
	CImageList* m_pImListLeafsOld;
	int m_iIconPos;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogLeafs)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedListLeafs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonGoto();
	afx_msg void OnDblclkListLeafs(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGLEAFS_H__DD875DB2_E466_4542_8F29_BFC08BE3AE10__INCLUDED_)
