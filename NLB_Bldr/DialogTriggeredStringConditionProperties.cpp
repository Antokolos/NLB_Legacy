// DialogTriggeredStringConditionProperties.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "NLB_BldrDoc.h"
#include "MainFrm.h"
#include "DialogTriggeredStringConditionProperties.h"
#include "CBookVar.h"
#include "CStringVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTriggeredStringConditionProperties dialog


CDialogTriggeredStringConditionProperties::CDialogTriggeredStringConditionProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTriggeredStringConditionProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTriggeredStringConditionProperties)
	//}}AFX_DATA_INIT

	m_hwndlstConditionsParent = NULL;
	m_tsc.ConditionPairCur.pBVCondition = NULL;
	m_tsc.ConditionPairCur.pStrVar = NULL;
	m_fNeedToDisableAddButton = FALSE;
	m_fCloseImmediately = FALSE;
}


void CDialogTriggeredStringConditionProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTriggeredStringConditionProperties)
	DDX_Control(pDX, IDC_COMBO_VALUE, m_cbbValue);
	DDX_Control(pDX, IDC_COMBO_CONDITION, m_cbbCondition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTriggeredStringConditionProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogTriggeredStringConditionProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTriggeredStringConditionProperties message handlers

BOOL CDialogTriggeredStringConditionProperties::OnInitDialog() 
{
	int iConditionSel = 0;
	int iValueSel = 0;
	int iConditionCount = 0;
	int iAlreadyExistingConditionCount = 0;
	int iValueCount = 0;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CNLB_BldrDoc* pDoc;
	POSITION pos;
	int iBookVarsCount;
	CBookVar* pBookVarCur;
	int iItemPos;
	int iStrVarsCount;
	CStringVar* pStrVarCur;
	int i, j;
	S_TS_ConditionPair* pstsc;
	BOOL fCanInclude;
	LVITEM lvitem;
	BOOL fVarLBoxHasItems = FALSE;	// Если в список Condition не было добавлено ни одной переменной, закрываем диалог сразу же, сказав, что облом.
	CString strTemp;

	CDialog::OnInitDialog();
	
	m_fNeedToDisableAddButton = FALSE;
	iAlreadyExistingConditionCount = ::SendMessage(m_hwndlstConditionsParent, LVM_GETITEMCOUNT, 0, 0);
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			pos = pDoc->m_lstBookVars.GetHeadPosition();
			iBookVarsCount = pDoc->m_lstBookVars.GetCount();
			for (i=0; i<iBookVarsCount; i++)
			{			
				pBookVarCur = pDoc->m_lstBookVars.GetAt(pos);				
							
				// Проверим, может быть, такое условие уже есть. Тогда не надо добавлять
				fCanInclude = TRUE;
				for (j = 0; j < iAlreadyExistingConditionCount; j++)
				{					
					lvitem.iItem = j;
					lvitem.iSubItem = 0;
					lvitem.mask = LVIF_PARAM;
					if (::SendMessage(m_hwndlstConditionsParent, LVM_GETITEM, 0, (LPARAM) &lvitem))
					{
						pstsc = (S_TS_ConditionPair*) lvitem.lParam;
						if ((pBookVarCur == pstsc->pBVCondition) && (pBookVarCur != m_tsc.ConditionPairCur.pBVCondition))
						{
							fCanInclude = FALSE;
							break;						
						}
					}
				}

				if (fCanInclude)
				{
					fVarLBoxHasItems = TRUE;
					//iItemPos = m_cbbCondition.AddString(pBookVarCur->Get_strBookVarName());
					// Вместо нее используем InsertString, так как
					// Unlike the AddString method, InsertString does not cause a list with the LBS_SORT style to be sorted.
					// Или же надо быть уверенным, что в списке не стоит галка автосортировки
					// -1 означает "добавить в конец"
					iItemPos = m_cbbCondition.InsertString(-1, pBookVarCur->Get_strBookVarName());	
					if (iItemPos >= 0) 
					{
						m_cbbCondition.SetItemDataPtr(iItemPos, pBookVarCur);
						if (pBookVarCur == m_tsc.ConditionPairCur.pBVCondition) iConditionSel = iConditionCount;					
						iConditionCount++;						
					}
				}
				
				pDoc->m_lstBookVars.GetNext(pos);
			}

			pos = pDoc->m_lstStrVars.GetHeadPosition();
			iStrVarsCount = pDoc->m_lstStrVars.GetCount();
			for (i=0; i<iStrVarsCount; i++)
			{			
				pStrVarCur = pDoc->m_lstStrVars.GetAt(pos);				
						
				if (pStrVarCur->m_lstStrVarParameters.GetCount() == 0)		// Позволяется добавлять только строки без параметров!
				{
					if (pStrVarCur->m_lstStrVarDependencies.GetCount() == 0)
						strTemp = pStrVarCur->Get_strStrVarName();
					else
						strTemp.Format(IDS_STRING_TSVAR_DEPENDENT_STRING_VALUE, pStrVarCur->Get_strStrVarName());

					//iItemPos = m_cbbValue.AddString(strTemp);
					iItemPos = m_cbbValue.InsertString(-1, strTemp);
					if (iItemPos >= 0) 
					{
						m_cbbValue.SetItemDataPtr(iItemPos, pStrVarCur);
						if (pStrVarCur == m_tsc.ConditionPairCur.pStrVar) iValueSel = iValueCount;					
						iValueCount++;						
					}
				}
				
				pDoc->m_lstStrVars.GetNext(pos);
			}
		}
	}
		
	m_cbbCondition.SetCurSel(iConditionSel);	
	m_cbbValue.SetCurSel(iValueSel);

	if ((!fVarLBoxHasItems) || m_fCloseImmediately)
	{
		int nRet = IRETCODE_ALL_BOOKVARS_IS_USED;
		EndDialog(nRet); // This value is returned by DoModal!
		// Фактически, закрытие диалога произойдет уже ПОСЛЕ выхода из этой функции, т.е. по грамотному
	}

	if (m_fCloseImmediately)
	{
		if (iConditionCount == 0) m_fNeedToDisableAddButton = TRUE;	// Этот вариант - когда открывается уже полностью заполненная переменная, т.е. нельзя добавить новый элемент. А в строчке ниже добавляемый элемент будет последним, т.е. тоже надо дизаблить Add
	}
	else
	{
		if (iConditionCount == 1) m_fNeedToDisableAddButton = TRUE;	// Если больше нельзя добавить ни одного элемента, надо запретить их дальнейшее добавление
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE		
}

void CDialogTriggeredStringConditionProperties::OnOK() 
{
	int iItemPos;

	iItemPos = m_cbbCondition.GetCurSel();
	m_tsc.ConditionPairCur.pBVCondition = (CBookVar*) m_cbbCondition.GetItemDataPtr(iItemPos);

	iItemPos = m_cbbValue.GetCurSel();
	m_tsc.ConditionPairCur.pStrVar = (CStringVar*) m_cbbValue.GetItemDataPtr(iItemPos);
	
	CDialog::OnOK();
}
