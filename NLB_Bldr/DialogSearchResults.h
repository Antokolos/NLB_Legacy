#if !defined(AFX_DIALOGSEARCHRESULTS_H__8CC0406F_FE95_4CF6_A838_E931D3DA63E0__INCLUDED_)
#define AFX_DIALOGSEARCHRESULTS_H__8CC0406F_FE95_4CF6_A838_E931D3DA63E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSearchResults.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDialogSearchResults dialog

class CDialogSearchResults : public CDialog
{
// Construction
public:
	CDialogSearchResults(CWnd* pParent = NULL);   // standard constructor
	void OnOkDialogSearchResults() {OnOK(); }

// Dialog Data
	//{{AFX_DATA(CDialogSearchResults)
	enum { IDD = IDD_DIALOG_SEARCH_RESULTS };
	CListCtrl	m_lstResults;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSearchResults)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Поля
public:
	int m_iIconPosPage;
	int m_iIconPosLink;
	int m_iIconPosBookVar;
	int m_iIconPosProbVar;

// Поля
protected:
	CWnd* m_pParent;
	CImageList m_ImListSRes;
	CImageList* m_pImListSResOld;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSearchResults)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListResults(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSEARCHRESULTS_H__8CC0406F_FE95_4CF6_A838_E931D3DA63E0__INCLUDED_)
