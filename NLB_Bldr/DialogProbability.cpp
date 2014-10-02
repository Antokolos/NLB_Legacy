// DialogProbability.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "consts.h"
#include "CProbVar.h"
#include "MainFrm.h"
#include "NLB_BldrDoc.h"
#include "DialogProbability.h"
#include "DialogProbVarProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogProbability dialog


CDialogProbability::CDialogProbability(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProbability::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogProbability)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iIconPos = -1;
}


void CDialogProbability::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogProbability)
	DDX_Control(pDX, IDC_BUTTON_EDIT_PROBVAR, m_btnEditProbVar);
	DDX_Control(pDX, IDC_BUTTON_DELETE_PROBVAR, m_btnDeleteProbVar);
	DDX_Control(pDX, IDC_BUTTON_ADD_PROBVAR, m_btnAddProbVar);
	DDX_Control(pDX, IDC_LIST_PROBABILITY_VARS, m_lstProbVars);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogProbability, CDialog)
	//{{AFX_MSG_MAP(CDialogProbability)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ADD_PROBVAR, OnButtonAddProbvar)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PROBVAR, OnButtonEditProbvar)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_PROBVAR, OnButtonDeleteProbvar)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROBABILITY_VARS, OnItemchangedListProbabilityVars)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROBABILITY_VARS, OnDblclkListProbabilityVars)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_PROBABILITY_VARS, OnKeydownListProbabilityVars)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogProbability message handlers

BOOL CDialogProbability::OnInitDialog() 
{		
	BeginWaitCursor();
	CString strProbVarChance;
	POSITION pos;
	CProbVar* pProbVarCur;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrDoc* pDoc;
	int iProbVarCount;

	CDialog::OnInitDialog();	

	HICON hIconNew = AfxGetApp()->LoadIcon(IDR_PROBVAR);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstProbVars.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_PROBVAR_NAME);		
	m_lstProbVars.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVPROBVARS_COL0_WIDTH,
		0);

	lpszColumnHeading.LoadString(IDS_UITXT_LV_PROBVAR_CHANCE);		
	m_lstProbVars.InsertColumn(1,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVPROBVARS_COL1_WIDTH,
		0);
	
	m_ImListProbVars.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								1, 
								1);
	m_iIconPos = m_ImListProbVars.Add(hIconNew);
	m_pImListProbVarsOld = m_lstProbVars.SetImageList(&m_ImListProbVars, LVSIL_SMALL);	

	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{			
			iProbVarCount = pDoc->m_lstProbVars.GetCount();
			pos = pDoc->m_lstProbVars.GetHeadPosition();			
			for (int i = 0; i < iProbVarCount; i++)
			{
				pProbVarCur = pDoc->m_lstProbVars.GetAt(pos);
				m_lstProbVars.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
						i,
						pProbVarCur->Get_strProbVarName(),
						0,
						0,
						m_iIconPos,
						(LPARAM) pProbVarCur);		
				strProbVarChance.Format(IDS_PROBVAR_CHANCE_BUF, pProbVarCur->Get_iDiceK(), pProbVarCur->Get_iDiceY());
				m_lstProbVars.SetItemText(i, 1, strProbVarChance);				
				pDoc->m_lstProbVars.GetNext(pos);
			}					
			
			m_btnEditProbVar.EnableWindow(FALSE);
			m_btnDeleteProbVar.EnableWindow(FALSE);
		}
	}
	
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE		
}

void CDialogProbability::OnDestroy() 
{
	CDialog::OnDestroy();
	m_lstProbVars.SetImageList(m_pImListProbVarsOld, LVSIL_SMALL);		
	m_ImListProbVars.DeleteImageList();	
}

void CDialogProbability::OnButtonAddProbvar() 
{
	CProbVar* pProbVarNew;
	CString strProbVarChance;
	CDialogProbVarProperties dlg;
	CNLB_BldrDoc* pDoc;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	int ciItems;										
						
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			dlg.m_strProbVarName = _T("");	// Новая вероятностная переменная
			dlg.m_iDiceK = 1;
			dlg.m_iDiceY = 2;
			if (dlg.DoModal() == IDOK)
			{
				if ((pProbVarNew = new CProbVar) == NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
				pProbVarNew->Get_strProbVarName() = dlg.m_strProbVarName;
				pProbVarNew->Set_iDiceK(dlg.m_iDiceK);			
				pProbVarNew->Set_iDiceY(dlg.m_iDiceY);
				pDoc->m_lstProbVars.AddHead(pProbVarNew);
				ciItems = m_lstProbVars.GetItemCount();	
				m_lstProbVars.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
					ciItems,
					pProbVarNew->Get_strProbVarName(),
					0,
					0,
					m_iIconPos,
					(LPARAM) pProbVarNew);		
				strProbVarChance.Format(IDS_PROBVAR_CHANCE_BUF, pProbVarNew->Get_iDiceK(), pProbVarNew->Get_iDiceY());
				m_lstProbVars.SetItemText(ciItems, 1, strProbVarChance);				
			}
		}
	}	
}

