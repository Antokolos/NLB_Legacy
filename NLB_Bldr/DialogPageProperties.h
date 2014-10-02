#if !defined(AFX_DIALOGPAGEPROPERTIES_H__E970E9EE_89AA_4B46_B1F2_5DACA224258C__INCLUDED_)
#define AFX_DIALOGPAGEPROPERTIES_H__E970E9EE_89AA_4B46_B1F2_5DACA224258C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPageProperties.h : header file
//

#include "CColorSelectButton.h"	// Моя owner-draw кнопка

/////////////////////////////////////////////////////////////////////////////
// CDialogPageProperties dialog

class CDialogPageProperties : public CDialog
{
protected:
	BOOL RemoveLinkFromListLinksFrom(CLink* pLinkToDelete);

// Поля
public:
	LOGBRUSH m_lBrush;
	LOGPEN m_lPen;
	COLORREF m_txtColor;
	CList<S_PLink_LCounter_And_LStrings*, S_PLink_LCounter_And_LStrings*&> m_ListLinksFrom;	// Список ссылок, идущих от данной страницы и дополнительной информации о них

	// Переменные для массовых операций
	BOOL m_fAbortMassOperationFlag;	// Если эта переменная равна TRUE, то кнопки Abort mass operation нет. Она становится равной TRUE при нажатии на эту кнопку. Это означает, что массовую операцию обработки страниц надо прервать.
	int m_iPagesProcessed;		// Устанавливает вызывающая процедура, нужно для прогрессбара
	int m_iPagesTotal;			// Устанавливает вызывающая процедура, нужно для прогрессбара
	BOOL m_fGoPrevPage;	// Если	FALSE, переходим к следующей странице. Иначе - к предыдущей

// Construction
public:
	CDialogPageProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogPageProperties)
	enum { IDD = IDD_DIALOG_PAGE_PROPERTIES };
	CStatic	m_stcPercComplete;
	CProgressCtrl	m_ProgressMassOperation;
	CButton	m_btnPrevPage;
	CButton	m_btnNextPage;
	CButton	m_btnAbortMassOperation;
	CButton	m_btnMoveLinkUp;
	CButton	m_btnMoveLinkDown;
	CButton	m_btnDeleteLink;
	CButton	m_btnEditLink;
	CListCtrl	m_lstLinkNumbers;
	CEdit	m_edtPageCaption;
	CEdit	m_edtPageVar;
	CColorSelectButton	m_btnTxtClr;
	CColorSelectButton	m_btnPageClr;
	CColorSelectButton	m_btnBrdrClr;
	CString	m_strPageCaption;
	CString	m_strPageText;
	CString	m_strPageVar;
	CString	m_strPercComplete;
	BOOL	m_fUsePageCaption;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPageProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CWnd* pParentView;
	CString m_strInitialVarName;
	CImageList m_ImListLinkNumbers;
	CImageList* m_pImListLinkNumbersOld;
	int m_iIconPos;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogPageProperties)
	afx_msg void OnButtPageClr();
	afx_msg void OnButtBrdrClr();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtTxtClr();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedListLinkNumbers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListLinkNumbers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonEdit();
	afx_msg void OnGetdispinfoListLinkNumbers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonMoveUp();
	afx_msg void OnButtonMoveDown();
	virtual void OnCancel();
	afx_msg void OnButtonRevertOrder();
	afx_msg void OnButtonAbortMassOperation();
	afx_msg void OnButtonNextPage();
	afx_msg void OnButtonPrevPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPAGEPROPERTIES_H__E970E9EE_89AA_4B46_B1F2_5DACA224258C__INCLUDED_)
