// DialogBookVarProperties.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "NLB_BldrDoc.h"
#include "NLB_BldrView.h"
#include "DialogBookVarProperties.h"
#include "MainFrm.h"
#include "funcs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogBookVarProperties dialog


CDialogBookVarProperties::CDialogBookVarProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBookVarProperties::IDD, pParent)
{	
	//{{AFX_DATA_INIT(CDialogBookVarProperties)
	m_strBookVarBody = _T("");
	m_strBookVarName = _T("");
	m_iInitVal = 0;
	m_fIsVarGlobal = FALSE;
	//}}AFX_DATA_INIT

	m_strInitialVarName = _T("");	// На самом деле изменение произойдет на OnInitDialog
}


void CDialogBookVarProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBookVarProperties)
	DDX_Control(pDX, IDC_CHECK_GLOBAL, m_btnIsVarGlobal);
	DDX_Control(pDX, IDC_STATIC_INITVAL, m_stcInitVal);
	DDX_Control(pDX, IDC_STATIC_BOOKVAR_BODY, m_stcBookVarBody);
	DDX_Control(pDX, IDC_EDIT_INITVAL, m_edtInitVal);
	DDX_Control(pDX, IDC_EDIT_BOOKVAR_BODY, m_edtBookVarBody);
	DDX_Control(pDX, IDC_EDIT_BOOKVAR_NAME, m_edtBookVarName);
	DDX_Text(pDX, IDC_EDIT_BOOKVAR_BODY, m_strBookVarBody);
	DDX_Text(pDX, IDC_EDIT_BOOKVAR_NAME, m_strBookVarName);
	DDX_Text(pDX, IDC_EDIT_INITVAL, m_iInitVal);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_fIsVarGlobal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBookVarProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogBookVarProperties)
	ON_BN_CLICKED(IDC_CHECK_GLOBAL, OnCheckGlobal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBookVarProperties message handlers

void CDialogBookVarProperties::OnOK() 
{
	CString strErrorMessage;
	CString strErrorCaption;
	BOOL fOk = true;	// Будем оптимистами!
	BOOL fVarChanged;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrDoc* pDoc;	
	TCHAR* lpBuffer;
	
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);		
		UpdateData(TRUE);
		fVarChanged = (m_strInitialVarName != m_strBookVarName);

		if (((pDoc->FindPageIndexByVar(m_strBookVarName) != -1) ||
			(pDoc->FindBookVarPositionByName(m_strBookVarName)) ||
			(pDoc->FindProbVarPositionByName(m_strBookVarName)) ||
			(pDoc->FindStrVarPositionByName(m_strBookVarName)) ||
			(pDoc->FindTSVarPositionByName(m_strBookVarName))) && (fVarChanged))
		{
			fOk = false;	
			strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
			strErrorMessage.Format(IDS_ERROR_VAR_ALREADY_EXISTS, m_strBookVarName);
			MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
			m_edtBookVarName.SetSel(0, -1);	// Выделить весь текст в Edit
			m_edtBookVarName.SetFocus();
		}

		if (!CheckVarCorrect(m_strBookVarName))
		{
			fOk = false;	
			strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
			strErrorMessage.Format(IDS_ERROR_VAR_INCORRECT, m_strBookVarName);
			MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
			m_edtBookVarName.SetSel(0, -1);	// Выделить весь текст в Edit
			m_edtBookVarName.SetFocus();
		}		
		
		if (fOk) 
		{
			m_strBookVarName.TrimLeft((WCHAR) ' ');	// Убрать пробелы слева от строки
			m_strBookVarName.TrimRight((WCHAR) ' ');	// Убрать пробелы справа от строки
			UpdateData(FALSE);	// Сохранить подрезанную строку

			if (m_fIsVarGlobal)
			{
				lpBuffer = m_strBookVarBody.GetBuffer(MAX_LOADSTRING);
				_itot(m_iInitVal, lpBuffer, 10);
				m_strBookVarBody.ReleaseBuffer();
				UpdateData(FALSE);	// Сохранить строку
				pDoc->SetModifiedFlag(TRUE);
				CDialog::OnOK();
			}
			else
			{
				CList<CBookVar*, CBookVar*&> lstBookVarsForCurPage;	// Пустой, он тут не используется

				if (m_strBookVarBody.GetLength() == 0)	// Если в результате получилась пустая строка - это ошибка!
				{			
					strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
					strErrorMessage.LoadString(IDS_BOOKVAR_BODY_CANNOT_BE_EMPTY);
					MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);			
				}
				else if (CompileBooleanFormulaAndDisplayResults(m_strBookVarBody))
				{
					E_ILType ilResult = CheckLinkContainInfiniteLoop(m_strBookVarBody, lstBookVarsForCurPage);
					if ((ilResult == E_IL_OK) ||	// Это имеет место, если формула верна и бесконечных циклов нет
						(ilResult == E_IL_COMPERR))	// Это имеет место, если формула неверна, но пользователь все равно выбрал сохранить ее
					{
						pDoc->SetModifiedFlag(TRUE);
						CDialog::OnOK();
					}
					else if (ilResult == E_IL_ILEXISTS)
					{
						strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
						strErrorMessage.LoadString(IDS_FORMULA_CONTAINS_INFINITE_LOOP);
						if (MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
						{
							pDoc->SetModifiedFlag(TRUE);
							CDialog::OnOK();
						}
					}
					else
					{
						strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
						strErrorMessage.LoadString(IDS_ERROR_CALLING_CHECK_LINK_CONTAINS_IL);
						MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);							
					}
				}
			}
		}
	}
}

BOOL CDialogBookVarProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_strInitialVarName	= m_strBookVarName;
	if (m_fIsVarGlobal)
	{
		m_iInitVal = _ttoi(m_strBookVarBody);	
		m_edtBookVarBody.ShowWindow(SW_HIDE);
		m_stcBookVarBody.ShowWindow(SW_HIDE);
		m_edtInitVal.ShowWindow(SW_SHOW);
		m_stcInitVal.ShowWindow(SW_SHOW);
	}
	else
	{
		m_iInitVal = 0;	
		m_edtBookVarBody.ShowWindow(SW_SHOW);
		m_stcBookVarBody.ShowWindow(SW_SHOW);
		m_edtInitVal.ShowWindow(SW_HIDE);
		m_stcInitVal.ShowWindow(SW_HIDE);
	}
	UpdateData(FALSE);

	m_edtBookVarName.SetSel(0, -1);	// Выделить весь текст в Edit
	m_edtBookVarName.SetFocus();
	return FALSE;

	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogBookVarProperties::OnCheckGlobal() 
{	
	if (m_btnIsVarGlobal.GetCheck() == 1)
	{
		m_edtBookVarBody.ShowWindow(SW_HIDE);
		m_stcBookVarBody.ShowWindow(SW_HIDE);
		m_edtInitVal.ShowWindow(SW_SHOW);
		m_stcInitVal.ShowWindow(SW_SHOW);
	}
	else
	{
		m_edtBookVarBody.ShowWindow(SW_SHOW);
		m_stcBookVarBody.ShowWindow(SW_SHOW);
		m_edtInitVal.ShowWindow(SW_HIDE);
		m_stcInitVal.ShowWindow(SW_HIDE);
	}
}
