#if !defined(AFX_DIALOGPROBVARPROPERTIES_H__E6319854_3366_482F_8ACA_E999C6416311__INCLUDED_)
#define AFX_DIALOGPROBVARPROPERTIES_H__E6319854_3366_482F_8ACA_E999C6416311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogProbVarProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogProbVarProperties dialog

class CDialogProbVarProperties : public CDialog
{
// Construction
public:
	CDialogProbVarProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogProbVarProperties)
	enum { IDD = IDD_DIALOG_PROBVAR_PROPERTIES };
	CEdit	m_edtProbVarName;
	CString	m_strProbVarName;
	int		m_iDiceK;
	int		m_iDiceY;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogProbVarProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CString m_strInitialVarName;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogProbVarProperties)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPROBVARPROPERTIES_H__E6319854_3366_482F_8ACA_E999C6416311__INCLUDED_)
