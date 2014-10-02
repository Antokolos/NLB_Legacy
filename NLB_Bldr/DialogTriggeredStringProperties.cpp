// DialogTriggeredStringProperties.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "consts.h"
#include "funcs.h"
#include "MainFrm.h"
#include "DialogTriggeredStringProperties.h"
#include "DialogTriggeredStringConditionProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTriggeredStringProperties dialog


CDialogTriggeredStringProperties::CDialogTriggeredStringProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTriggeredStringProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTriggeredStringProperties)
	m_strTSVarName = _T("");
	//}}AFX_DATA_INIT
	m_iIconPos = -1;
	pTSCur = NULL;
}


void CDialogTriggeredStringProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTriggeredStringProperties)
	DDX_Control(pDX, IDC_COMBO_DEFAULT_VALUE, m_cbbDefaultValue);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_EDIT_TSVARNAME, m_edtTSVarName);
	DDX_Control(pDX, IDC_ADD_CONDITION, m_btnAddCondition);
	DDX_Control(pDX, IDC_DELETE_CONDITION, m_btnDeleteCondition);
	DDX_Control(pDX, IDC_BUTTON_EDIT_CONDITION, m_btnEditCondition);
	DDX_Control(pDX, IDC_LIST_CONDITIONS, m_lstConditions);
	DDX_Text(pDX, IDC_EDIT_TSVARNAME, m_strTSVarName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTriggeredStringProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogTriggeredStringProperties)
	ON_BN_CLICKED(IDC_ADD_CONDITION, OnAddCondition)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_EDIT_CONDITION, OnButtonEditCondition)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CONDITIONS, OnItemchangedListConditions)
	ON_BN_CLICKED(IDC_DELETE_CONDITION, OnDeleteCondition)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_CONDITIONS, OnKeydownListConditions)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONDITIONS, OnDblclkListConditions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTriggeredStringProperties message handlers

void CDialogTriggeredStringProperties::OnAddCondition() 
{	
	CDialogTriggeredStringConditionProperties dlg;
	dlg.m_hwndlstConditionsParent = m_lstConditions.GetSafeHwnd();
	if (dlg.DoModal() == IDOK)
	{
		pTSCur->AddConditionPair(dlg.m_tsc.ConditionPairCur);
		RefreshLVContent();
		if (dlg.m_fNeedToDisableAddButton) m_btnAddCondition.EnableWindow(FALSE);
	}	
}

