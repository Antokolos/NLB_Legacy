#if !defined(AFX_DIALOGCOMPERR_H__981F23EA_341A_48EF_83D7_0ECD6BA9B263__INCLUDED_)
#define AFX_DIALOGCOMPERR_H__981F23EA_341A_48EF_83D7_0ECD6BA9B263__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogComperr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogComperr dialog

class CDialogComperr : public CDialog
{
// Construction
public:
	CDialogComperr(CWnd* pParent = NULL);   // standard constructor
	void OnOkDialogComperr() {OnOK(); }

// Dialog Data
	//{{AFX_DATA(CDialogComperr)
	enum { IDD = IDD_DIALOG_COMPERR };
	CListCtrl	m_lstCompErr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogComperr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Поля
public:
	int m_iIconPosLink;
	int m_iIconPosBookVar;

// Поля
protected:
	CWnd* m_pParent;
	CImageList m_ImListComperr;
	CImageList* m_pImListComperrOld;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogComperr)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListComperr(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCOMPERR_H__981F23EA_341A_48EF_83D7_0ECD6BA9B263__INCLUDED_)
