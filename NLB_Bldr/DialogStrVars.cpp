// DialogStrVars.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "DialogStrVars.h"
#include "DialogStrVarProperties.h"
#include "consts.h"
#include "NLB_BldrDoc.h"
#include "MainFrm.h"
#include "CStringVar.h"
#include "funcs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogStrVars dialog


CDialogStrVars::CDialogStrVars(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogStrVars::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogStrVars)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iIconPos = -1;
}


void CDialogStrVars::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogStrVars)
	DDX_Control(pDX, IDC_BUTTON_DELETE_STRVAR, m_btnDeleteStrVar);
	DDX_Control(pDX, IDC_BUTTON_EDIT_STRVAR, m_btnEditStrVar);
	DDX_Control(pDX, IDC_LIST_STRVARS, m_lstStrVars);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogStrVars, CDialog)
	//{{AFX_MSG_MAP(CDialogStrVars)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ADD_STRVAR, OnButtonAddStrvar)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_STRVAR, OnButtonDeleteStrvar)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_STRVAR, OnButtonEditStrvar)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STRVARS, OnItemchangedListStrvars)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STRVARS, OnDblclkListStrvars)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_STRVARS, OnKeydownListStrvars)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogStrVars message handlers

BOOL CDialogStrVars::OnInitDialog() 
{
	BeginWaitCursor();
	POSITION pos;
	CStringVar* pStrVarCur;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrDoc* pDoc;
	int iStrVarCount;
	CString strVarBodyToSetInLV;
	CString strCR;
	CString strCRSymbolic;

	CDialog::OnInitDialog();	

	HICON hIconNew = AfxGetApp()->LoadIcon(IDR_STRVAR);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstStrVars.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_STRVAR_NAME);		
	m_lstStrVars.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVSTRVARS_COL0_WIDTH,
		0);

	lpszColumnHeading.LoadString(IDS_UITXT_LV_STRVAR_BODY);		
	m_lstStrVars.InsertColumn(1,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVSTRVARS_COL1_WIDTH,
		0);
	
	m_ImListStrVars.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								1, 
								1);
	m_iIconPos = m_ImListStrVars.Add(hIconNew);
	m_pImListStrVarsOld = m_lstStrVars.SetImageList(&m_ImListStrVars, LVSIL_SMALL);	

	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{			
			iStrVarCount = pDoc->m_lstStrVars.GetCount();
			pos = pDoc->m_lstStrVars.GetHeadPosition();			
			for (int i = 0; i < iStrVarCount; i++)
			{
				pStrVarCur = pDoc->m_lstStrVars.GetAt(pos);
				m_lstStrVars.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
						i,
						pStrVarCur->Get_strStrVarName(),
						0,
						0,
						m_iIconPos,
						(LPARAM) pStrVarCur);		
				strVarBodyToSetInLV = pStrVarCur->Get_strStrVarBody();
				strCR.LoadString(IDS_CR);
				strCRSymbolic.LoadString(IDS_CR_SYMBOLIC);
				strVarBodyToSetInLV.Replace(strCR, strCRSymbolic);
				m_lstStrVars.SetItemText(i, 1, strVarBodyToSetInLV);
				pDoc->m_lstStrVars.GetNext(pos);
			}

			m_btnEditStrVar.EnableWindow(FALSE);
			m_btnDeleteStrVar.EnableWindow(FALSE);
		}
	}
	
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogStrVars::OnDestroy() 
{
	CDialog::OnDestroy();
	m_lstStrVars.SetImageList(m_pImListStrVarsOld, LVSIL_SMALL);		
	m_ImListStrVars.DeleteImageList();	
}

void CDialogStrVars::OnButtonAddStrvar() 
{
	CStringVar* pStrVarNew;
	CDialogStrVarProperties dlg;	
	CNLB_BldrDoc* pDoc;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	int ciItems;				
	CString strVarBodyToSetInLV;
	CString strCR;
	CString strCRSymbolic;	
						
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{			
			dlg.m_strStrVarName = _T("");	// Новая Str Variable			
			if (dlg.DoModal() == IDOK)
			{
				if ((pStrVarNew = new CStringVar) == NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
				pStrVarNew->Get_strStrVarName() = dlg.m_strStrVarName;
				pStrVarNew->Get_strStrVarBody() = dlg.m_strStrVarBody;					
				CStringVar::ListAssign(pStrVarNew->m_lstStrVarParameters, dlg.m_lstStrVarParameters);
				CStringVar::ListAssign(pStrVarNew->m_lstStrVarDependencies, dlg.m_lstStrVarDependencies);
				pStrVarNew->Get_fSaveInTheBookmarksFile() = dlg.m_fSaveInTheBookmarksFile;	

				pDoc->m_lstStrVars.AddHead(pStrVarNew);
				ciItems = m_lstStrVars.GetItemCount();	
				m_lstStrVars.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
					ciItems,
					pStrVarNew->Get_strStrVarName(),
					0,
					0,
					m_iIconPos,
					(LPARAM) pStrVarNew);	
				strVarBodyToSetInLV = pStrVarNew->Get_strStrVarBody();
				strCR.LoadString(IDS_CR);
				strCRSymbolic.LoadString(IDS_CR_SYMBOLIC);
				strVarBodyToSetInLV.Replace(strCR, strCRSymbolic);
				m_lstStrVars.SetItemText(ciItems, 1, strVarBodyToSetInLV);	
			}
		}
	}	
}