BOOL CDialogTriggeredStringProperties::OnInitDialog() 
{
	POSITION pos;
	int iStrVarsCount;
	int i;
	CStringVar* pStrVarCur;
	int iItem, iSelItem;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CNLB_BldrDoc* pDoc;
	CString strTmp;

	BeginWaitCursor();	

	CDialog::OnInitDialog();

	m_strInitialTSVarName = m_strTSVarName;

	HICON hIconNew = AfxGetApp()->LoadIcon(IDR_TRIGSTR);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstConditions.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_TRIGGERED_STRING_PROPERTIES_CONDITION);		
	m_lstConditions.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVTRIGSTRPROP_COL0_WIDTH,
		0);

	lpszColumnHeading.LoadString(IDS_UITXT_LV_TRIGGERED_STRING_PROPERTIES_VALUE);		
	m_lstConditions.InsertColumn(1,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVTRIGSTRPROP_COL1_WIDTH,
		0);
	
	m_ImListTriggeredStringProperties.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								1, 
								1);
	m_iIconPos = m_ImListTriggeredStringProperties.Add(hIconNew);
	m_pImListTriggeredStringPropertiesOld = m_lstConditions.SetImageList(&m_ImListTriggeredStringProperties, LVSIL_SMALL);	

	RefreshLVContent();

	m_btnEditCondition.EnableWindow(FALSE);
	m_btnDeleteCondition.EnableWindow(FALSE);

	strTmp.LoadString(IDS_STRING_TSVAR_DEFAULT_EMPTY_VALUE);
	iItem = m_cbbDefaultValue.InsertString(-1, strTmp);
	if (pTSCur->GetDefaultValue() == NULL) iSelItem = iItem;
	m_cbbDefaultValue.SetItemDataPtr(iItem, NULL);
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			pos = pDoc->m_lstStrVars.GetHeadPosition();
			iStrVarsCount = pDoc->m_lstStrVars.GetCount();
			for (i=0; i<iStrVarsCount; i++)
			{			
				pStrVarCur = pDoc->m_lstStrVars.GetNext(pos);		
				if (pStrVarCur->m_lstStrVarParameters.GetCount() == 0)	// Позволяется добавлять только строки без параметров!
				{
					if (pStrVarCur->m_lstStrVarDependencies.GetCount() == 0)
						strTmp = pStrVarCur->Get_strStrVarName();
					else
						strTmp.Format(IDS_STRING_TSVAR_DEFAULT_DEPENDENT_STRING_VALUE, pStrVarCur->Get_strStrVarName());
					iItem = m_cbbDefaultValue.InsertString(-1, strTmp);
					if (pTSCur->GetDefaultValue() == pStrVarCur) iSelItem = iItem;
					m_cbbDefaultValue.SetItemDataPtr(iItem, pStrVarCur);
				}
			}
		}
	}
	m_cbbDefaultValue.SetCurSel(iSelItem);

	// Надо ли дизаблить кнопку Add сразу, определяем неколько извращенным способом
	CDialogTriggeredStringConditionProperties dlg;
	dlg.m_fCloseImmediately = TRUE;
	dlg.m_hwndlstConditionsParent = m_lstConditions.GetSafeHwnd();
	dlg.DoModal();	// Он сразу же закроется, но там будет определено, надо ли дизаблить Add
	if (dlg.m_fNeedToDisableAddButton) m_btnAddCondition.EnableWindow(FALSE);	
	
	EndWaitCursor();
	
	m_edtTSVarName.SetSel(0, -1);	// Выделить весь текст в Edit
	m_edtTSVarName.SetFocus();
	return FALSE;

	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDialogTriggeredStringProperties::RefreshLVContent()
{
	POSITION pos;
	S_TS_ConditionPair* pTSCPCur;	
	int iTSCPCount;	
	CString strTemp;

	if (pTSCur)
	{
		m_lstConditions.DeleteAllItems();
		iTSCPCount = pTSCur->GetConditionPairsCount();
		pos = pTSCur->GetConditionPairsHeadPosition();			
		for (int i = 0; i < iTSCPCount; i++)
		{
			pTSCPCur = pTSCur->GetConditionPairAt(pos);
			m_lstConditions.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
					i,
					pTSCPCur->pBVCondition->Get_strBookVarName(),
					0,
					0,
					m_iIconPos,
					(LPARAM) pTSCPCur);		
			
			if (pTSCPCur->pStrVar->m_lstStrVarDependencies.GetCount() == 0)
				strTemp = pTSCPCur->pStrVar->Get_strStrVarName();
			else
				strTemp.Format(IDS_STRING_TSVAR_DEPENDENT_STRING_VALUE, pTSCPCur->pStrVar->Get_strStrVarName());
			m_lstConditions.SetItemText(i, 1, strTemp);
			pTSCur->GetNextConditionPair(pos);
		}		

		if (m_lstConditions.GetItemCount() >= 1) 
			m_btnOK.EnableWindow(TRUE);
		else
			m_btnOK.EnableWindow(FALSE);
	}
}

void CDialogTriggeredStringProperties::OnDestroy() 
{
	CDialog::OnDestroy();
	m_lstConditions.SetImageList(m_pImListTriggeredStringPropertiesOld, LVSIL_SMALL);		
	m_ImListTriggeredStringProperties.DeleteImageList();	
}

void CDialogTriggeredStringProperties::OnButtonEditCondition() 
{
	CDialogTriggeredStringConditionProperties dlg;	
	S_TS_ConditionPair* pstsc;
	POSITION SelPos;
	int nItem;	
	CString strTemp;

	SelPos = m_lstConditions.GetFirstSelectedItemPosition();
	if (SelPos)
	{	
		nItem = m_lstConditions.GetNextSelectedItem(SelPos);
		pstsc = (S_TS_ConditionPair*) m_lstConditions.GetItemData(nItem);
		if (pstsc)
		{
			dlg.m_hwndlstConditionsParent = m_lstConditions.GetSafeHwnd();
			dlg.m_tsc.ConditionPairCur.pBVCondition = pstsc->pBVCondition;
			dlg.m_tsc.ConditionPairCur.pStrVar = pstsc->pStrVar;
			if (dlg.DoModal() == IDOK)
			{
				pstsc->pBVCondition = dlg.m_tsc.ConditionPairCur.pBVCondition;
				pstsc->pStrVar = dlg.m_tsc.ConditionPairCur.pStrVar;
				
				m_lstConditions.SetItemText(nItem, 0, pstsc->pBVCondition->Get_strBookVarName());					

				if (pstsc->pStrVar->m_lstStrVarDependencies.GetCount() == 0)
					strTemp = pstsc->pStrVar->Get_strStrVarName();
				else
					strTemp.Format(IDS_STRING_TSVAR_DEPENDENT_STRING_VALUE, pstsc->pStrVar->Get_strStrVarName());

				m_lstConditions.SetItemText(nItem, 1, strTemp);	
			}
		}
	}
}

