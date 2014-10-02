#if !defined(AFX_DIALOGFINDTXT_H__BC009C1D_C055_4AC7_9D33_7DB1738AB21D__INCLUDED_)
#define AFX_DIALOGFINDTXT_H__BC009C1D_C055_4AC7_9D33_7DB1738AB21D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogFindTxt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogFindTxt dialog

class CDialogFindTxt : public CDialog
{
// Construction
public:
	CDialogFindTxt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogFindTxt)
	enum { IDD = IDD_DIALOG_FIND_TXT };
	BOOL	m_fFindInBookVars;
	BOOL	m_fFindInLnks;
	BOOL	m_fFindInPages;
	BOOL	m_fFindInProbVars;
	CString	m_strToFind;
	BOOL	m_fCaseSensitive;
	BOOL	m_fWholeWords;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogFindTxt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogFindTxt)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGFINDTXT_H__BC009C1D_C055_4AC7_9D33_7DB1738AB21D__INCLUDED_)
