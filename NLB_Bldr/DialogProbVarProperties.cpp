// DialogProbVarProperties.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "MainFrm.h"
#include "NLB_BldrDoc.h"
#include "funcs.h"
#include "DialogProbVarProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogProbVarProperties dialog


CDialogProbVarProperties::CDialogProbVarProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProbVarProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogProbVarProperties)
	m_strProbVarName = _T("");
	m_iDiceK = 0;
	m_iDiceY = 0;
	//}}AFX_DATA_INIT

	// Значения по умолчанию
	m_iDiceK = 1;	
	m_iDiceY = 1;
	m_strInitialVarName = _T("");	// На самом деле изменение произойдет на OnInitDialog
}


void CDialogProbVarProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogProbVarProperties)
	DDX_Control(pDX, IDC_EDIT_PROBVAR_NAME, m_edtProbVarName);
	DDX_Text(pDX, IDC_EDIT_PROBVAR_NAME, m_strProbVarName);
	DDX_Text(pDX, IDC_EDIT_DICE_K, m_iDiceK);
	DDV_MinMaxInt(pDX, m_iDiceK, 1, 2147483647);
	DDX_Text(pDX, IDC_EDIT_DICE_Y, m_iDiceY);
	DDV_MinMaxInt(pDX, m_iDiceY, 1, 2147483647);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogProbVarProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogProbVarProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogProbVarProperties message handlers

void CDialogProbVarProperties::OnOK() 
{
	BOOL fOk = true;	// Будем оптимистами!
	BOOL fVarChanged;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CNLB_BldrDoc* pDoc;
						
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();	
		ASSERT_VALID(pDoc);		

		if (pDoc)
		{
			UpdateData(TRUE);
			fVarChanged = (m_strInitialVarName != m_strProbVarName);
			
			CString strErrorMessage;
			CString strErrorCaption;

			if (((pDoc->FindPageIndexByVar(m_strProbVarName) != -1) ||
				(pDoc->FindBookVarPositionByName(m_strProbVarName)) ||
				(pDoc->FindProbVarPositionByName(m_strProbVarName)) ||
				(pDoc->FindStrVarPositionByName(m_strProbVarName)) ||
				(pDoc->FindTSVarPositionByName(m_strProbVarName))) && (fVarChanged))
			{
				fOk = false;	
				strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
				strErrorMessage.Format(IDS_ERROR_VAR_ALREADY_EXISTS, m_strProbVarName);
				MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
				m_edtProbVarName.SetSel(0, -1);	// Выделить весь текст в Edit
				m_edtProbVarName.SetFocus();
			}

			if (!CheckVarCorrect(m_strProbVarName))
			{
				fOk = false;	
				strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
				strErrorMessage.Format(IDS_ERROR_VAR_INCORRECT, m_strProbVarName);
				MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
				m_edtProbVarName.SetSel(0, -1);	// Выделить весь текст в Edit
				m_edtProbVarName.SetFocus();
			}
			
			if (fOk) 
			{
				m_strProbVarName.TrimLeft((WCHAR) ' ');	// Убрать пробелы слева от строки
				m_strProbVarName.TrimRight((WCHAR) ' ');	// Убрать пробелы справа от строки
				UpdateData(FALSE);	// Сохранить подрезанную строку				
				pDoc->SetModifiedFlag(TRUE);
				CDialog::OnOK();				
			}			
		}
	}
}

BOOL CDialogProbVarProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_strInitialVarName = m_strProbVarName;
	
	m_edtProbVarName.SetSel(0, -1);	// Выделить весь текст в Edit
	m_edtProbVarName.SetFocus();
	return FALSE;

	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