void CDialogTriggeredStringProperties::OnItemchangedListConditions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uChanged) & LVIF_STATE)
	{
		if ((pNMListView->uNewState ) & LVIS_SELECTED)
		{
			m_btnEditCondition.EnableWindow(TRUE);
			m_btnDeleteCondition.EnableWindow(TRUE);
		}
		else
		{
			m_btnEditCondition.EnableWindow(FALSE);
			m_btnDeleteCondition.EnableWindow(FALSE);
		}
	}
	
	*pResult = 0;
}

void CDialogTriggeredStringProperties::OnDeleteCondition() 
{
	CDialogTriggeredStringConditionProperties dlg;	
	S_TS_ConditionPair* pstsc;
	POSITION SelPos;
	int nItem;	

	SelPos = m_lstConditions.GetFirstSelectedItemPosition();
	if (SelPos)
	{	
		nItem = m_lstConditions.GetNextSelectedItem(SelPos);
		pstsc = (S_TS_ConditionPair*) m_lstConditions.GetItemData(nItem);
		if (pstsc)
		{
			CString strCaption;
			CString strMessage;
			strMessage.LoadString(IDS_CONFIRM_DEL_TRIGGERED_STRING_CONDITION_PAIR);	
			strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
			if (MessageBox(strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				pTSCur->RemoveConditionPair(pstsc);
				RefreshLVContent();
				m_btnAddCondition.EnableWindow(TRUE);	// В любом случае можно добавить еще одну Condition Pair
			}
		}
	}
}

void CDialogTriggeredStringProperties::OnKeydownListConditions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if (pLVKeyDow->wVKey == VK_SPACE)
		OnButtonEditCondition();
	else if (pLVKeyDow->wVKey == VK_DELETE)
		OnDeleteCondition();
	
	*pResult = 0;
}

void CDialogTriggeredStringProperties::OnDblclkListConditions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEditCondition();
	
	*pResult = 0;
}

void CDialogTriggeredStringProperties::OnOK() 
{
	// TS не имеет смысла, если нет хотя бы двух вариантов (в т.ч. Default)
	// В любом случае, всегда можно нажать Cancel
	if (m_lstConditions.GetItemCount() >= 1)
	{
		CString strErrorMessage;
		CString strErrorCaption;
		BOOL fOk = true;	// Будем оптимистами!
		BOOL fVarChanged;
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
		CNLB_BldrDoc* pDoc;
		int iTSDefValCurSel;
		
		if (pMainFrame)
		{
			pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
			ASSERT_VALID(pDoc);		
			UpdateData(TRUE);
			fVarChanged = (m_strInitialTSVarName != m_strTSVarName);

			if (((pDoc->FindPageIndexByVar(m_strTSVarName) != -1) ||
				(pDoc->FindBookVarPositionByName(m_strTSVarName)) ||
				(pDoc->FindProbVarPositionByName(m_strTSVarName)) ||
				(pDoc->FindStrVarPositionByName(m_strTSVarName)) ||
				(pDoc->FindTSVarPositionByName(m_strTSVarName))) && (fVarChanged))
			{
				fOk = false;	
				strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
				strErrorMessage.Format(IDS_ERROR_VAR_ALREADY_EXISTS, m_strTSVarName);
				MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
				m_edtTSVarName.SetSel(0, -1);	// Выделить весь текст в Edit
				m_edtTSVarName.SetFocus();
			}

			if (!CheckVarCorrect(m_strTSVarName))
			{
				fOk = false;	
				strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
				strErrorMessage.Format(IDS_ERROR_VAR_INCORRECT, m_strTSVarName);
				MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
				m_edtTSVarName.SetSel(0, -1);	// Выделить весь текст в Edit
				m_edtTSVarName.SetFocus();
			}		

			if (fOk)
			{
				iTSDefValCurSel = m_cbbDefaultValue.GetCurSel();
				pTSCur->SetDefaultValue((CStringVar*) m_cbbDefaultValue.GetItemData(iTSDefValCurSel));
				CDialog::OnOK();
				pDoc->SetModifiedFlag(TRUE);
			}
		}	
	}
}
