#if !defined(AFX_DIALOGTRIGGEREDSTRINGPROPERTIES_H__C47382E7_6E35_4BB8_A9AB_8DC2B4C63078__INCLUDED_)
#define AFX_DIALOGTRIGGEREDSTRINGPROPERTIES_H__C47382E7_6E35_4BB8_A9AB_8DC2B4C63078__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTriggeredStringProperties.h : header file
//
#include "CTriggeredString.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogTriggeredStringProperties dialog

class CDialogTriggeredStringProperties : public CDialog
{
// Поля
public:
	CTriggeredString* pTSCur;

// Construction
public:
	CDialogTriggeredStringProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogTriggeredStringProperties)
	enum { IDD = IDD_DIALOG_TRIGGERED_STRING_PROPERTIES };
	CComboBox	m_cbbDefaultValue;
	CButton	m_btnOK;
	CEdit	m_edtTSVarName;
	CButton	m_btnAddCondition;
	CButton	m_btnDeleteCondition;
	CButton	m_btnEditCondition;
	CListCtrl	m_lstConditions;
	CString	m_strTSVarName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTriggeredStringProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Поля
protected:
	CImageList m_ImListTriggeredStringProperties;
	CImageList* m_pImListTriggeredStringPropertiesOld;
	int m_iIconPos;
	CString m_strInitialTSVarName;

// Методы
protected:
	VOID RefreshLVContent();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTriggeredStringProperties)
	afx_msg void OnAddCondition();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonEditCondition();
	afx_msg void OnItemchangedListConditions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteCondition();
	afx_msg void OnKeydownListConditions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListConditions(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTRIGGEREDSTRINGPROPERTIES_H__C47382E7_6E35_4BB8_A9AB_8DC2B4C63078__INCLUDED_)