void CDialogProbability::OnButtonEditProbvar() 
{
	CDialogProbVarProperties dlg;
	CString strProbVarChance;
	CProbVar* pProbVarToEdit;
	POSITION SelPos;
	int nItem;
		
	SelPos = m_lstProbVars.GetFirstSelectedItemPosition();
	if (SelPos)
	{	
		nItem = m_lstProbVars.GetNextSelectedItem(SelPos);
		pProbVarToEdit = (CProbVar*) m_lstProbVars.GetItemData(nItem);
		if (pProbVarToEdit)
		{
			dlg.m_strProbVarName = pProbVarToEdit->Get_strProbVarName();
			dlg.m_iDiceK = pProbVarToEdit->Get_iDiceK();
			dlg.m_iDiceY = pProbVarToEdit->Get_iDiceY();
			if (dlg.DoModal() == IDOK)
			{
				pProbVarToEdit->Get_strProbVarName() = dlg.m_strProbVarName;
				pProbVarToEdit->Set_iDiceK(dlg.m_iDiceK);			
				pProbVarToEdit->Set_iDiceY(dlg.m_iDiceY);
				strProbVarChance.Format(IDS_PROBVAR_CHANCE_BUF, pProbVarToEdit->Get_iDiceK(), pProbVarToEdit->Get_iDiceY());
				m_lstProbVars.SetItemText(nItem, 0, pProbVarToEdit->Get_strProbVarName());	
				m_lstProbVars.SetItemText(nItem, 1, strProbVarChance);	
			}
		}
	}
}

void CDialogProbability::OnButtonDeleteProbvar() 
{
	CProbVar* pProbVarToDelete;	
	CProbVar* pProbVarCur;	
	CNLB_BldrDoc* pDoc;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	POSITION pos;
	int nItem;
	int iProbVarCount;										
						
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			CString strCaption;
			CString strMessage;
			strMessage.LoadString(IDS_CONFIRM_DEL_PROBVAR);	
			strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
			if (MessageBox(strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
			{				
				pos = m_lstProbVars.GetFirstSelectedItemPosition();
				if (pos)
				{	
					nItem = m_lstProbVars.GetNextSelectedItem(pos);
					pProbVarToDelete = (CProbVar*) m_lstProbVars.GetItemData(nItem);				
					iProbVarCount = pDoc->m_lstProbVars.GetCount();
					pos = pDoc->m_lstProbVars.GetHeadPosition();			
					for (int i = 0; i < iProbVarCount; i++)
					{
						pProbVarCur = pDoc->m_lstProbVars.GetAt(pos);
						if (pProbVarCur == pProbVarToDelete)
						{
							delete pProbVarToDelete;
							pDoc->m_lstProbVars.RemoveAt(pos);
							m_lstProbVars.DeleteItem(nItem);
							break;
						}
						pDoc->m_lstProbVars.GetNext(pos);
					}
				}	
			}
		}
	}	
}

void CDialogProbability::OnItemchangedListProbabilityVars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ((pNMListView->uChanged) & LVIF_STATE)
	{
		if ((pNMListView->uNewState ) & LVIS_SELECTED)
		{
			m_btnEditProbVar.EnableWindow(TRUE);
			m_btnDeleteProbVar.EnableWindow(TRUE);
		}
		else
		{
			m_btnEditProbVar.EnableWindow(FALSE);
			m_btnDeleteProbVar.EnableWindow(FALSE);
		}
	}	
	*pResult = 0;
}

void CDialogProbability::OnDblclkListProbabilityVars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEditProbvar();	
	*pResult = 0;
}

void CDialogProbability::OnKeydownListProbabilityVars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if (pLVKeyDow->wVKey == VK_SPACE)
		OnButtonEditProbvar();
	else if (pLVKeyDow->wVKey == VK_DELETE)
		OnButtonDeleteProbvar();	

	*pResult = 0;
}