void CDialogStrVars::OnButtonDeleteStrvar() 
{
	CStringVar* pStrVarToDelete;	
	CStringVar* pStrVarCur;	
	CNLB_BldrDoc* pDoc;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	POSITION pos;
	int nItem;
	int iStrVarCount;										
						
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			CString strCaption;
			CString strMessage;
			strMessage.LoadString(IDS_CONFIRM_DEL_STRVAR);	
			strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
			if (MessageBox(strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
			{				
				pos = m_lstStrVars.GetFirstSelectedItemPosition();
				if (pos)
				{	
					nItem = m_lstStrVars.GetNextSelectedItem(pos);
					pStrVarToDelete = (CStringVar*) m_lstStrVars.GetItemData(nItem);				
					iStrVarCount = pDoc->m_lstStrVars.GetCount();
					pos = pDoc->m_lstStrVars.GetHeadPosition();			
					for (int i = 0; i < iStrVarCount; i++)
					{
						pStrVarCur = pDoc->m_lstStrVars.GetAt(pos);
						if (pStrVarCur == pStrVarToDelete)
						{
							delete pStrVarToDelete;
							pDoc->m_lstStrVars.RemoveAt(pos);
							m_lstStrVars.DeleteItem(nItem);
							break;
						}
						pDoc->m_lstStrVars.GetNext(pos);
					}
				}	
			}
		}
	}
}

void CDialogStrVars::OnButtonEditStrvar() 
{
	CDialogStrVarProperties dlg;
	CStringVar* pStrVarToEdit;
	POSITION SelPos;
	int nItem;
	CString strVarBodyToSetInLV;
	CString strCR;
	CString strCRSymbolic;	

	SelPos = m_lstStrVars.GetFirstSelectedItemPosition();
	if (SelPos)
	{	
		nItem = m_lstStrVars.GetNextSelectedItem(SelPos);
		pStrVarToEdit = (CStringVar*) m_lstStrVars.GetItemData(nItem);
		if (pStrVarToEdit)
		{
			dlg.m_strStrVarName = pStrVarToEdit->Get_strStrVarName();
			dlg.m_strStrVarBody  = pStrVarToEdit->Get_strStrVarBody();	
			CStringVar::ListAssign(dlg.m_lstStrVarParameters, pStrVarToEdit->m_lstStrVarParameters);
			CStringVar::ListAssign(dlg.m_lstStrVarDependencies, pStrVarToEdit->m_lstStrVarDependencies);
			dlg.m_fSaveInTheBookmarksFile = pStrVarToEdit->Get_fSaveInTheBookmarksFile();
			if (dlg.DoModal() == IDOK)
			{
				pStrVarToEdit->Get_strStrVarName() = dlg.m_strStrVarName;
				pStrVarToEdit->Get_strStrVarBody() = dlg.m_strStrVarBody;	
				CStringVar::ListAssign(pStrVarToEdit->m_lstStrVarParameters, dlg.m_lstStrVarParameters);
				CStringVar::ListAssign(pStrVarToEdit->m_lstStrVarDependencies, dlg.m_lstStrVarDependencies);
				pStrVarToEdit->Get_fSaveInTheBookmarksFile() = dlg.m_fSaveInTheBookmarksFile;	
				m_lstStrVars.SetItemText(nItem, 0, pStrVarToEdit->Get_strStrVarName());	
				strVarBodyToSetInLV = pStrVarToEdit->Get_strStrVarBody();
				strCR.LoadString(IDS_CR);
				strCRSymbolic.LoadString(IDS_CR_SYMBOLIC);
				strVarBodyToSetInLV.Replace(strCR, strCRSymbolic);
				m_lstStrVars.SetItemText(nItem, 1, strVarBodyToSetInLV);	
			}
		}
	}
}

void CDialogStrVars::OnItemchangedListStrvars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ((pNMListView->uChanged) & LVIF_STATE)
	{
		if ((pNMListView->uNewState ) & LVIS_SELECTED)
		{
			m_btnEditStrVar.EnableWindow(TRUE);
			m_btnDeleteStrVar.EnableWindow(TRUE);
		}
		else
		{
			m_btnEditStrVar.EnableWindow(FALSE);
			m_btnDeleteStrVar.EnableWindow(FALSE);
		}
	}
		
	*pResult = 0;
}

void CDialogStrVars::OnDblclkListStrvars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEditStrvar();
	
	*pResult = 0;
}

void CDialogStrVars::OnKeydownListStrvars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if (pLVKeyDow->wVKey == VK_SPACE)
		OnButtonEditStrvar();
	else if (pLVKeyDow->wVKey == VK_DELETE)
		OnButtonDeleteStrvar();
	
	*pResult = 0;
}
