#if !defined(AFX_DIALOGBOOKVARPROPERTIES_H__56D13C3B_A72F_41C8_AF83_0EB01D0EBED0__INCLUDED_)
#define AFX_DIALOGBOOKVARPROPERTIES_H__56D13C3B_A72F_41C8_AF83_0EB01D0EBED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogBookVarProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogBookVarProperties dialog

class CDialogBookVarProperties : public CDialog
{
// Construction
public:
	CDialogBookVarProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogBookVarProperties)
	enum { IDD = IDD_DIALOG_BOOKVAR_PROPERTIES };
	CButton	m_btnIsVarGlobal;
	CStatic	m_stcInitVal;
	CStatic	m_stcBookVarBody;
	CEdit	m_edtInitVal;
	CEdit	m_edtBookVarBody;
	CEdit	m_edtBookVarName;
	CString	m_strBookVarBody;
	CString	m_strBookVarName;
	int		m_iInitVal;
	BOOL	m_fIsVarGlobal;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBookVarProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:	
	CString m_strInitialVarName;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogBookVarProperties)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckGlobal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBOOKVARPROPERTIES_H__56D13C3B_A72F_41C8_AF83_0EB01D0EBED0__INCLUDED_)
