// DialogBookVars.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "consts.h"
#include "DialogBookVars.h"
#include "NLB_BldrDoc.h"
#include "MainFrm.h"
#include "DialogBookVarProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogBookVars dialog


CDialogBookVars::CDialogBookVars(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBookVars::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogBookVars)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iIconPos = -1;
}


void CDialogBookVars::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBookVars)
	DDX_Control(pDX, IDC_BUTTON_EDIT_BOOKVAR, m_btnEditBookVar);
	DDX_Control(pDX, IDC_BUTTON_DELETE_BOOKVAR, m_btnDeleteBookVar);
	DDX_Control(pDX, IDC_LIST_BOOKVARS, m_lstBookVars);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBookVars, CDialog)
	//{{AFX_MSG_MAP(CDialogBookVars)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ADD_BOOKVAR, OnButtonAddBookvar)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_BOOKVAR, OnButtonEditBookvar)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_BOOKVAR, OnButtonDeleteBookvar)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BOOKVARS, OnItemchangedListBookvars)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BOOKVARS, OnDblclkListBookvars)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_BOOKVARS, OnKeydownListBookvars)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBookVars message handlers

BOOL CDialogBookVars::OnInitDialog() 
{
	BeginWaitCursor();
	POSITION pos;
	CBookVar* pBookVarCur;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrDoc* pDoc;
	int iBookVarCount;

	CDialog::OnInitDialog();	

	HICON hIconNew = AfxGetApp()->LoadIcon(IDR_BOOKVAR);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstBookVars.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_BOOKVAR_NAME);		
	m_lstBookVars.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVBOOKVARS_COL0_WIDTH,
		0);

	lpszColumnHeading.LoadString(IDS_UITXT_LV_BOOKVAR_BODY);		
	m_lstBookVars.InsertColumn(1,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVBOOKVARS_COL1_WIDTH,
		0);
	
	m_ImListBookVars.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								1, 
								1);
	m_iIconPos = m_ImListBookVars.Add(hIconNew);
	m_pImListBookVarsOld = m_lstBookVars.SetImageList(&m_ImListBookVars, LVSIL_SMALL);	

	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{			
			iBookVarCount = pDoc->m_lstBookVars.GetCount();
			pos = pDoc->m_lstBookVars.GetHeadPosition();			
			for (int i = 0; i < iBookVarCount; i++)
			{
				pBookVarCur = pDoc->m_lstBookVars.GetAt(pos);
				m_lstBookVars.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
						i,
						pBookVarCur->Get_strBookVarName(),
						0,
						0,
						m_iIconPos,
						(LPARAM) pBookVarCur);		
				m_lstBookVars.SetItemText(i, 1, pBookVarCur->Get_strBookVarBody());				
				pDoc->m_lstBookVars.GetNext(pos);
			}			

			m_btnEditBookVar.EnableWindow(FALSE);
			m_btnDeleteBookVar.EnableWindow(FALSE);
		}
	}
	
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogBookVars::OnDestroy() 
{
	CDialog::OnDestroy();	
	m_lstBookVars.SetImageList(m_pImListBookVarsOld, LVSIL_SMALL);		
	m_ImListBookVars.DeleteImageList();	
}

void CDialogBookVars::OnButtonAddBookvar() 
{
	CBookVar* pBookVarNew;
	CDialogBookVarProperties dlg;
	CNLB_BldrDoc* pDoc;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	int ciItems;										
						
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			dlg.m_strBookVarName = _T("");	// Новая Book Variable
			dlg.m_fIsVarGlobal = FALSE;
			if (dlg.DoModal() == IDOK)
			{
				if ((pBookVarNew = new CBookVar) == NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
				pBookVarNew->Get_strBookVarName() = dlg.m_strBookVarName;
				pBookVarNew->Get_strBookVarBody() = dlg.m_strBookVarBody;			
				pBookVarNew->Get_fIsVarGlobal() = dlg.m_fIsVarGlobal;
				pDoc->m_lstBookVars.AddHead(pBookVarNew);
				ciItems = m_lstBookVars.GetItemCount();	
				m_lstBookVars.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
					ciItems,
					pBookVarNew->Get_strBookVarName(),
					0,
					0,
					m_iIconPos,
					(LPARAM) pBookVarNew);		
				m_lstBookVars.SetItemText(ciItems, 1, pBookVarNew->Get_strBookVarBody());	
			}
		}
	}
}

