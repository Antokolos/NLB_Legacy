#if !defined(AFX_DIALOGTRIGGEREDSTRINGS_H__2908E5D2_7AB3_4890_8D98_A0AB338B1F49__INCLUDED_)
#define AFX_DIALOGTRIGGEREDSTRINGS_H__2908E5D2_7AB3_4890_8D98_A0AB338B1F49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTriggeredStrings.h : header file
//

#include "CTriggeredString.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogTriggeredStrings dialog

class CDialogTriggeredStrings : public CDialog
{
// Construction
public:
	CDialogTriggeredStrings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogTriggeredStrings)
	enum { IDD = IDD_DIALOG_TRIGGERED_STRINGS };
	CButton	m_btnDeleteTriggeredString;
	CButton	m_btnEditTriggeredString;
	CListCtrl	m_lstTriggeredStrings;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTriggeredStrings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Поля
protected:
	CImageList m_ImListTriggeredStrings;
	CImageList* m_pImListTriggeredStringsOld;
	int m_iIconPos;

// Методы
protected:
	VOID GenerateTSBodyToSetInLV(CTriggeredString* pTS, CString& strResult);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTriggeredStrings)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonAddTriggeredStrings();
	afx_msg void OnButtonDeleteTriggeredString();
	afx_msg void OnButtonEditTriggeredString();
	afx_msg void OnItemchangedListTriggeredStrings(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListTriggeredStrings(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListTriggeredStrings(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTRIGGEREDSTRINGS_H__2908E5D2_7AB3_4890_8D98_A0AB338B1F49__INCLUDED_)
