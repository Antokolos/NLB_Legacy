#if !defined(AFX_DIALOGLINKPROPERTIES_H__17E9EDCD_9309_4E78_BF3C_0091EF20B8BD__INCLUDED_)
#define AFX_DIALOGLINKPROPERTIES_H__17E9EDCD_9309_4E78_BF3C_0091EF20B8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogLinkProperties.h : header file
//

#include "Resource.h"
#include "CColorSelectButton.h"	// Моя owner-draw кнопка
#include "CModificationsList.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogLinkProperties dialog

class CDialogLinkProperties : public CDialog
{
public:
	CModificationsList m_lstModificationsObj;

// Construction
public:
	CDialogLinkProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogLinkProperties)
	enum { IDD = IDD_DIALOG_LINK_PROPERTIES };
	CListCtrl	m_lstModifications;
	CButton	m_btnEdit;
	CButton	m_btnDelete;
	CButton	m_btnAdd;
	CEdit	m_edtLnkCaption;
	CColorSelectButton	m_btnSetLinkColor;
	CString	m_strLinkCaption;
	CString	m_strLinkConstraints;
	//}}AFX_DATA

// Поля
protected:	
	CImageList m_ImListModifications;
	CImageList* m_pImListModificationsOld;
	int m_iIconPos;
// Поля
public:	
	LOGPEN m_lPen;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogLinkProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogLinkProperties)
	afx_msg void OnButtonSetLinkColor();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedListModifications(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnDblclkListModifications(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGLINKPROPERTIES_H__17E9EDCD_9309_4E78_BF3C_0091EF20B8BD__INCLUDED_)
