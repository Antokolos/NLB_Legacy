#if !defined(AFX_DIALOGTRIGGEREDSTRINGCONDITIONPROPERTIES_H__3E79C901_5DB4_44B8_922A_3191CD19CFDE__INCLUDED_)
#define AFX_DIALOGTRIGGEREDSTRINGCONDITIONPROPERTIES_H__3E79C901_5DB4_44B8_922A_3191CD19CFDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTriggeredStringConditionProperties.h : header file
//

#include "CTriggeredString.h"
#include "types.h"

const int IRETCODE_ALL_BOOKVARS_IS_USED = 7;

struct S_TS_Conditions
{
	S_TS_ConditionPair ConditionPairCur;
};

/////////////////////////////////////////////////////////////////////////////
// CDialogTriggeredStringConditionProperties dialog

class CDialogTriggeredStringConditionProperties : public CDialog
{
// Construction
public:
	CDialogTriggeredStringConditionProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogTriggeredStringConditionProperties)
	enum { IDD = IDD_DIALOG_TRIGGERED_STRING_CONDITION_PROPERTIES };
	CComboBox	m_cbbValue;
	CComboBox	m_cbbCondition;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTriggeredStringConditionProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// ѕол€
public:
		S_TS_Conditions m_tsc;
		HWND m_hwndlstConditionsParent;
		BOOL m_fNeedToDisableAddButton;
		BOOL m_fCloseImmediately;	// ≈сли TRUE, диалог закрываетс€ сразу после выполнени€ InitDialog. Ќужно дл€ проверки на m_fNeedToDisableAddButton при первом открытии диалога TriggeredStringCondition (если там уже что-то есть, может понадобитьс€ сразу же задизаблить кнопку Add)

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTriggeredStringConditionProperties)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTRIGGEREDSTRINGCONDITIONPROPERTIES_H__3E79C901_5DB4_44B8_922A_3191CD19CFDE__INCLUDED_)