void CDialogBookVars::OnButtonEditBookvar() 
{
	CDialogBookVarProperties dlg;
	CBookVar* pBookVarToEdit;
	POSITION SelPos;
	int nItem;
		
	SelPos = m_lstBookVars.GetFirstSelectedItemPosition();
	if (SelPos)
	{	
		nItem = m_lstBookVars.GetNextSelectedItem(SelPos);
		pBookVarToEdit = (CBookVar*) m_lstBookVars.GetItemData(nItem);
		if (pBookVarToEdit)
		{
			dlg.m_strBookVarName = pBookVarToEdit->Get_strBookVarName();
			dlg.m_strBookVarBody  = pBookVarToEdit->Get_strBookVarBody();
			dlg.m_fIsVarGlobal = pBookVarToEdit->Get_fIsVarGlobal();
			if (dlg.DoModal() == IDOK)
			{
				pBookVarToEdit->Get_strBookVarName() = dlg.m_strBookVarName;
				pBookVarToEdit->Get_strBookVarBody() = dlg.m_strBookVarBody;
				pBookVarToEdit->Get_fIsVarGlobal() = dlg.m_fIsVarGlobal;
				m_lstBookVars.SetItemText(nItem, 0, pBookVarToEdit->Get_strBookVarName());	
				m_lstBookVars.SetItemText(nItem, 1, pBookVarToEdit->Get_strBookVarBody());	
			}
		}
	}
}

void CDialogBookVars::OnButtonDeleteBookvar() 
{
	CBookVar* pBookVarToDelete;	
	CBookVar* pBookVarCur;	
	CNLB_BldrDoc* pDoc;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	POSITION pos;
	int nItem;
	int iBookVarCount;										
						
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			CString strCaption;
			CString strMessage;
			strMessage.LoadString(IDS_CONFIRM_DEL_BOOKVAR);	
			strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
			if (MessageBox(strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
			{				
				pos = m_lstBookVars.GetFirstSelectedItemPosition();
				if (pos)
				{	
					nItem = m_lstBookVars.GetNextSelectedItem(pos);
					pBookVarToDelete = (CBookVar*) m_lstBookVars.GetItemData(nItem);				
					iBookVarCount = pDoc->m_lstBookVars.GetCount();
					pos = pDoc->m_lstBookVars.GetHeadPosition();			
					for (int i = 0; i < iBookVarCount; i++)
					{
						pBookVarCur = pDoc->m_lstBookVars.GetAt(pos);
						if (pBookVarCur == pBookVarToDelete)
						{
							delete pBookVarToDelete;
							pDoc->m_lstBookVars.RemoveAt(pos);
							m_lstBookVars.DeleteItem(nItem);
							break;
						}
						pDoc->m_lstBookVars.GetNext(pos);
					}
				}	
			}
		}
	}
}

void CDialogBookVars::OnItemchangedListBookvars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*) pNMHDR;
		
	if ((pNMListView->uChanged) & LVIF_STATE)
	{
		if ((pNMListView->uNewState ) & LVIS_SELECTED)
		{
			m_btnEditBookVar.EnableWindow(TRUE);
			m_btnDeleteBookVar.EnableWindow(TRUE);
		}
		else
		{
			m_btnEditBookVar.EnableWindow(FALSE);
			m_btnDeleteBookVar.EnableWindow(FALSE);
		}
	}
	
	*pResult = 0;
}

void CDialogBookVars::OnDblclkListBookvars(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonEditBookvar();
	*pResult = 0;
}

void CDialogBookVars::OnKeydownListBookvars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if (pLVKeyDow->wVKey == VK_SPACE)
		OnButtonEditBookvar();
	else if (pLVKeyDow->wVKey == VK_DELETE)
		OnButtonDeleteBookvar();	

	*pResult = 0;
}
